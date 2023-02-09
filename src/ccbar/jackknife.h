//---------------------------------------------------------------------
/**
 * @file jackknife.h
 * @author TC (reeft137@gmail.com)
 * @brief Provide functions of jackknife sample and average. 
 *        Also, these functions will provide the file names 
 *        of the sampled data. 
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

void jackknife_resample(char *datalist[], char *result_datalist[], int maxline, int N); 

void jackknife_average(char *datalist[], const char *result_fname, int maxline, int N);

# endif