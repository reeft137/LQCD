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
#include <math.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

char **jackknife_sample(char *datalist[], int maxline, int N, char *r_datalist[]); 

void jackknife_average(char *datalist[], int maxline, int N, const char *ofname);

# endif