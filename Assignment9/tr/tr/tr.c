//
//  main.c
//  tr
//
//  Created by Tom Nuss on 2018-03-30.
//  Copyright © 2018 Tomas Gonzalez. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "tr.h"
//#define MAX_SIZE 100 // Max string size

#include <stdio.h>

int main(int argc, char** argv) {
    
    if(argc != 4) {
        exit(EXIT_FAILURE);
    }
    
    //open the file for reading and updating
    FILE* word = fopen(argv[3], "r+");

    if (word == NULL) {
        exit(EXIT_FAILURE);
    }
    
    char index = 0;
    char original = argv[1][0];
    char wanted = argv[2][0];
    
    while((index = fgetc(word)) != EOF) {
        if (index == original) {
            //set the position of the stream one character back
            fseek(word, ftell(word) - 1, SEEK_SET);
            /* done by getting the current position using ftell, subtracting one from it and using fseek to set a new position */
            //write the new character at the new position
            fprintf(word, "%c", wanted);
        }
    }
    fclose(word); // preventing memory leaks
    
    return 0;
}

/*

void tr(char original, char wanted, char * word);

int main() {
    
    char original, wanted, word[MAX_SIZE];
    original = getchar();
    wanted = getchar();
    gets(word);
    tr(original, wanted, word);
    printf("%s", word);
    
    return 0;
}

void tr(char original, char wanted, char * word) {
    int i = 0;
    while(word[i] != '\0') {
        if(word[i] == original) {
            word[i] = wanted;
        }
        i++;
    }
}
*/
