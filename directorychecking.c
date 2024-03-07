#include "header.h"

// This source file is for functions that does directory stuff
// The first one opens, reads, processes and closes directories
// The second one is actually kind of related to the stuff in patternmatch.c, but I've decided to put it here because it's more directory related

// Explanation for the second function
// So for the -i -o and -r, if we match a certain pattern with -r, we want to copy the folder(s) where the pattern is found/ignored, but we don't want those that 
// are not detected by the -i and -o if that makes sense

// For example if we did - r -o '*.[ch]', we want to copy folders that have files with either a .c or .h extension, along with the .c and .h files in it
// We don't want other files that are not .c or .h from that folder
// So that is what the second function is for. It checks if the directory contains the wanted files 

// First function
// Lists the contents of the directory
void content(const char *directory) {
    //printf("brbr directory: %s\n", directory); 
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directory);
    if (!dir) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        // If -a option is not set, skip 
        if (!allFunc && entry->d_name[0] == '.') {
            continue;
        }
        // If -v mode is set, print directory content
        if (verbFunc) {
            printf("%s/%s\n", directory, entry->d_name);
        }
    }

    closedir(dir);
}

// Second function
bool insideDirectory(const char *dirPath) {
    struct dirent *entry;
    struct stat buf; // to check if it is a directory
    char insideRoute[1000];
    bool fileCanFR = false;

    DIR *dirStream = opendir(dirPath);
    if (dirStream == NULL) {
        perror("Failed to open directory");
        return false;
    }

    while ((entry = readdir(dirStream)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(insideRoute, sizeof(insideRoute), "%s/%s", dirPath, entry->d_name);

        if (stat(insideRoute, &buf) == -1) {
            perror("stat error for insideRoute");
            continue; // if there's an error getting stats for the file, skip 
        }

        // If it's a file and should be copied, then the folder should be copied
        if (!S_ISDIR(buf.st_mode) && fileNameCan(entry->d_name)) {
            fileCanFR = true;
            break;
        }
        // If it's a directory, -r
        else if (S_ISDIR(buf.st_mode) && insideDirectory(insideRoute)) {
            fileCanFR = true;
            break;
        }
    }

    closedir(dirStream);
    return fileCanFR;
}
