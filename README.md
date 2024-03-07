# C-File-Synchronization-Program
Project was last updated on: 20 October 2023

**This was a university project for a basic java object oriented programming unit/module.**

This was a university project for a systems programming unit that involved using the C programming language.

## Project Overview

So I'm just going to give a quick summary of what this project is about, but full instructions can be found [here]([here](https://teaching.csse.uwa.edu.au/units/CITS2002/past-projects/p2023-2/summary.php)).

This project involved the development of a command-line utility program designed for the synchronization of files across multiple directories. which offered various synchronization options, including specific directory synchronization, exclusion of files with particular extensions, and the ability to output only the intended changes and many more. 

The implementation incorporated essential programming functions and libraries such as perror, getopt, malloc, realloc, strdup, opendir, readdir, closedir, stat, mkdir, open, read, write, close, chmod, utime, as well as external utilities like glob2regex, regcomp, and regexec.

There are multiple files to better organize the tasks into distinct and independent sets where the main() function is contained within the mysync.c file, while other functionalities are distributed across various files, such as header.h and the other c files.
