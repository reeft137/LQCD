/**
 * @file main.cc
 * @author TC (reeft137@gmai.com)
 * @brief Main function for calculating the mass and potential of ccbar meson
 * @version 0.1
 * @date 2023-02-02
 *
 */

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "valarray_io.h"
#include "jackknife.h"

// Rename some lengthy type name
typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// Usage function
void usage(char *name)
{
  fprintf(stderr, "USAGE: \n"
                  "    %s [OPTIONS] ifname_1 ifname_2 [ifname_3...]\n",
          name);
  fprintf(stderr, "OPTIONS: \n"
                  "    -h, --help:           see usage\n"
                  "    -corr_2pt (ofname):   result of 2pt correlator\n"
                  "    -effmass (ofname):    result of effective mass\n"
                  "    -corr_4pt (ofname):   result of 4pt correlator\n"
                  "    -maxline (maxline):   2pt: # of time sites; 4pt: # of space sites\n"
                  "    -fit_range (min max): set the range for fitting mass");
}

// Inline functions (to make code concise)
// Make directory and change the ownership
inline void mkchdir(const char *basename)
{
  if (mkdir(basename, 0777))
    perror(basename);
  if (chmod(basename, 0000755))
    perror(basename);
}

// Add prefix to a file path
inline char *add_prefix(const char *fname, const char *prefix)
{
  char stmp[4096], dir[4096], base[4096], ofname[4096];
  strncpy(stmp, fname, 4095);
  strncpy(dir, dirname(stmp), 4095);
  strncpy(base, basename(stmp), 4095);
  sprintf(ofname, "%s/%s.%s", dir, prefix, base);
  return ofname;
}

// Global variables to store the contents of options
static const char *corr_2pt = NULL;
static const char *effmass = NULL;
static const char *corr_4pt = NULL;
int maxline = 0;
int fitmin = 0;
int fitmax = 0;

int main(int argc, char *argv[])
{
  char program_name[4096];
  strncpy(program_name, basename(argv[0]), 4095);
  argc--;
  argv++;

  while (argc > 0 && argv[0][0] == '-') // deal with all options regardless of their order
  {
    // -h and --help options and syntax check
    if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0)
    {
      usage(program_name);
      exit(0);
    }

    if (strcmp(argv[0], "-corr_2pt") == 0)
    {
      corr_2pt = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-effmass") == 0)
    {
      effmass = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-corr_4pt") == 0)
    {
      corr_4pt = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-maxline") == 0)
    {
      maxline = atoi(argv[1]); // atoi(): convert ASCII string to integer
      if (!maxline)
      {
        usage(program_name);
        exit(1);
      }
      argc -= 2;
      argv += 2;
    }

    fprintf(stderr, "Error: Unknown option '%s'\n", argv[0]);
    exit(1);
  }

  if (argc < 2)
  {
    usage(program_name);
    exit(1);
  }

  int N = argc; // # of data files

  if (corr_2pt)
  {
    jackknife_sample(argv, maxline, N);

    jackknife_average();
  }
}