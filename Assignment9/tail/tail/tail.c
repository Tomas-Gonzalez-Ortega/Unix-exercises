//
//  main.c
//  tail
//
//  Created by Tom Nuss on 2018-03-30.
//  Copyright © 2018 Tomas Gonzalez. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINES 10 // default line size
#define MAXLINES 50000 // max amount of lines that can be read from stdin
#define MAXLENGTH 1000 // max line length
#define ALLOCSIZE 10000000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;
char *lineptr[MAXLINES];

/** Modification of C's getline */
int getLine(char *line, int lim) {
    int c = 0, i;
    for(i = 0; i < lim && (c = getchar()) != EOF && c != '\n'; i++) {
        *(line++) = c;
    }
    if(c == '\n') {
        i++;
        *(line++) = c;
    }
    *line = '\0';
    return i;
}

char *alloc(int n) {
    if(allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else {
        return 0;
    }
}

/** Reads lines from stdin */
int readLines(char *lineptr[], int maxLines) {
    int len, nLines;
    char *p, line[MAXLENGTH];
    nLines = 0;
    while((len = getLine(line, MAXLENGTH)) > 0) {
        if(nLines >= maxLines || (p = alloc(len)) == NULL) {
            return -1;
        }
        else {
            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nLines++] = p;
        }
    }
    return nLines;
}

/** Writes lines to stdio */
void writeLines(char *lineptr[], int sLine, int nLines) {
    int i;
    if (sLine > 0) {
        i = sLine;
    }
    else {
        i = 0;
    }
    for (; i < nLines && lineptr[i] != NULL; i++) {
        printf("%s\n", lineptr[i]);
    }
}

/**
 * Get the number of lines to read from command line argument.
 * If no line length is specified, returns DEFAULT_LINES
 * Example command line input: tail.out -n 50
 */
int getNumLines(int argc, char *argv[]) {
    if(argc > 1) {
        if(argv[2] != NULL) {
            return atoi(argv[2]);
        }
        return DEFAULT_LINES;
    }
    else {
        return DEFAULT_LINES;
    }
}

/** argc = argument count, argv = argument vector */
int main(int argc, char *argv[]) {
    int nLines, linesToPrint;
    linesToPrint = getNumLines(argc, argv);
    
    if ((nLines = readLines(lineptr, MAXLINES)) >= 0) {
        writeLines(lineptr, nLines - linesToPrint, nLines);
        return 0;
    }
    else {
        nLines = MAXLINES;
        writeLines(lineptr, nLines - linesToPrint, nLines);
        printf("ERROR: buffer overflow.\n");
        return 1;
    }
}
