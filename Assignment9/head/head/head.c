//
//  main.c
//  head
//
//  Created by Tom Nuss on 2018-03-29.
//  Copyright © 2018 Tomas Gonzalez. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    if(argc != 2) {
        exit(EXIT_FAILURE);
    }
    int linesNumber = atoi(argv[1]);
    char* line = NULL;
    size_t len = 0;
    int number = 0;
    while(getline(&line, &len, stdin) != -1 && number < linesNumber) {
        printf("%s", line);
        number++;
    }
    return 0;
}
