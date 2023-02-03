/**
 * @file data_io.h
 * @author TC (reeft137@gmail.com)
 * @brief Read and write valarrays (binary files and txt files)
 * @version 0.1
 * @date 2023-01-17
 *
 */

#ifndef IS_INCLUDED_DATA_IO_H
#define IS_INCLUDED_DATA_IO_H

// C libraries
#include <stdio.h>
#include <stdlib.h>

// C++ libraries
#include <complex>
#include <valarray>

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

VARRAY_DOUBLE read_double_varray(const char *fname, int maxline);

VARRAY_COMPLEX read_complex_varray(const char *fname, int maxline);

void write_double_varray(const char *fname, int maxline, const VARRAY_DOUBLE &valdata);

void write_complex_varray(const char *fname, int maxline, const VARRAY_COMPLEX &valdata);

void write_txt(const char *fname, int maxline, const VARRAY_COMPLEX &valdata);

#endif
