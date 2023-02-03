/**
 * @file ccbarmain.cc
 * @author TC (reeft137@gmail.com)
 * @brief Main function for calculating the mass and potential of ccbar meson
 * @version 0.1
 * @date 2023-02-02
 *
 */

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"
#include "jackknife.h"
#include "misc.h"

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
                  "    [-h, --help]:           see usage\n"
                  "    [-corr_2pt] (ofname):   filename for the result of 2pt correlator\n"
                  "    [-effmass] (ofname):    filename for the result of effective mass\n"
                  "    [-corr_4pt] (ofname):   filename for the result of 4pt correlator\n"
                  "    [-fit_range] (min max): set the range for fitting mass\n"
                  "    -maxline (maxline):     2pt: # of time sites; 4pt: # of space sites\n");
}

// Inline functions (to make code concise)
// Make directory and change the ownership
inline void mkchdir(const char *destination)
{
  if (mkdir(destination, 0777))
    perror(destination);
  if (chmod(destination, 0000755))
    perror(destination);
}

// Global variables to store the contents of options
static const char *corr_2pt = NULL;
static const char *effmass = NULL;
static const char *corr_4pt = NULL;
int maxline = 0;
int fitmin = 0;
int fitmax = 0;
bool fit_do = false;

int main(int argc, char *argv[])
{
  char program_name[2048];
  strncpy(program_name, basename(argv[0]), 2047);
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

    if (strcmp(argv[0], "-fit_range") == 0)
    {
      fitmin = atoi(argv[1]);
      fitmax = atoi(argv[2]);
      if (!fitmin || !fitmax)
      {
        usage(program_name);
        exit(1);
      }
      fit_do = true;
      argc -= 3;
      argv += 3;
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
      continue;
    }

    fprintf(stderr, "Error: Unknown option '%s'\n", argv[0]);
    exit(1);
  }

  if (argc < 2)
  {
    usage(program_name);
    exit(1);
  }

  // Initialization
  int N = argc; // # of data files

  char *tmp_datalist[N];
  for (int i = 0; i < N; i++)
  {
    tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
    strncpy(tmp_datalist[i], argv[i], 2047);
  }

  // Main Body
  if (corr_2pt)
  {
    jackknife_sample(argv, maxline, N, tmp_datalist);

    jackknife_average(tmp_datalist, maxline, N, corr_2pt);
    for (int i = 0; i < N; i++)
    {
      if (remove(tmp_datalist[i]))
        perror(tmp_datalist[i]);
    }
  }

  if (effmass)
  {
  }

  if (fit_do)
  {
  }

  if (corr_4pt)
  {
    int maxline3 = int(pow(maxline, 3));

    jackknife_sample(argv, maxline3, N, tmp_datalist);

    fprintf(stderr, "Yeh!");

    jackknife_average(tmp_datalist, maxline3, N, corr_4pt);

    fprintf(stderr, "Yeh!");

    for (int i = 0; i < N; i++)
    {
      if (remove(tmp_datalist[i]))
        perror(tmp_datalist[i]);
    }

#define Corr(ptr, x, y, z) ((COMPLEX *)ptr)[(x) + maxline * ((y) + maxline * (z))]

    FILE *fp = NULL;
    fp = fopen(corr_4pt, "rb");

    COMPLEX raw[maxline3];

    fread(raw, sizeof(COMPLEX), maxline3, fp); // Use "fread" to read data
    fclose(fp);

    int sum = 0;

    for (int i = 0; i < maxline / 2 + 1; i++)
      for (int j = i; j < maxline / 2 + 1; j++)
        for (int k = j; k < maxline / 2 + 1; k++)
        {
          sum++;
        }

    double leng[sum];
    double real[sum];
    double imag[sum];

    int index = 0;

    for (int ix = 0; ix < maxline / 2 + 1; ix++)
      for (int iy = ix; iy < maxline / 2 + 1; iy++)
        for (int iz = iy; iz < maxline / 2 + 1; iz++)
        {
          double temp = ix * ix + iy * iy + iz * iz;
          temp = pow(temp, 0.5);
          leng[index] = temp;
          real[index] = Corr(raw, ix, iy, iz).real();
          imag[index] = Corr(raw, ix, iy, iz).imag();

          index++;
        }

    FILE *ofp = NULL;
    ofp = fopen("testresult.txt", "w");

    for (int i = 0; i < index; i++)
    {
      fprintf(ofp, "%1.16e %1.16e\n", leng[i], real[i]);
    }
  }

  // Finalization
  for (int i = 0; i < N; i++)
  {
    free(tmp_datalist[i]);
  }

  return 0;
}