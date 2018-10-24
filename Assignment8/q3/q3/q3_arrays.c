//  Tomas Gonzalez ID 100266942
//  q3_arrays.c
//  q3
//
//  Created by Tom Nuss on 2018-03-14.
//  Copyright © 2018 Tom Nuss. Langara College. All rights reserved.
//

#include "q3_arrays.h"
#include <stdlib.h>

int sum_pi(int* a, int len) {
    int i, sum = 0;
    for(i = 0; i < len; i++) {
        sum += a[i];
    }
    return sum;
}

int sum_pp(int* start, int* end) {
    int sum = 0;
    while (start < end) {
        sum += *start; // update sum
        start++; // point to the next
    }
    return sum;
}

void minmax(int* start, int* end, int** min, int** max) {
    
}

void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_intStar(int** a, int** b) {
    int *temp = *a;
    *a = *b;
    *b = temp;
}

void reverse_values(int** start, int** end) {

}

void reverse_intStar(int** start, int** end) {
    
}

int count(int* a, int* b) {
    return (int)(b - a);
}
