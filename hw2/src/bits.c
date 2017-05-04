/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
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
    unsigned exponent = uf >> 23 & 0xff;
    unsigned fraction = uf & 0x7fffff;
    unsigned tmin = 0x1 << 31;
    // if exponent is all ones, and the fraction is non zero,
    // then it's a NaN.
    if ((exponent==0xff) && (!!fraction)) {
        return uf;
    }

    return uf^tmin;
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
	// maximum e
	int e = 158;

	// sign bit bask
    int mask = 1 << 31;

	// if x is empty, return now.
	if (!x) {
		return 0;
	}

	// if x is tmin, return the same as float
    if (x == mask) {
        return mask | (e << 23);
	}

	int sign = x & mask;
    if (sign) {
        x = ~x + 1;
	}

	// calculate e
    while (!(x & mask)) {
        x = x << 1;
        e = e - 1;
    }

    int frac = (x & ~mask) >> 8;
    if (x & 0x80 && ((x & 0x7f) > 0 || frac & 1)) {
        frac++;
	}

    return sign + (e << 23) + frac;
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
    unsigned exponent = uf >> 23 & 0xff;
    unsigned fraction = uf & 0x7fffff;
    unsigned sign_bit = uf & (1 << 31);

    // either infinity, NaN or just can't increase the number anymore
    if (exponent == 255) {
        return uf;
    }

    // number is zero, 0 * 2 is still 0...
    if (exponent == 0 && fraction == 0) {
        return uf;
    }

    // increase the exponent (i.e: multiply by two)
    if (exponent) {
        exponent++;
    } else if (fraction == 0x7fffff) {
        // fraction is full, exponent is empty
        fraction--;
        exponent++;
    } else {
        // fraction is not full and the exponent is not full,
        // so increase the fraction part
        fraction <<= 1;
    }

    return sign_bit | (exponent << 23) | fraction;

}
/*
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
    unsigned sign = uf & (0x80000000);
    unsigned E = uf >> 23 & 0xff;
    unsigned frac = uf & 0x7fffff;
	unsigned frac_size = 23;

    if(E == 0xff) {
        return uf;
	}

	if (E > 1) {
        return sign | (E-1) << frac_size | frac;
	}

	if (E == 1) {
		frac |= 1 << frac_size;
	}

	if ((frac & 3) == 3) {
		frac++;
	}

	frac >>= 1;
	return sign | frac;
}
/*
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
  int exponent = (uf >> 23) & 0xff;
  int E = exponent - 127;

  if(exponent == 0x7f800000) {
    return 1 << 31;
  }

  // if exponent is empty (only a fraction) or zero -> return 0
  if(!exponent || E < 0) {
    return 0;
  }

  // if biased exponent is biggest, return
  if(E > 30) {
    return 1 << 31;
  }

  int frac = (uf & 0x7fffff) | 0x800000;
  frac = E >= 23 ? frac << (E - 23) : frac >> (23 - E);

  if((uf >> 31) & 1) {
    return ~frac + 1;
  }

  return frac;
}
/*
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {

	// clear msb (sign bit), don't change anything else
	unsigned exponent = uf >> 23 & 0xff;
	unsigned fraction = uf & 0x7fffff;

	// if exponent is all ones, and the fraction is non zero, then it's a nan.
	if ((exponent==0xff) && (!!fraction)) {
		return uf;
	}

	unsigned mask = 0x7fffffff;
	return uf & mask;
}
