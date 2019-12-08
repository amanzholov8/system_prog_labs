
/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here> Alim Amanzholov (cs20160845)
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
   //(x and y) == not ((not x) or (not y)) 
   return ~((~x) | (~y));
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  //n bytes to bits (multiply by 8=2^3 i.e. shift left by 3)
  int nbits;
  nbits = (n<<3);
  //shift x by nbits to the right, then & with 11111111 (to get only one byte)
  return (x>>nbits) & 0xff;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  //to change arithmetic to logical shift we need to make first n shifted bits 0 
  int nzeros = ~(((1<<31)>>n)<<1);
  return (x>>n) & nzeros;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  //divide 32 bits into 8 pieces of 4 bits by using hide
  int hide = (0x11) | (0x11<<8) | (0x11<<16) | (0x11<<24);
  int count = 0;
  int count_total;
  //first, count number of 1s in least significant bit positions of each 4 bit piece
  count = count + (x & hide);
  //then, count number of 1s in 2 position of each 4 bit piece and so on
  count = count + ((x>>1) & hide);
  count = count + ((x>>2) & hide);
  count = count + ((x>>3) & hide);
  //finally, add up number of 1s of each 4 bit piece
  count_total = (count & 0xf) + ((count>>4) & 0xf) + ((count>>8) & 0xf) + ((count>>12) & 0xf) + ((count>>16) & 0xf) + ((count>>20) & 0xf) + ((count>>24) & 0xf) + ((count>>28) & 0xf);
  return count_total;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  // neg_x = -x, 
  int neg_x = (~x) + 1;
  //if x != 0, then (sign bit of x) | (sign bit of neg_x) will be 1 and its ~ will be 0; 
  //if x == 0, ~((sign bit of x) | (sign bit of neg_x)) = 1
  return ((~(x | neg_x)) >> 31) & 1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // tmin is 100...0 with 31 zeros, so we need to shift left 0001 by 31 bits
  return 1<<31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  //shift = 32 - n, the number of bits to the right of n least bits
  int shift = 32 + (~n) + 1; 
  //extend last n bits into 32 bits
  int new = (x << shift) >> shift;
  //diff = new - x, check if diff is 0, if not 0, then we cannot fit x into n bits
  int diff = new + (~x) + 1;
  return !diff; 
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  //bias = 2^n - 1 = (1<<n) - 1 = (1<<n) + (~0)
  //we only need bias when x is negative
  int bias;
  bias = ((1<<n) + (~0)) & (x>>31);
  return (x + bias)>>n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  //-x = ~x + 1
  return (~x) + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  //if x=0, !x=1 and result is 0; if x<0, !(x>>31)=0 and result is 0; if x>0, x>>31=0 and result is 1 
  return !(!x) & !(x>>31);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int neg_x = (~x) + 1;
  int diff = y + neg_x;
  //to get the sign of int, shift int to the right by 31 (=111...1 if int<0, = 000...0 if int>=0) and & with 1
  int sign_diff = (diff>>31) & 1;
  int sign_x = (x>>31) & 1;
  int sign_y = (y>>31) & 1;
  //y>=x either if sign_y=0 and sign_x=1 or if sign_y=sign_x and sign_diff=0
  return (sign_x & !sign_y) | (!sign_diff & !(sign_x ^ sign_y));
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  //declare variables
  int res;
  int is16;
  int is8;
  int is4;
  int is2;
  int is1;
  //check if there is 1 in first 16 bits from left, if there is res will be > 16
  is16 = !(!(x >> 16)) << 4;
  res = is16;
  //then check if there is 1 in next 8 bits, if there is res will be > 8 + prev res
  is8 = !(!(x >> (8+res))) << 3;
  res += is8;
  //and so on
  is4 = !(!(x >> (4+res))) << 2;
  res += is4;
  is2 = !(!(x >> (2+res))) << 1;
  res += is2;
  is1 = !(!(x >> (1+res)));
  res += is1;
  return res;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  //check if uf is infinity or NaN
  if (uf == 0xffc00000 || uf == 0x7fc00000) return uf;
  //otherwise, set the sign bit of uf to 1
  return uf ^ (1<<31);
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  //we need to set sign bit, fraction and exponent
  unsigned s = 0;
  unsigned f = 0; 
  unsigned e = 127; //bias=127
  unsigned carry = 0;
  if (x==0) return x;
  //if x is negative, we need to change s to 1
  if (x & 0x80000000) {
    s = 1;
    x = -x;
  }
  e += 31;
  while (!(x & 0x80000000)) {
    x = x<<1;
    e -= 1;
  }
  //we need to transfer last 31 bits of x to f and shift by 8 (since f is 23 bits)
  f = (x & (~(1<<31))) >> 8;
  //x might not fit into f, in that case we need to set carry to 1 and add it to f
  if ((x & 0xff) > 0x80) carry = 1;
  else if ((x & 0x000001ff) == 0x180) carry = 1;
  f += carry;
  return ((s<<31) + (e<<23)) + f;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  //we need to get sign bit(s), exponent(e) and fraction(f) of giving floating point number
  unsigned s = (uf>>31) & 1;
  unsigned e = (uf>>23) & 0xff;
  unsigned f = uf & ~((1<<31)>>8);
  //check if given argument is NaN or infinity
  if (e == 0xff) return uf;
  //if e is maximum, multiplying argument by 2 will overflow (i.e. infinity)
  if (e == 0xff - 1) {
    e = 0xff;
    f = 0;
  // if argument is denormalized (i.e e=0), just shift f left by 1 (i.e mult by 2)
  } else if (e==0) f = f << 1;
  //else - when argument is normalized, just increase exponent by 1
  else e = e + 1;
  //combine sign, expoenent and fraction back
  return ((s<<31) | (e<<23)) | f;
}
