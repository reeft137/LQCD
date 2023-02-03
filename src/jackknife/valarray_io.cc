/**
 * @file data_io.cc
 * @author TC (reeft137@gmail.com)
 * @brief Read and write valarrays (binary files)
 * @version 0.1
 * @date 2023-01-17
 *
 */

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// Inline functions
/**
 * @brief Print the origin name of the input file
 * 
 * @param fname 
 * @return char* 
 */
inline char *ifname(const char *fname)
{
  char stmp[2048], dir[2048], base[2048], ifname[2048];
  strncpy(stmp, fname, 2047);
  strncpy(dir, dirname(stmp), 2047);
  strncpy(base, basename(stmp), 2047);
  sprintf(ifname, "%s/%s", dir, base);
  fprintf(stderr, "Reading file: %s... \n", ifname);
  return ifname;
}

/**
 * @brief Create the output file name (add the prefix) and print it 
 * 
 * @param fname 
 * @param prefix 
 * @return char* 
 */
inline char *ofname(const char *fname, const char *prefix)
{
  char stmp[2048], dir[2048], base[2048], ofname[2048];
  strncpy(stmp, fname, 2047);
  strncpy(dir, dirname(stmp), 2047);
  strncpy(base, basename(stmp), 2047);
  sprintf(ofname, "%s/%s.%s", dir, prefix, base);
  fprintf(stderr, "generating %s\n", ofname);
  return ofname;
}

/**
 * @brief Open file with a status check
 * 
 * @param fname 
 * @param type 
 * @return FILE* 
 */
inline FILE *openfile(const char *fname, const char *type)
{
  if (fopen(fname, type) == NULL)
  {
    perror(fname);
    exit(1);
  }
  else
  {
    return fopen(fname, type);
  }
}

// Main body
VARRAY_DOUBLE read_double_varray(const char *fname, int maxline)
{
  FILE *fp = openfile(ifname(fname), "rb");
  VARRAY_DOUBLE data(maxline);
  data = 0.0;
  fread(&data[0], sizeof(double), maxline, fp); // Use "fread" to read data from binary file
  fclose(fp);

  return data;
}

VARRAY_COMPLEX read_complex_varray(const char *fname, int maxline)
{
  FILE *fp = openfile(ifname(fname), "rb");
  VARRAY_COMPLEX data(maxline);
  data = 0.0;
  fread(&data[0], sizeof(COMPLEX), maxline, fp); // Use "fread" to read data
  fclose(fp);

  return data;
}

void write_double_varray(const char *fname, const char *prefix, int maxline, const VARRAY_DOUBLE &valdata)
{
  FILE *fp = openfile(ofname(fname, prefix), "wb");
  fwrite(&valdata[0], sizeof(double), maxline, fp);
  fclose(fp);
}

void write_complex_varray(const char *fname, const char *prefix, int maxline, const VARRAY_COMPLEX &valdata)
{
  FILE *fp = openfile(ofname(fname, prefix), "wb");
  fwrite(&valdata[0], sizeof(COMPLEX), maxline, fp);
  fclose(fp);
}
