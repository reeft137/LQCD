/**
 * @file data_io.cc
 * @author TC (reeft137@gmail.com)
 * @brief Read and write valarrays (binary files and txt files)
 * @version 0.1
 * @date 2023-01-17
 *
 */

// C libraries
#include <stdio.h>
#include <stdlib.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"
#include "misc.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// Inline functions
// Open file with a status check
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
  char pname[2048]; 
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading file: '%s'... \n", pname);
  FILE *fp = openfile(fname, "rb");
  VARRAY_DOUBLE data(maxline);
  data = 0.0;
  fread(&data[0], sizeof(double), maxline, fp);
  fclose(fp);

  return data;
}

VARRAY_COMPLEX read_complex_varray(const char *fname, int maxline)
{
  char pname[2048]; 
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading file: '%s'... \n", pname);
  FILE *fp = openfile(fname, "rb");
  VARRAY_COMPLEX data(maxline);
  data = 0.0;
  fread(&data[0], sizeof(COMPLEX), maxline, fp);
  fclose(fp);

  return data;
}

void write_double_varray(const char *fname, int maxline, const VARRAY_DOUBLE &valdata)
{
  char pname[2048]; 
  gen_print_name(fname, pname);
  fprintf(stderr, "Generating file: %s... \n", pname);
  FILE *fp = openfile(fname, "wb");
  fwrite(&valdata[0], sizeof(double), maxline, fp);
  fclose(fp);
}

void write_complex_varray(const char *fname, int maxline, const VARRAY_COMPLEX &valdata)
{
  char pname[2048]; 
  gen_print_name(fname, pname);
  fprintf(stderr, "Generating file: %s... \n", pname);
  FILE *fp = openfile(fname, "wb");
  fwrite(&valdata[0], sizeof(COMPLEX), maxline, fp);
  fclose(fp);
}

void write_txt(const char *fname, int maxline, const VARRAY_COMPLEX &valdata)
{
  char pname[2048]; 
  gen_print_name(fname, pname);
  fprintf(stderr, "Generating file: %s... \n", pname);
  FILE *fp = openfile(fname, "w");
  for (int i = 0; i < maxline; i++)
  {
    fprintf(fp, "%02d %1.16e %1.16e\n", i, valdata[i].real(), valdata[i].imag());
  }
  fclose(fp);
}
