/**
 * @file valarray_io.cc
 * @author TC (reeft137@gmail.com)
 * @brief Read and write valarrays (binary files and txt files)
 * @version 0.1
 * @date 2023-01-17
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
// #include <complex.h>
#include <valarray>
#include "valarray_io.h"

// typedef double complex Complex;
typedef double _Complex Complex;
typedef std::valarray<double> VAL_double;
typedef std::valarray<Complex> VAL_complex;

VAL_double read_corr_double(const char *ifname, int maxrow)
{
  // Copy the
  char tmp[2048];
  char dir[2048];
  char base[2048];
  strncpy(tmp, ifname, 2047);
  strncpy(dir, dirname(tmp), 2047);
  strncpy(tmp, ifname, 2047);
  strncpy(base, basename(tmp), 2047);

  char print_ifname[2048];
  sprintf(print_ifname, "%s/%s", dir, base);
  fprintf(stderr, "reading %s\n", print_ifname); // print the input file name

  VAL_double data(maxrow);
  data = 0.0;

  FILE *ifp = NULL;
  ifp = fopen(ifname, "rb");
  if (ifp == NULL) // Deal with wrong file name
  {
    perror(ifname);
    exit(1);
  }

  fread(&data[0], sizeof(double), maxrow, ifp); // Use "fread" to read data
  fclose(ifp);

  return data;
}

VAL_complex read_corr_complex(const char *ifname, int maxrow)
{
  char tmp[2048];
  char dir[2048];
  char base[2048];
  strncpy(tmp, ifname, 2047);
  strncpy(dir, dirname(tmp), 2047);
  strncpy(tmp, ifname, 2047);
  strncpy(base, basename(tmp), 2047);

  char print_ifname[2048];
  sprintf(print_ifname, "%s/%s", dir, base);
  fprintf(stderr, "reading %s\n", print_ifname); // print the input file name

  VAL_complex data(maxrow);
  data = 0.0;

  FILE *ifp = NULL;
  ifp = fopen(ifname, "rb");
  if (ifp == NULL) // Deal with wrong file name
  {
    perror(ifname);
    exit(1);
  }

  fread(&data[0], sizeof(Complex), maxrow, ifp); // Use "fread" to read data
  fclose(ifp);

  return data;
}

void write_corr_double(const char *ifname, const char *prefix, int maxrow, const VAL_double &valdata)
{
  char tmp[2048];
  char dir[2048];
  char base[2048];
  strncpy(tmp, ifname, 2047);
  strncpy(dir, dirname(tmp), 2047);
  strncpy(tmp, ifname, 2047);
  strncpy(base, basename(tmp), 2047);

  char ofname[2048];

  sprintf(ofname, "%s/%s.%s", dir, prefix, base);
  fprintf(stderr, "generating %s\n", ofname); // print the output file name (print to the stderr file)

  FILE *ofp = NULL;
  ofp = fopen(ofname, "w");
  if (ofp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  fwrite(&valdata[0], sizeof(double), maxrow, ofp);
  fclose(ofp);
}

void write_corr_complex(const char *ifname, const char *prefix, int maxrow, const VAL_complex &valdata)
{
  char tmp[2048];
  char dir[2048];
  char base[2048];
  strncpy(tmp, ifname, 2047);
  strncpy(dir, dirname(tmp), 2047);
  strncpy(tmp, ifname, 2047);
  strncpy(base, basename(tmp), 2047);

  char ofname[2048];

  sprintf(ofname, "%s/%s.%s", dir, prefix, base);
  fprintf(stderr, "generating %s\n", ofname); // print the output file name (print to the stderr file)

  FILE *ofp = NULL;
  ofp = fopen(ofname, "w");
  if (ofp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  fwrite(&valdata[0], sizeof(Complex), maxrow, ofp);
  fclose(ofp);
}

void write_result_to_txt(const char *ofname, int maxrow, const VAL_complex &valdata, bool judge_4pt)
{
  fprintf(stderr, "generating %s\n", ofname); // print the output file name (print to the stderr file)

  FILE *ofp = NULL;
  ofp = fopen(ofname, "w");
  if (ofp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  if (judge_4pt)
  {
    int max_distance = 0;

    for (int i = 0; i < maxrow / 2 + 1; i++)
      for (int j = i; j < maxrow / 2 + 1; j++)
        for (int k = j; k < maxrow / 2 + 1; k++)
        {
          max_distance++;
        }

    double distance[max_distance];
    int index = 0;

    for (int i = 0; i < maxrow / 2 + 1; i++)
      for (int j = i; j < maxrow / 2 + 1; j++)
        for (int k = j; k < maxrow / 2 + 1; k++)
        {
          double temp = i * i + j * j + k * k;
          temp = pow(temp, 0.5);
          distance[index] = temp;
          ;

          index++;
        }

    for (int i = 0; i < max_distance; i++)
    {
      fprintf(ofp, "%1.16e  %1.16e  %1.16e\n", distance[i], __real__ valdata[i], __imag__ valdata[i]);
    }
  }
  else
  {
    for (int i = 0; i < maxrow; i++)
    {
      fprintf(ofp, "%d  %1.16e  %1.16e\n", i, __real__ valdata[i], __imag__ valdata[i]);
    }
  }

  fclose(ofp);
}