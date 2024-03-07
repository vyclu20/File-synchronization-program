#include "header.h"

// So this source file is for functions that copy files and make directories

void copyFile(const char *first, const char *second) {
    struct stat firstUse;
    if (stat(first, &firstUse) == 0 && S_ISDIR(firstUse.st_mode)) {
        printf("copyFile called on a directory: %s. you can't.\n", first);
        return;
    }
    // Checks for -n at the start
    if (noChangeFunc) {  // if no -n
        printf("Would copy, but is not actually copying: %s to %s\n", first, second);
        return; // abort do nothing
    }

    // Actual file copying operation
    FILE *firstFile = fopen(first, "rb");
    FILE *secondFile = fopen(second, "wb");
    char ioPower[4096];
    size_t fileBytes;

    if (!firstFile || !secondFile) {
        perror("Couldn't open file to copy");
        exit(EXIT_FAILURE);
    }

    while ((fileBytes = fread(ioPower, 1, sizeof(ioPower), firstFile)) > 0) {
        fwrite(ioPower, 1, fileBytes, secondFile);
    }

    fclose(firstFile);
    fclose(secondFile);

    // Keep the time and permissions the same if -p 
    if (permSame) {
        struct stat firstUses;
        struct utimbuf sameTime;

        if (stat(first, &firstUses) == 0) {
            sameTime.actime = firstUses.st_atime;
            sameTime.modtime = firstUses.st_mtime;

            utime(second, &sameTime);  // Set access and modification times
            chmod(second, firstUses.st_mode);  // Set file permissions
        }
    }
}

void copyDirectory(const char *first, const char *second) {
    DIR *dir;
    struct dirent *entry;
    char firstRoute[1000], secondRoute[1000];

    // Create the directory
    if (mkdir(second, 0700) != 0) {
        perror("Failed to create directory");
        return;
    } else {
        printf("Directory %s created successfully.\n", second);
    }

    // Open source directory
    dir = opendir(first);
    if (!dir) {
        perror("Error opening source directory");
        return;
    }

    // Iterate over each entry in the source directory
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip 
        }

        snprintf(firstRoute, sizeof(firstRoute), "%s/%s", first, entry->d_name);
        snprintf(secondRoute, sizeof(secondRoute), "%s/%s", second, entry->d_name);

        struct stat ptwy;
        if (stat(firstRoute, &ptwy) == -1) {
            perror("Error stating source entry");
            continue;
        }

        if (S_ISDIR(ptwy.st_mode)) {
            // If directory, -r
            copyDirectory(firstRoute, secondRoute);
        } else {
            // If file, normal
            copyFile(firstRoute, secondRoute);
        }
    }

    closedir(dir);
}
