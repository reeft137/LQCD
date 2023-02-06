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
#include "nbsmisc.h"
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
                  "  MAN: \n"
                  "    [-h, --help]:           see usage\n"
                  "  2PT: \n"
                  "    [-corr_2pt] (ofname):   filename for the result of 2pt correlator\n"
                  "    [-effmass] (ofname):    filename for the result of effective mass\n"
                  "    [-fit_range] (min max): set the range for fitting mass\n"
                  "  4PT: \n"
                  "    [-corr_4pt] (ofname):   filename for the result of 4pt correlator\n"
                  "  MUST: \n"
                  "    -maxline (maxline):     2pt: # of time sites; 4pt: # of space sites\n");
}

// Custom functions and inline functions (to make code concise)
// Make directory and change the ownership
inline void mkchdir(const char *destination)
{
  if (mkdir(destination, 0777))
    perror(destination);
  if (chmod(destination, 0000755))
    perror(destination);
}

// Time reversal
void time_reverse_2pt(char *datalist[], int maxline, int N, char *r_datalist[]);

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
      effmass = "effmass";
      argc--;
      argv++;
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
      if (!fitmin || !fitmax || fitmin > fitmax || fitmin == fitmax)
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

  // Main Body
  // DATA ANALYSING ON 2PT CORRELATION FUNCTIONS
  if (corr_2pt)
  {
    // Create some string arrays for temparory file names (time reversed data, jackknife resampled data...)
    char *tr_tmp_datalist[N], *js_tmp_datalist[N];
    for (size_t i = 0; i < N; i++)
    {
      tr_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
      strncpy(tr_tmp_datalist[i], argv[i], 2047);
      js_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
      strncpy(js_tmp_datalist[i], argv[i], 2047);
    }

    // Do time reverse and Jackknife resampling
    time_reverse_2pt(argv, maxline, N, tr_tmp_datalist);
    jackknife_resample(tr_tmp_datalist, maxline, N, js_tmp_datalist);

    // Generate the 2pt correlation function
    jackknife_average(js_tmp_datalist, maxline, N, corr_2pt);

    // Calculate the effective mass (exp and cosh types)
    if (effmass)
    {
      char expmass[2048], coshmass[2048];
      add_prefix(effmass, "exp", expmass);
      add_prefix(effmass, "cosh", coshmass);

      char *em_tmp_datalist[N], *hm_tmp_datalist[N];
      for (size_t i = 0; i < N; i++)
      {
        em_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
        strncpy(em_tmp_datalist[i], argv[i], 2047);
        hm_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
        strncpy(hm_tmp_datalist[i], argv[i], 2047);
      }

      // Something from effmass.h
      /*
      sadfsafdsfsdafsa
      asdfdsafdsa
      fd
      */

      jackknife_average(em_tmp_datalist, maxline, N, expmass);
      jackknife_average(hm_tmp_datalist, maxline, N, coshmass);

      // Remove temporary files
      for (size_t i = 0; i < N; i++)
      {
        if (remove(em_tmp_datalist[i]))
          perror(em_tmp_datalist[i]);
        if (remove(hm_tmp_datalist[i]))
          perror(hm_tmp_datalist[i]);
      }

      // Finalization for the string arrays
      for (size_t i = 0; i < N; i++)
      {
        free(em_tmp_datalist[i]);
        free(hm_tmp_datalist[i]);
      }
    }

    // Fit the mass and get the final result
    if (fit_do)
    {
    }

    // Remove temporary files
    for (size_t i = 0; i < N; i++)
    {
      if (remove(tr_tmp_datalist[i]))
        perror(tr_tmp_datalist[i]);
      if (remove(js_tmp_datalist[i]))
        perror(js_tmp_datalist[i]);
    }

    // Finalization for the string arrays
    for (size_t i = 0; i < N; i++)
    {
      free(tr_tmp_datalist[i]);
      free(js_tmp_datalist[i]);
    }
  }

  // DATA ANALYSING ON 4PT CORRELATION FUNCTIONS
  if (corr_4pt)
  {
    // Create some string arrays for temparory file names (A1+ data, jackknife resampled data...)
    char *a1_tmp_datalist[N], *n2_tmp_datalist[N], *js_tmp_datalist[N];
    for (size_t i = 0; i < N; i++)
    {
      a1_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
      strncpy(a1_tmp_datalist[i], argv[i], 2047);
      n2_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
      strncpy(n2_tmp_datalist[i], argv[i], 2047);
      js_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
      strncpy(js_tmp_datalist[i], argv[i], 2047);
    }

    // Length of the data of space correlators
    int maxline3 = int(pow(maxline, 3));

    // A1+ projection and normalization
    a1_plus(argv, maxline, N, a1_tmp_datalist);
    normalize(a1_tmp_datalist, maxline, N, n2_tmp_datalist);

    // Calculate correlators
    jackknife_resample(n2_tmp_datalist, maxline3, N, js_tmp_datalist);
    jackknife_average(js_tmp_datalist, maxline3, N, corr_4pt);
    cartesian_to_spherical(corr_4pt, maxline);

    // Kawanai-Sasaki method

    // Watanabe method
    
    // Remove temporary files
    for (size_t i = 0; i < N; i++)
    {
      if (remove(a1_tmp_datalist[i]))
        perror(a1_tmp_datalist[i]);
      if (remove(n2_tmp_datalist[i]))
        perror(n2_tmp_datalist[i]);
      if (remove(js_tmp_datalist[i]))
        perror(js_tmp_datalist[i]);
    }
    if (remove(corr_4pt))
      perror(corr_4pt);

    // Finalization for the string arrays
    for (size_t i = 0; i < N; i++)
    {
      free(a1_tmp_datalist[i]);
      free(n2_tmp_datalist[i]);
      free(js_tmp_datalist[i]);
    }
  }

  return 0;
}

// Definition for the time_reverse_2pt function
void time_reverse_2pt(char *datalist[], int maxline, int N, char *r_datalist[])
{
  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX data;
    data = 0.0;
    read_bin(datalist[i], maxline, data);

    for (size_t i = 0; i < maxline / 2 + 1; i++)
    {
      data[i] = (data[i] + data[(maxline - i) % maxline]) * 0.5;
    }

    char ofname[2048];
    add_prefix(datalist[i], "TR", ofname);

    write_bin(ofname, maxline, data);

    strncpy(r_datalist[i], ofname, 2047);
  }
}