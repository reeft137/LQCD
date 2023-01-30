//---------------------------------------------------------------------
/**
 * @file jackknife_average.cc
 * @author Tianchen (zhangtc@rcnp.osaka-u.ac.jp)
 * @brief calculate the average of data with jackknife variance
 * @version 0.1
 * @date 2023-01-16
 *
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <math.h>

#include <valarray>

#include "valarray_io.h"

typedef double _Complex Complex;
typedef std::valarray<Complex> VAL_complex;
typedef std::valarray<double> VAL_double;

void usage(char *);

//---------------------------------------------------------------------
/**
 * @brief global variables to store the contents of options
 *
 */
//---------------------------------------------------------------------
bool correlator_judge = false;
int maxrow = 0;
static const char *ofname = NULL;

int main(int argc, char *argv[])
{
  char program_name[2048];
  strncpy(program_name, basename(argv[0]), 2047);
  argc--;
  argv++;

  // -h and --help options and syntax check
  if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0)
  {
    usage(program_name);
    exit(1);
  }

  if (strcmp(argv[0], "-2pt") == 0)
  {
    correlator_judge = true;
    argc--;
    argv++;
  }

  maxrow = atoi(argv[0]); // atoi(): convert ASCII string to integer
  if (!maxrow)
  {
    usage(program_name);
    exit(1);
  }
  argc--;
  argv++;
  if (argc < 3)
  {
    usage(program_name);
    exit(1);
  }

  ofname = argv[0];
  argc--;
  argv++;

  int N = argc; // # of data files

  if (correlator_judge)
  {
    VAL_double mean(maxrow);
    mean = 0.0;
    VAL_double var(maxrow);
    var = 0.0;

    for (int i = 0; i < N; i++)
    {
      char *ifname = argv[i];
      VAL_double tmp = read_corr_double(ifname, maxrow);

      mean += tmp / double(N);
    }

    for (int i = 0; i < N; i++)
    {
      char *ifname = argv[i];
      VAL_double tmp = read_corr_double(ifname, maxrow);

      var += (tmp - mean) * (tmp - mean);
    }

    var = sqrt(var * double(N - 1) / double(N));

    VAL_complex varout_tmp(maxrow);
    varout_tmp = 0.0;

    for (int i = 0; i < maxrow; i++)
    {
      __real__ varout_tmp[i] = mean[i];
      __imag__ varout_tmp[i] = var[i];
    }
    
    write_result_to_txt(ofname, maxrow, varout_tmp, false);
  }
}

void usage(char *name)
{
  fprintf(stderr, "SYNOPSIS\n");
  fprintf(stderr, "    %s [-2pt] maxrow ofname ifname_1 ifname_2 [ifname_3...]\n", name);
  fprintf(stderr, "DESCRIPTION\n");
  fprintf(stderr, "    maxrow:  time sites for 2pt; space sites for 4pt\n");
  fprintf(stderr, "    -2pt:    deal with 2pt correlation function\n");

}