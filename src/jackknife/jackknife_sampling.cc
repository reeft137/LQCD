//---------------------------------------------------------------------
/**
 * @file jackknife_sampling.cc
 * @author Tianchen (zhangtc@rcnp.osaka-u.ac.jp)
 * @brief convert original data to jackknife sampled data
 * @version 0.1
 * @date 2023-01-16
 *
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

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
int maxrow = 0;
bool correlator_judge = false;

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

  if (argc < 2)
  {
    usage(program_name);
    exit(1);
  }

  int N = argc; // # of data files

  if (correlator_judge)
  {
    VAL_double sum(maxrow);
    sum = 0.0;
    VAL_double sum_square(maxrow);
    sum_square = 0.0;
    VAL_double out(maxrow);
    out = 0.0;
    VAL_double var(maxrow);
    var = 0.0;

    /**
     * @brief read the files for the first round to get the sum and sum^2 (also the time reverse is done)
     *
     */
    for (int i = 0; i < N; i++)
    {
      char *ifname = argv[i];
      VAL_complex tmp = read_corr_complex(ifname, maxrow);

      VAL_double tmp_double(maxrow);
      tmp_double = 0.0;
      for (int j = 0; j < maxrow; j++)
      {
        tmp_double[j] = __real__ tmp[j];
      }
      for (int j = 0; j < maxrow / 2 + 1; j++)
      {
        tmp_double[j] = (tmp_double[j] + tmp_double[(maxrow - j) % maxrow]) * 0.5;
      }

      sum += tmp_double;
      sum_square += tmp_double * tmp_double;
    }

    /**
     * @brief read the files for the second round to get the sampled data and the variance
     * 				also output the final file
     *
     */
    for (int i = 0; i < N; i++)
    {
      char *ifname = argv[i];
      VAL_complex tmp = read_corr_complex(ifname, maxrow);

      VAL_double tmp_double(maxrow);
      tmp_double = 0.0;
      for (int j = 0; j < maxrow; j++)
      {
        tmp_double[j] = __real__ tmp[j];
      }
      for (int j = 0; j < maxrow / 2 + 1; j++)
      {
        tmp_double[j] = (tmp_double[j] + tmp_double[(maxrow - j) % maxrow]) * 0.5;
      }

      out = (sum - tmp_double) / (N - 1.0);
      var = (sum_square - tmp_double * tmp_double) / double(N - 1);
      var = sqrt((var - out * out) / double(N - 2));

      write_corr_double(ifname, "sample", maxrow, out);

      VAL_complex varout_tmp(maxrow);
      varout_tmp = 0.0;
      for (int j = 0; j < maxrow; j++)
      {
        __real__ varout_tmp[j] = out[j];
        __imag__ varout_tmp[j] = var[j];
      }

      char dir[2048];
      char base[2048];
      strncpy(dir, dirname(argv[i]), 2047);
      strncpy(base, basename(argv[i]), 2047);

      char ofname[2048];
      sprintf(ofname, "%s/txt.sample_var.%s", dir, base);

      write_result_to_txt(ofname, maxrow, varout_tmp, false);
      // write_corr_complex(ifname, "jacksample_var", maxrow, varout_tmp);
    }
  }
  else
  {
    maxrow = maxrow * maxrow * maxrow;

    VAL_complex sum(maxrow);
    sum = 0.0;
    VAL_complex out(maxrow);
    out = 0.0;

    for (int i = 0; i < N; i++)
    {
      char *ifname = argv[i];
      VAL_complex tmp = read_corr_complex(ifname, maxrow);
      
      sum += tmp;
    }

    for (int i = 0; i < N; i++)
    {
      char *ifname = argv[i];
      VAL_complex tmp = read_corr_complex(ifname, maxrow);

      tmp = read_corr_complex(ifname, maxrow);
      out = (sum - tmp) / (N - 1.0);

      write_corr_complex(ifname, "sample", maxrow, out);
    }
  }

  return 0;
}

void usage(char *name)
{
  fprintf(stderr, "SYNOPSIS\n");
  fprintf(stderr, "    %s [-2pt] maxrow ifname_1 ifname_2 [ifname_3...]\n", name);
  fprintf(stderr, "DESCRIPTION\n");
  fprintf(stderr, "    maxrow:  time sites for 2pt; space sites for 4pt\n");
  fprintf(stderr, "    -2pt:    do the sampling for 2pt correlation function\n");
}