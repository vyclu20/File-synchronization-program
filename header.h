#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>  
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <time.h>
#include <regex.h>
#include <getopt.h>


#define PATTERNMAX 100

// Options
extern bool allFunc;       // -a
extern bool verbFunc;        // -v
extern bool noChangeFunc;       // -n
extern bool folderIterateFunc; // -r
extern bool permSame; // -p

extern char* ignoreFunc[];  
extern int ignoreCount;

extern char* onlyFunc[];
extern int onlyCount;

// inside patternmatch.c
char *glob2regex(char *glob);
bool fileNameCan(const char *filename);
bool hiddenDirectory(const char *name);

// inside filemaking.c
void copyFile(const char *first, const char *second);
void copyDirectory(const char *first, const char *second);

// inside directorychecking.c
void content(const char *directory);
bool insideDirectory(const char *dirPath);
