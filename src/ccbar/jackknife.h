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
#include <math.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

void jackknife_resample(char *datalist[], char *r_datalist[], int maxline, int file_total); 

void jackknife_average(char *datalist[], const char *r_fname, int maxline, int file_total);

# endif