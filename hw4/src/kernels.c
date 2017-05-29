/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Please fill in the following team struct
 */
team_t team = {
    "311121289+201285509",                           /* Team name */
    "Afik Bar", "afikbar@campus.technion.ac.il",     /* First Member */
    "Oded Lazar", "oded.lazar@campus.technion.ac.il" /* Second Member */
};

/***************
 * SMOOTH KERNEL
 **************/
/* ... Register additional test functions here */
/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
  int red;
  int green;
  int blue;
  int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum) {
  sum->red = sum->green = sum->blue = 0;
  sum->num = 0;
  return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p) {
  sum->red += (int)p.red;
  sum->green += (int)p.green;
  sum->blue += (int)p.blue;
  sum->num++;
  return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) {
  current_pixel->red = (unsigned short)(sum.red / sum.num);
  current_pixel->green = (unsigned short)(sum.green / sum.num);
  current_pixel->blue = (unsigned short)(sum.blue / sum.num);
  return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src) {
  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;

  initialize_pixel_sum(&sum);
  for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
    for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
      accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

  assign_sum_to_pixel(&current_pixel, sum);
  return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

inline void calculate_top_left_corner(int dim, pixel *src, pixel *dst) {
  dst[0].red = (src[0].red + src[1].red + src[dim].red + src[dim + 1].red) / 4;
  dst[0].blue = (src[0].blue + src[1].blue + src[dim].blue + src[dim + 1].blue) / 4;
  dst[0].green = (src[0].green + src[1].green + src[dim].green + src[dim + 1].green) / 4;
}

inline void calculate_top_row(int dim, pixel *src, pixel *dst) {
  for (int i = 1; i < dim - 1; i++) {
    dst[i].red = (src[i].red + src[i - 1].red + src[i + 1].red + src[dim + i - 1].red +
                  src[dim + i].red + src[dim + i + 1].red) /
                 6;
    dst[i].blue = (src[i].blue + src[i - 1].blue + src[i + 1].blue + src[dim + i - 1].blue +
                   src[dim + i].blue + src[dim + i + 1].blue) /
                  6;
    dst[i].green = (src[i].green + src[i - 1].green + src[i + 1].green + src[dim + i - 1].green +
                    src[dim + i].green + src[dim + i + 1].green) /
                   6;
  }
}
inline void calculate_top_right_corner(int dim, pixel *src, pixel *dst) {

  dst[dim - 1].red =
      (src[dim - 1].red + src[dim - 2].red + src[dim + dim - 2].red + src[dim + dim - 1].red) / 4;
  dst[dim - 1].blue =
      (src[dim - 1].blue + src[dim - 2].blue + src[dim + dim - 2].blue + src[dim + dim - 1].blue) /
      4;
  dst[dim - 1].green = (src[dim - 1].green + src[dim - 2].green + src[dim + dim - 2].green +
                        src[dim + dim - 1].green) /
                       4;
}
inline void calculate_left_column(int dim, pixel *src, pixel *dst) {
  for (int i = 1, j = dim; i < dim - 1; i++, j = dim * i) {
    dst[j].red = (src[j].red + src[j - dim].red + src[j - dim + 1].red + src[j + 1].red +
                  src[j + dim].red + src[j + dim + 1].red) /
                 6;
    dst[j].blue = (src[j].blue + src[j - dim].blue + src[j - dim + 1].blue + src[j + 1].blue +
                   src[j + dim].blue + src[j + dim + 1].blue) /
                  6;
    dst[j].green = (src[j].green + src[j - dim].green + src[j - dim + 1].green + src[j + 1].green +
                    src[j + dim].green + src[j + dim + 1].green) /
                   6;
  }
}

inline void calculate_right_column(int dim, pixel *src, pixel *dst) {
  for (int i = 1, j = dim + dim - 1; i < dim - 1; i++, j = dim * i + dim - 1) {
    dst[j].red = (src[j].red + src[j - dim].red + src[j - dim - 1].red + src[j - 1].red +
                  src[j + dim].red + src[j + dim - 1].red) /
                 6;
    dst[j].blue = (src[j].blue + src[j - dim].blue + src[j - dim - 1].blue + src[j - 1].blue +
                   src[j + dim].blue + src[j + dim - 1].blue) /
                  6;
    dst[j].green = (src[j].green + src[j - dim].green + src[j - dim - 1].green + src[j - 1].green +
                    src[j + dim].green + src[j + dim - 1].green) /
                   6;
  }
}

inline void calculate_bottom_left_corner(int dim, int j, pixel *src, pixel *dst) {
  dst[j].red = (src[j].red + src[j - dim].red + src[j - dim + 1].red + src[j + 1].red) / 4;
  dst[j].blue = (src[j].blue + src[j - dim].blue + src[j - dim + 1].blue + src[j + 1].blue) / 4;
  dst[j].green =
      (src[j].green + src[j - dim].green + src[j - dim + 1].green + src[j + 1].green) / 4;
}

inline void calculate_bottom_row(int dim, int j, pixel *src, pixel *dst) {
  for (int i = 1; i < dim - 1; i++) {
    dst[j + i].red = (src[j + i].red + src[j + i - 1].red + src[j + i - dim - 1].red +
                      src[j + i - dim].red + src[j + i - dim + 1].red + src[j + i + 1].red) /
                     6;
    dst[j + i].blue = (src[j + i].blue + src[j + i - 1].blue + src[j + i - dim - 1].blue +
                       src[j + i - dim].blue + src[j + i - dim + 1].blue + src[j + i + 1].blue) /
                      6;
    dst[j + i].green =
        (src[j + i].green + src[j + i - 1].green + src[j + i - dim - 1].green +
         src[j + i - dim].green + src[j + i - dim + 1].green + src[j + i + 1].green) /
        6;
  }
}

inline void calculate_bottom_right_corner(int dim, int j, pixel *src, pixel *dst) {
  dst[j].red = (src[j].red + src[j - dim].red + src[j - dim - 1].red + src[j - 1].red) / 4;
  dst[j].blue = (src[j].blue + src[j - dim].blue + src[j - dim - 1].blue + src[j - 1].blue) / 4;
  dst[j].green =
      (src[j].green + src[j - dim].green + src[j - dim - 1].green + src[j - 1].green) / 4;
}

inline void calculate_middle_block(int dim, pixel *src, pixel *dst) {
  for (int i = 1, row = dim; i < dim - 1; i++, row = i * dim) {

    for (int j = 1; j < dim - 1; j++) {
      dst[row + j].red =
          (src[row + j].red + src[row + j - 1].red + src[row + j + 1].red +
           src[row + j - dim - 1].red + src[row + j - dim].red + src[row + j - dim + 1].red +
           src[row + j + dim - 1].red + src[row + j + dim].red + src[row + j + dim + 1].red) /
          9;

      dst[row + j].blue =
          (src[row + j].blue + src[row + j - 1].blue + src[row + j + 1].blue +
           src[row + j - dim - 1].blue + src[row + j - dim].blue + src[row + j - dim + 1].blue +
           src[row + j + dim - 1].blue + src[row + j + dim].blue + src[row + j + dim + 1].blue) /
          9;

      dst[row + j].green =
          (src[row + j].green + src[row + j - 1].green + src[row + j + 1].green +
           src[row + j - dim - 1].green + src[row + j - dim].green + src[row + j - dim + 1].green +
           src[row + j + dim - 1].green + src[row + j + dim].green + src[row + j + dim + 1].green) /
          9;
    }
  }
}

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) {
  calculate_top_left_corner(dim, src, dst);
  calculate_top_row(dim, src, dst);
  calculate_top_right_corner(dim, src, dst);
  calculate_left_column(dim, src, dst);
  calculate_right_column(dim, src, dst);

  int j = dim * (dim - 1);
  calculate_bottom_left_corner(dim, j, src, dst);

  j = dim * (dim - 1);
  calculate_bottom_row(dim, j, src, dst);

  j = (dim + 1) * (dim - 1);
  calculate_bottom_right_corner(dim, j, src, dst);

  calculate_middle_block(dim, src, dst);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) { naive_smooth(dim, src, dst); }

/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions() {
  add_smooth_function(&smooth, smooth_descr);
  add_smooth_function(&naive_smooth, naive_smooth_descr);
}
