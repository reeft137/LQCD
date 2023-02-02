//---------------------------------------------------------------------
/**
 * @file jackknife.h
 * @author TC (reeft137@gmail.com)
 * @brief Provide functions of jackknife sample and average
 * @version 0.1
 * @date 2023-01-16
 *
 */
//---------------------------------------------------------------------

#ifndef IS_INCLUDED_JACKKNIFE_H
#define IS_INCLUDED_JACKKNIFE_H

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "valarray_io.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

void jackknife_sample(const char *datalist[], int maxline, int N); 

void jackknife_average(const char *datalist[], int maxline, int N);

# endif