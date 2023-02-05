/**
 * @file nbsmisc.h
 * @author TC (reeft137@gmail.com)
 * @brief A1+ projection, Laplacian, data adding,...
 * @version 0.1
 * @date 2023-02-05
 * 
 */

#ifndef IS_INCLUDED_NBSMISC_H
#define IS_INCLUDED_NBSMISC_H

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"
#include "misc.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

void a1_plus(const char *datalist[], int maxline, int N, char *r_datalist[]);

#endif