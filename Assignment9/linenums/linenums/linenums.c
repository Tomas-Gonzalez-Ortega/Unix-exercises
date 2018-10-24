//
//  main.c
//  linenums
//
//  Created by Tom Nuss on 2018-03-29.
//  Copyright © 2018 Tomas Gonzalez. All rights reserved.
//

#include <stdio.h>
#include "linenums.h"

int main(int argc, const char * argv[]) {
    char* line = NULL;
    size_t len = 0;
    int count = 1;
    while(getline(&line, &len, stdin) != -1) {
        printf("%d:%s", count, line);
        count++;
    }
    return 0;
}
