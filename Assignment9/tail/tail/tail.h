//
//  tail.h
//  tail
//
//  Created by Tom Nuss on 2018-03-30.
//  Copyright © 2018 Tomas Gonzalez. All rights reserved.
//

#ifndef tail_h
#define tail_h

int getLine(char *line, int lim);
char *alloc(int n);
int readLines(char *lineptr[], int maxlines);
void writeLines(char *lineptr[], int sline, int nlines)
#endif /* tail_h */
