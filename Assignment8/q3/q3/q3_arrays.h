//
//  q3_arrays.h
//  q3
//
//  Created by Tom Nuss on 2018-03-14.
//  Copyright © 2018 Tom Nuss. All rights reserved.
//

// "q3_arrays.h" //

#ifndef __q3_arrays_h__
#define __q3_arrays_h__

/*
 NOTE: you are not allowed to use subscripts or indices for any of these questions except for sum_pi(int* a, int len).
 No: x[i], x+i
 OK: *x, x++, ++x, x+1
 */

/** Finds the sum of an array of integers.
 @param[in] a the beginning of the array
 @param[in] len the length of the array (number of elements)
 @returns the sum of the numbers beginning at a for the specified number of elements
 @test x[3] = { 1, 2, 3 }; assert( sum_pi(x, 3) == 6 );
 @test x[3] = { 1, 2, 3 }; assert( sum_pi(x, 2) == 3 );
 @test x[3] = { 1, 2, 3 }; assert( sum_pi(x, 1) == 1 );
 @remark ~ 5 lines
 */
int sum_pi(int* a, int len);

/** Finds the sum of an array of integers.
 @param[in] start the beginning of the array (inclusive)
 @param[in] end end of the array (exclusive)
 @returns the sum of the numbers beginning at a for the specified number of elements
 @test int x[3] = { 1, 2, 3 }; assert( sum_pp(x, x+3) == 6 );
 @test int x[3] = { 1, 2, 3 }; assert( sum_pp(x, x+2) == 3 );
 @test int x[3] = { 1, 2, 3 }; assert( sum_pp(x, x+1) == 1 );
 @remark No cheating! Use the pointers as pointers. DO NOT compute the length and then use sum_pi.
 @remark ~ 5 lines
 */
int sum_pp(int* start, int* end);

/** Finds the min and max elements of an array.
 @param[in] start the beginning of the array (inclusive)
 @param[in] end end of the array (exclusive)
 @param[out] min pointer to the minimum element
 @param[out] max pointer to the maximum element
 @test int *min, *max;
 int x[3] = { 10, 20, 30 };
 minmax(x, x+3, &min, &max);
 assert(min == x); assert(max == x+2);
 @test int *min, *max;
 int x[10] = { 54, 65, 9, 73, 22, 88, 71, 18, 65, 56 };
 minmax(x, x+10, &min, &max);
 assert(min == x+2); assert(max == x+5);
 @remark ~ 8 lines
 */
void minmax(int* start, int* end, int** min, int** max);

/** Swaps two integers.
 @param[in/out] a pointer to the 1st int to swap
 @param[in/out] b pointer to the 2nd int to swap
 @test int x = 5, y = 7;
 swap_int(&x, &y);
 assert(x == 7); assert(y == 5);
 @remark ~ 3 lines
 */
void swap_int(int* a, int* b);

/** Swaps two pointers.
 @param[in/out] a pointer to the 1st pointer to swap
 @param[in/out] b pointer to the 2nd pointer to swap
 @test
 int   x = 5 ,    y = 7;
 int *px = &x, *py = &y;
 swap_intStar(&px, &py);
 assert( px == &y); assert( py == &x);
 assert(*px == 7 ); assert(*py == 5 );
 assert(  x == 5 ); assert(  y == 7 );
 @remark ~ 3 lines
 */
void swap_intStar(int** a, int** b);

/** Reverses the scalars in an array of pointers to scalars.
 @param[in] a the start of the array (inclusive)
 @param[in] b the end of the array (exclusive)
 @test
 int x[10] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };
 int z[10]; for(int i = 0; i < 10; i++) { z[i] = x[i]; } // copy of x
 int *px[10]; for(int i = 0; i < 10; i++) { px[i] = &x[i]; } // pointers to values of x
 reverse_values(px, px+10);
 for(int i = 0; i < 10; i++) { assert( x[i] == z[10-1-i]) ; } // values reversed
 for(int i = 0; i < 10; i++) { assert( px[i] = &x[i] ); } // pointers unchanged
 @remark ~ 4 lines
 */
void reverse_values(int** start, int** end);

/** Reverses the pointers in an array of pointers to scalars.
 @param[in] start the start of the array (inclusive)
 @param[in] end the end of the array (exclusive)
 @test
 int x[10] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };
 int z[10]; for(int i = 0; i < 10; i++) { z[i] = x[i]; } // copy of x
 int *px[10]; for(int i = 0; i < 10; i++) { px[i] = &x[i]; } // pointers to values of x
 reverse_intStar(px, px+10);
 for(int i = 0; i < 10; i++) { assert( x[i] == z[i] ); } // values unchanged
 for(int i = 0; i < 10; i++) { assert( px[i] = &x[10-1-i] ); } // pointers reversed
 @remark ~ 4 lines
 */
void reverse_intStar(int** start, int** end);

/** Returns the number of values between the two pointers.
 @param[in] a the start of the array (inclusive)
 @param[in] b the end of the array (exclusive)
 @return the number of values between the two pointers
 @test
 int x[10];
 assert( count(x, x+10) == 10 );
 @remark 1 line. No loop.
 @hint Think about the types of:  int* + int <-> int*
 */
int count(int* a, int* b);

#endif //__q3_arrays_h__

