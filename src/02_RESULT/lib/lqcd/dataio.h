/**
 * @file data_io.h
 * @author TC (reeft137@gmail.com)
 * @brief Read and write data (binary files).
 *        Provide 2 functions:
 *        void read_bin(): Read data from binary file;
 *        void write_bin(): Write data to binary file;
 * @version 0.1
 * @date 2023-01-17
 *
 */

#ifndef IS_INCLUDED_DATAIO_H
#define IS_INCLUDED_DATAIO_H

// Custom libraries
#include "misc.h"
#include "typealias.h"

/**
 * @brief Read from binary file
 *
 * @param datafile Input file name of the data file
 * @param maxline Total of double/complex numbers
 * @param data The pointer of the array that contains the data read
 */
void read_bin(const char *datafile, int maxline, DOUBLE *data);
void read_bin(const char *datafile, int maxline, COMPLX *data);
void read_bin(const char *datafile, int maxline, DVARRAY &data);
void read_bin(const char *datafile, int maxline, CVARRAY &data);

/**
 * @brief Write to binary file
 *
 * @param datafile Output file name of the data file
 * @param maxline Total of double/complex numbers
 * @param data The pointer of the array that contains the data to be written to file
 */
void write_bin(const char *datafile, int maxline, const DOUBLE *data);
void write_bin(const char *datafile, int maxline, const COMPLX *data);
void write_bin(const char *datafile, int maxline, const DVARRAY &data);
void write_bin(const char *datafile, int maxline, const CVARRAY &data);

/**
 * @brief Convert binary file to txt file (only for complex data)
 *
 * @param namebin File name of the binary file
 * @param nametxt File name of the txt file
 * @param maxline Total of complex numbers
 */
void bin2txt(const char *namebin, const char *nametxt, int maxline);

/**
 * @brief Keep the real part of the data
 *
 * @param data original data (complex valarray)
 * @param rdata real part of the valarray (double valarray)
 * @param maxline
 */
void keep_real(CVARRAY &data, DVARRAY &realdata, int maxline);

/**
 * @brief Keep the imaginary part of the data
 *
 * @param data original data (complex valarray)
 * @param rdata imaginary part of the valarray (double valarray)
 * @param maxline
 */
void keep_imag(CVARRAY &data, DVARRAY &imagdata, int maxline);

#endif
