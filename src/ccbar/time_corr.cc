/**
 * @file time_corr.cc
 * @author TC (reeft137@gmail.com)
 * @brief Main program to calculate the mass of ccbar meson
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
                  "    -timelength (int):       total # of time sites\n"
                  "    [-h, --help]:            see usage\n"
                  "    [-fit_range] (min max):  do fit and set the range for fitting mass\n");
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
void time_reverse_2pt(char *datalist[], int timelength, int file_total, char *r_datalist[]);

// Global variables to store the contents of options
static const char *correlator = NULL;
static const char *effmass = NULL;
int timelength = 0;
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

    if (strcmp(argv[0], "-correlator") == 0)
    {
      correlator = argv[1];
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

    if (strcmp(argv[0], "-timelength") == 0)
    {
      timelength = atoi(argv[1]); // atoi(): convert ASCII string to integer
      if (!timelength)
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
  int file_total = argc; // # of data files

  // Main Body
  // DATA ANALYSIS ON 2PT CORRELATION FUNCTIONS
  if (correlator)
  {
    // Create some string arrays for temparory file names (time reversed data, jackknife resampled data...)
    char *tr_tmp_datalist[file_total], *js_tmp_datalist[file_total];
    for (size_t i = 0; i < file_total; i++)
    {
      tr_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
      strncpy(tr_tmp_datalist[i], argv[i], 2047);
      js_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
      strncpy(js_tmp_datalist[i], argv[i], 2047);
    }

    // Do time reverse and Jackknife resampling
    time_reverse_2pt(argv, timelength, file_total, tr_tmp_datalist);
    jackknife_resample(tr_tmp_datalist, timelength, file_total, js_tmp_datalist);

    // Generate the 2pt correlation function
    jackknife_average(js_tmp_datalist, timelength, file_total, correlator);

    // Calculate the effective mass (exp and cosh types)
    if (effmass)
    {
      char expmass[2048], coshmass[2048];
      add_prefix(effmass, "exp", expmass);
      add_prefix(effmass, "cosh", coshmass);

      char *em_tmp_datalist[file_total], *hm_tmp_datalist[file_total];
      for (size_t i = 0; i < file_total; i++)
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

      jackknife_average(em_tmp_datalist, timelength, file_total, expmass);
      jackknife_average(hm_tmp_datalist, timelength, file_total, coshmass);

      // Remove temporary files
      for (size_t i = 0; i < file_total; i++)
      {
        if (remove(em_tmp_datalist[i]))
          perror(em_tmp_datalist[i]);
        if (remove(hm_tmp_datalist[i]))
          perror(hm_tmp_datalist[i]);
      }

      // Finalization for the string arrays
      for (size_t i = 0; i < file_total; i++)
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
    for (size_t i = 0; i < file_total; i++)
    {
      if (remove(tr_tmp_datalist[i]))
        perror(tr_tmp_datalist[i]);
      if (remove(js_tmp_datalist[i]))
        perror(js_tmp_datalist[i]);
    }

    // Finalization for the string arrays
    for (size_t i = 0; i < file_total; i++)
    {
      free(tr_tmp_datalist[i]);
      free(js_tmp_datalist[i]);
    }
  }

  return 0;
}

// Definition for the time_reverse_2pt function
void time_reverse_2pt(char *datalist[], int timelength, int file_total, char *r_datalist[])
{
  for (int i = 0; i < file_total; i++)
  {
    VARRAY_COMPLEX data;
    data = 0.0;
    read_bin(datalist[i], timelength, data);

    for (size_t i = 0; i < timelength / 2 + 1; i++)
    {
      data[i] = (data[i] + data[(timelength - i) % timelength]) * 0.5;
    }

    char ofname[2048];
    add_prefix(datalist[i], "TR", ofname);

    write_bin(ofname, timelength, data);

    strncpy(r_datalist[i], ofname, 2047);
  }
}

// Write to txt file
void write_txt(const char *fname, int timelength, const COMPLEX *data)
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

  for (size_t i = 0; i < timelength; i++)
  {
    fprintf(fp, "%d %1.16e %1.16e\n", i, data[i].real(), data[i].imag());
  }
  fclose(fp);
}

void write_txt(const char *fname, int timelength, const VARRAY_COMPLEX &data)
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

  for (size_t i = 0; i < timelength; i++)
  {
    fprintf(fp, "%d %1.16e %1.16e\n", i, data[i].real(), data[i].imag());
  }
  fclose(fp);
}