//
//  main.c
//  rev
//
//  Created by Tom Nuss on 2018-03-30.
//  Copyright © 2018 Tomas Gonzalez. All rights reserved.
//

#include<stdio.h>
#include <stdlib.h>
#include<string.h>

int main(int argc, char *argv[]) {
    
    char temp;
    char *word;
    int firstChar, lastChar;
    
    if(argc != 2) {
        exit(EXIT_FAILURE);
    }
    
    word = argv[2]; // the word to be reversed
    firstChar = 0; // Initialize at start
    lastChar = strlen(word) - 1; // Initialize at the end of string length
    
    /* Reverse characters from start to the end */
    while (firstChar < lastChar) {
        temp = word[firstChar];
        word[firstChar] = word[lastChar];
        word[lastChar] = temp;
        firstChar++;
        lastChar--;
    }

    printf("%s", word);
    free(word);
    return 0;
}
