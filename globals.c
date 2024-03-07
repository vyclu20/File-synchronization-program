#include "header.h"

//  CITS2002 Project 2 2023
//  Student1:   23484481   Victoria Felice

bool allFunc = false;       // -a
bool verbFunc = false;        // -v
bool noChangeFunc = false;       // -n
bool folderIterateFunc = false; // -r
bool permSame = false; // -p

char* ignoreFunc[PATTERNMAX];  
int ignoreCount = 0;

char* onlyFunc[PATTERNMAX];
int onlyCount = 0;

