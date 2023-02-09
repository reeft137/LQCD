/**
 * @file data_io.cc
 * @author TC (reeft137@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-17
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <valarray>
#include "data_io.h"
#include "misc.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

void read_bin(const char *fname, int maxline, double *data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(fname, "rb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fread(data, sizeof(double), maxline, fp);
  fclose(fp);
}

void read_bin(const char *fname, int maxline, COMPLEX *data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(fname, "rb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fread(data, sizeof(COMPLEX), maxline, fp);
  fclose(fp);
}

void read_bin(const char *fname, int maxline, VARRAY_DOUBLE &data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(fname, "rb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fread(&data[0], sizeof(double), maxline, fp);
  fclose(fp);
}

void read_bin(const char *fname, int maxline, VARRAY_COMPLEX &data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(fname, "rb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fread(&data[0], sizeof(COMPLEX), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const double *data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Writing data to file '%s'... \n", pname);

  FILE *fp = fopen(fname, "wb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fwrite(data, sizeof(double), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const COMPLEX *data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Writing data to file '%s'... \n", pname);

  FILE *fp = fopen(fname, "wb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fwrite(data, sizeof(COMPLEX), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const VARRAY_DOUBLE &data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Writing data to file '%s'... \n", pname);

  FILE *fp = fopen(fname, "wb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fwrite(&data[0], sizeof(double), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const VARRAY_COMPLEX &data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Writing data to file '%s'... \n", pname);

  FILE *fp = fopen(fname, "wb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fwrite(&data[0], sizeof(COMPLEX), maxline, fp);
  fclose(fp);
}
