#include "header.h"

// This entire source file is for pattern matching stuff (-i, -o, -a)
// Where the 1st function is the given regex function
// The second one is for -i/-o of files/directories and -a of files, so the -a is just the top part where it checks for the . and the rest is regex stuff
// The third function is only for -a of directories. Because me putting the -a in the 2nd function messed up the hidden directory checks due to
// regex stuff, so I've just made a separate function for it.

//  CONVERT A FILENAME PATTERN (a glob) TO A REGULAR EXPRESSION.
//  FILENAME PATTERNS MAY NOT INCLUDE DIRECTORY SEPARATORS.
//	ON SUCCESS - A REGULAR EXPRESSION WILL BE RETURNED IN DYNAMICALLY
//		     ALLOCATED MEMORY.
//	ON FAILURE - A NULL POINTER WILL BE RETURNED.

// This is the first function
char *glob2regex(char *glob)
{
    char *re = NULL;

    if(glob != NULL) {
	re	= calloc(strlen(glob)*2 + 4, sizeof(char));
        if(re == NULL) {
	    return NULL;
	}

	char *r	= re;

	*r++	= '^';
	while(*glob != '\0')
	    switch (*glob) {
		case '.' :
		case '\\':
		case '$' : *r++ = '\\'; *r++ = *glob++;	break;
		case '*' : *r++ = '.';  *r++ = *glob++;	break;
		case '?' : *r++ = '.'; glob++;		break;
		case '/' : free(re);
			   re	= NULL;
			   break;
		default  : *r++ = *glob++;
			   break;
	    }
	if(re) {
	    *r++	= '$';
	    *r		= '\0';
	}
    }
    return re;
}

// This is the second function
bool fileNameCan(const char *filename) {
    // So if -a is not called, then the default is to ignore hidden files
    if (!allFunc && filename[0] == '.') {
        return false;
    }

    regex_t regex;
    for (int i = 0; i < ignoreCount; i++) {
        regcomp(&regex, ignoreFunc[i], REG_EXTENDED);
        if (!regexec(&regex, filename, 0, NULL, 0)) {  // ignore pattern
            regfree(&regex);
            return false;
        }
        regfree(&regex);
    }

    if (onlyCount == 0) return true;

    for (int i = 0; i < onlyCount; i++) {
        regcomp(&regex, onlyFunc[i], REG_EXTENDED);
        if (!regexec(&regex, filename, 0, NULL, 0)) {  // only pattern
            regfree(&regex);
            return true;
        }
        regfree(&regex);
    }

    return false;
}

// This is the third function, the one where I was talking about above
bool hiddenDirectory(const char *name) {
    // Checks if -a is not called and if there is a . at the start
    if (!allFunc && name[0] == '.') {
        return true;  // should be hidden
    }
    return false;  // should not be hidden
}
