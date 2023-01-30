//---------------------------------------------------------------------
/**
 * @file eff_m_exp.cc
 * @author Tianchen (zhangtc@rcnp.osaka-u.ac.jp)
 * @brief Calculate the effective mass of the exp type
 * @version 0.1
 * @date 2021-11-10
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

typedef std::valarray<double> VAL_double;

void usage(char *);

//---------------------------------------------------------------------
/**
 * @brief global variables to store the contents of options
 *
 */
//---------------------------------------------------------------------
int maxrow = 0;

int main(int argc, char *argv[])
{
  char program_name[2048];
  strncpy(program_name, basename(argv[0]), 2047);
  argc--;
  argv++;

  // -h and --help options and syntax check
  if (argc < 2 || strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0)
  {
    usage(program_name);
    exit(1);
  }

  maxrow = atoi(argv[0]); // atoi(): convert ASCII string to integer
  if (!maxrow)
  {
    usage(program_name);
    exit(1);
  }
  argc--;
  argv++;

  char *ifname = argv[0];

  VAL_double m_exp(maxrow);
  m_exp = 0.0;
  VAL_double data = read_corr_double(ifname, maxrow);

  for (int i = 0; i < maxrow; i++)
  {
    m_exp[i] = log(data[i] / data[(i + 1) % maxrow]);
  }

  write_corr_double(ifname, "eff_m_exp", maxrow, m_exp);

  return 0;
}

void usage(char *name)
{
  fprintf(stderr, "SYNOPSIS\n");
  fprintf(stderr, "    %s maxrow ifname\n", name);
  fprintf(stderr, "DESCRIPTION\n");
  fprintf(stderr, "    maxrow:  time sites for 2pt; space sites for 4pt\n");
}
