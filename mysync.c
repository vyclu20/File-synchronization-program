#include "header.h"

//  CITS2002 Project 2 2023
//  Student1:   23484481   Victoria Felice

// This c source file contains the main as well as the function that does the syncing operations

// This one is here for recursive
void syncing(const char *firstFolder, const char *secondFolder);

void processDirectory(const char *sourceFolder, const char *targetFolder) {
    DIR *sourceFolderPTWY;
    struct dirent *sourceCan;
    struct stat sourceStat, targetStat;
    char sourcePTWYcan[1000], targetPTWYcan[1000];

    sourceFolderPTWY = opendir(sourceFolder);
    if (!sourceFolderPTWY) {
        return;
    }

    if (verbFunc) {
        printf("Processing directory: %s to %s\n", sourceFolder, targetFolder);
    }

    while ((sourceCan = readdir(sourceFolderPTWY)) != NULL) {
        if (strcmp(sourceCan->d_name, ".") == 0 || strcmp(sourceCan->d_name, "..") == 0) {
            continue;  // Skip dot stuff
        }

        if (hiddenDirectory(sourceCan->d_name)) {
            if (verbFunc) {
                printf("Skipping hidden file or directory: %s\n", sourceCan->d_name);
            }
            continue;
        }

        sprintf(sourcePTWYcan, "%s/%s", sourceFolder, sourceCan->d_name);
        sprintf(targetPTWYcan, "%s/%s", targetFolder, sourceCan->d_name);

        if (stat(sourcePTWYcan, &sourceStat) == -1) {
            perror("Error for source file path");
            continue;
        }

        if (S_ISDIR(sourceStat.st_mode)) {
            if (folderIterateFunc) {
                // Check if the folder should be copied
                if (insideDirectory(sourcePTWYcan)) {
                    if (stat(targetPTWYcan, &targetStat) == -1 && mkdir(targetPTWYcan, 0700) != 0) {
                        perror("Couldn't create directory in target");
                        continue;
                    } else {
                        if (verbFunc) {
                            printf("Directory %s created successfully.\n", targetPTWYcan);
                        }
                    }
                    syncing(sourcePTWYcan, targetPTWYcan); // -r sync the directories
                } else {
                    if (verbFunc) {
                        printf("Skipping directory %s bc it only contains unwanted files.\n", sourcePTWYcan);
                    }
                }
            }
        } else {
            if (fileNameCan(sourceCan->d_name)) {
                if (stat(targetPTWYcan, &targetStat) != 0 || difftime(sourceStat.st_mtime, targetStat.st_mtime) > 0) {
                    copyFile(sourcePTWYcan, targetPTWYcan);
                    if (verbFunc) {
                        printf("Copied file from %s to %s.\n", sourcePTWYcan, targetPTWYcan);
                    }
                }
            }
        }
    }

    closedir(sourceFolderPTWY);
}


void syncing(const char *firstFolder, const char *secondFolder) {
    processDirectory(firstFolder, secondFolder);
    processDirectory(secondFolder, firstFolder);
}


int main(int argc, char *argv[]) {
    int optionz;

    // Parse command-line options
    while ((optionz = getopt(argc, argv, "avnpro:i:")) != -1) {
        switch (optionz) {
        case 'a':
            allFunc = true;
            break;
        case 'v':
            verbFunc = true;
            break;
        case 'n':
            noChangeFunc = true;
            break;
        case 'i':
            ignoreFunc[ignoreCount++] = glob2regex(optarg);
            break;
        case 'o':
            onlyFunc[onlyCount++] = glob2regex(optarg);
            break;
        case 'r':
            folderIterateFunc = true;
            break;
        case 'p':
            permSame = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-a] [-v] [-n] [-p] [-r] [-i 'pattern'] [-o 'pattern'] dir1 [dir2 dir3 dir4]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (argc - optind < 2 || argc - optind > 4) {
        fprintf(stderr, "Minimum 2 directories, maximum 4 directories\n");
        exit(EXIT_FAILURE);
    }

    // Loop over all combinations of directories
    for (int i = optind; i < argc; i++) {
        for (int j = i + 1; j < argc; j++) {
            syncing(argv[i], argv[j]);
            syncing(argv[j], argv[i]);
        }
    }
    for (int i = 0; i < ignoreCount; i++) {
        free(ignoreFunc[i]);
    }
    for (int i = 0; i < onlyCount; i++) {
        free(onlyFunc[i]);
    }

    return 0;
}