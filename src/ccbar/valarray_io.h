/**
 * @file valarray_io.h
 * @author TC (reeft137@gmail.com)
 * @brief Read and write valarrays (binary files)
 * @version 0.1
 * @date 2023-01-17
 *
 */

#ifndef IS_INCLUDED_VALARRAY_IO_H
#define IS_INCLUDED_VALARRAY_IO_H

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

extern VARRAY_DOUBLE read_double_varray(const char *fname, int maxline);

extern VARRAY_COMPLEX read_complex_varray(const char *fname, int maxline);

extern void write_double_varray(const char *ifname, const char *prefix, int maxline, const VARRAY_DOUBLE &valdata);

extern void write_complex_varray(const char *fname, const char *prefix, int maxline, const VARRAY_COMPLEX &valdata);

#endif
