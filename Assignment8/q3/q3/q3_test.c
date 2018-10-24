#include <assert.h>
#include "q3_arrays.h"

/*
 
 gcc -g -Wall -std=gnu11 -o testArrays *.c && ./testArrays
 
 */

void test_sum_pi();
void test_sum_pp();
void test_minmax();
void test_swap_int();
void test_swap_intStar();
void test_reverse_values();
void test_reverse_intStar();
void test_count();

int main() {
    // TODO: fill in the tests
    //assert( false );
    test_sum_pi();
    test_sum_pp();
    test_minmax();
    test_swap_int();
    test_swap_intStar();
    test_reverse_values();
    test_reverse_intStar();
    test_count();
    return 0;
}

void test_sum_pi() {
    int x[3] = { 1, 2, 3 }; assert( sum_pi(x, 3) == 6 );
    int y[3] = { 1, 2, 3 }; assert( sum_pi(y, 2) == 3 );
    int z[3] = { 1, 2, 3 }; assert( sum_pi(z, 1) == 1 );
}

void test_sum_pp() {
    int x[3] = { 1, 2, 3 }; assert( sum_pp(x, x+3) == 6 );
    int y[3] = { 1, 2, 3 }; assert( sum_pp(y, x+2) == 3 );
    int z[3] = { 1, 2, 3 }; assert( sum_pp(z, x+1) == 1 );
}

void test_minmax() {
    int *min, *max;
    int x[3] = { 10, 20, 30 };
    minmax(x, x+3, &min, &max);
    assert(min == x); assert(max == x+2);

    int k[10] = { 54, 65, 9, 73, 22, 88, 71, 18, 65, 56 };
    minmax(k, k+10, &min, &max);
    assert(min == k+2); assert(max == k+5);
}

void test_swap_int() {
    int x = 5, y = 7;
    swap_int(&x, &y);
    assert(x == 7); assert(y == 5);
}

void test_swap_intStar() {
    int   x = 5 ,    y = 7;
    int *px = &x, *py = &y;
    swap_intStar(&px, &py);
    assert( px == &y); assert( py == &x);
    assert(*px == 7 ); assert(*py == 5 );
    assert(  x == 5 ); assert(  y == 7 );
}

void test_reverse_values() {
    int x[10] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };
    int z[10]; for(int i = 0; i < 10; i++) { z[i] = x[i]; } // copy of x
    int *px[10]; for(int i = 0; i < 10; i++) { px[i] = &x[i]; } // pointers to values of x
    reverse_values(px, px+10);
    for(int i = 0; i < 10; i++) { assert( x[i] == z[10-1-i]) ; } // values reversed
    for(int i = 0; i < 10; i++) { assert( px[i] = &x[i] ); } // pointers unchanged
}

void test_reverse_intStar() {
    int x[10] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };
    int z[10]; for(int i = 0; i < 10; i++) { z[i] = x[i]; } // copy of x
    int *px[10]; for(int i = 0; i < 10; i++) { px[i] = &x[i]; } // pointers to values of x
    reverse_intStar(px, px+10);
    for(int i = 0; i < 10; i++) { assert( x[i] == z[i] ); } // values unchanged
    for(int i = 0; i < 10; i++) { assert( px[i] = &x[10-1-i] ); } // pointers reversed
}

void test_count() {
    int x[10];
    assert( count(x, x+10) == 10 );
}

