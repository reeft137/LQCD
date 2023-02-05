/**
 * @file data_io.h
 * @author TC (reeft137@gmail.com)
 * @brief Read and write data (binary files and txt files). 
 *        Provide 3 functions: 
 *        void read_bin(): Read data from binary file; 
 *        void write_bin(): Write data to binary file; 
 *        void write_txt(): Write data to txt file. 
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

// Custom libraries
#include "misc.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// Read from binary file
void read_bin(const char *fname, int maxline, double *data);

void read_bin(const char *fname, int maxline, COMPLEX *data);

void read_bin(const char *fname, int maxline, VARRAY_DOUBLE &data);

void read_bin(const char *fname, int maxline, VARRAY_COMPLEX &data);

// Write to binary file
void write_bin(const char *fname, int maxline, const double *data);

void write_bin(const char *fname, int maxline, const COMPLEX *data);

void write_bin(const char *fname, int maxline, const VARRAY_DOUBLE &data);

void write_bin(const char *fname, int maxline, const VARRAY_COMPLEX &data);

// Write to txt file
void write_txt(const char *fname, int maxline, const COMPLEX *data);

void write_txt(const char *fname, int maxline, const VARRAY_COMPLEX &data);

#endif
