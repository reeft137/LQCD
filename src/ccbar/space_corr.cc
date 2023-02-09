/**
 * @file space_corr.cc
 * @author TC (reeft137@gmail.com)
 * @brief Main program to calculate the potential (and somthing else) of ccbar meson
 * @version 0.1
 * @date 2023-02-02
 *
 */

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
                  "    -spacelength (int):  total# of space sites\n"
                  "    -corr (ofname):      output file name of correlator"
                  "    [-h, --help]:        see usage\n");
}

// Custom functions (to make code concise)
// Get the value of correlator on a specific spacial site
COMPLEX &correlator(COMPLEX *data, int x, int y, int z, int spacelength);

// Simplification for the A1+ projection
COMPLEX sphere_sym(COMPLEX *data, int x, int y, int z, int spacelength);
COMPLEX a1_sym(COMPLEX *data, int x, int y, int z, int spacelength);

// A1+ projection main
void a1_plus(char *datalist[], char *r_datalist[], int spacelength, int file_total);

// N2 normalization
void normalization(char *datalist[], char *r_datalist[], int spacelength, int file_total);

// Change to spherical coordinate
void cartesian_to_spherical(const char *ifname, const char *ofname, int spacelength);

// Global variables to store the contents of options
int spacelength = 0;
static const char *corr = NULL;

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

    if (strcmp(argv[0], "-spacelength") == 0)
    {
      spacelength = atoi(argv[1]); // atoi(): convert ASCII string to integer
      if (!spacelength)
      {
        usage(program_name);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-corr") == 0)
    {
      corr = argv[1];
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
  const int file_total = argc; // total # of files
  fprintf(stderr, "##################################################\n");
  fprintf(stderr, "# Total of configurations: %d\n", file_total);
  fprintf(stderr, "##################################################\n\n");

  // Main Body
  // Create some string arrays for temparory file names (A1+ data, jackknife resampled data...)
  char *a1_tmp_datalist[file_total], *n2_tmp_datalist[file_total], *js_tmp_datalist[file_total];
  for (size_t i = 0; i < file_total; i++)
  {
    a1_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char)); // malloc: allocate memory for a pointer
    add_prefix(argv[i], "A1+", a1_tmp_datalist[i]);
    n2_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char));
    add_prefix(argv[i], "n2", n2_tmp_datalist[i]);
    js_tmp_datalist[i] = (char *)malloc(2048 * sizeof(char));
    add_prefix(argv[i], "js", js_tmp_datalist[i]);
  }

  // Length of the data of space correlators

  // A1+ projection and normalization
  fprintf(stderr, "##################################################\n");
  fprintf(stderr, "A1+ projection\n");
  fprintf(stderr, "##################################################\n\n");
  a1_plus(argv, a1_tmp_datalist, spacelength, file_total);
  fprintf(stderr, "Finished! \n\n");

  // fprintf(stderr, "##################################################\n");
  // fprintf(stderr, "Normalization\n");
  // fprintf(stderr, "##################################################\n\n");
  // normalization(a1_tmp_datalist, n2_tmp_datalist, spacelength, file_total);
  // fprintf(stderr, "Finished! \n\n");

  int maxline = int(pow(spacelength, 3));
  // Calculate correlators
  fprintf(stderr, "##################################################\n");
  fprintf(stderr, "Jackknife resample\n");
  fprintf(stderr, "##################################################\n\n");
  jackknife_resample(a1_tmp_datalist, js_tmp_datalist, maxline, file_total);
  fprintf(stderr, "Finished! \n\n");

  fprintf(stderr, "##################################################\n");
  fprintf(stderr, "Jackknife average\n");
  fprintf(stderr, "##################################################\n\n");
  char tmp_result[2047];
  add_prefix(corr, "tmp", tmp_result);
  jackknife_average(js_tmp_datalist, tmp_result, maxline, file_total);
  fprintf(stderr, "Finished! \n\n");

  fprintf(stderr, "##################################################\n");
  fprintf(stderr, "Finalize the final result\n");
  fprintf(stderr, "##################################################\n\n");
  cartesian_to_spherical(tmp_result, corr, spacelength);
  fprintf(stderr, "Finished! \n\n");

  // Kawanai-Sasaki method

  // Watanabe method

  // Remove temporary files
  fprintf(stderr, "##################################################\n");
  fprintf(stderr, "Remove temporary files\n");
  fprintf(stderr, "##################################################\n\n");
  for (size_t i = 0; i < file_total; i++)
  {
    if (remove(a1_tmp_datalist[i]))
      perror(a1_tmp_datalist[i]);
    if (remove(n2_tmp_datalist[i]))
      perror(n2_tmp_datalist[i]);
    if (remove(js_tmp_datalist[i]))
      perror(js_tmp_datalist[i]);
  }
  if (remove(tmp_result))
    perror(tmp_result);
  fprintf(stderr, "Finished! \n\n");

  // Finalization for the string arrays
  for (size_t i = 0; i < file_total; i++)
  {
    free(a1_tmp_datalist[i]);
    free(n2_tmp_datalist[i]);
    free(js_tmp_datalist[i]);
  }

  return 0;
}

// Definitions of functions
inline COMPLEX &correlator(COMPLEX *data, int x, int y, int z, int spacelength)
{
  x = (x + spacelength) % spacelength;
  y = (y + spacelength) % spacelength;
  z = (z + spacelength) % spacelength;
  COMPLEX &corr = data[x + spacelength * (y + spacelength * z)];
  return corr;
}

inline COMPLEX sphere_sym(COMPLEX *data, int x, int y, int z, int spacelength)
{
  return (correlator(data, x, y, z, spacelength) + correlator(data, y, z, x, spacelength) + correlator(data, z, x, y, spacelength) + correlator(data, x, z, y, spacelength) + correlator(data, z, y, x, spacelength) + correlator(data, y, x, z, spacelength)) / 6.0;
}

inline COMPLEX a1_sym(COMPLEX *data, int x, int y, int z, int spacelength)
{
  return (sphere_sym(data, x, y, z, spacelength) + sphere_sym(data, x, y, spacelength - z, spacelength) + sphere_sym(data, x, spacelength - y, z, spacelength) + sphere_sym(data, x, spacelength - y, spacelength - z, spacelength) + sphere_sym(data, spacelength - x, y, z, spacelength) + sphere_sym(data, spacelength - x, y, spacelength - z, spacelength) + sphere_sym(data, spacelength - x, spacelength - y, z, spacelength) + sphere_sym(data, spacelength - x, spacelength - y, spacelength - z, spacelength)) / 8.0;
}

void a1_plus(char *datalist[], char *r_datalist[], int spacelength, int file_total)
{
  int maxline = int(pow(spacelength, 3));

  for (size_t i = 0; i < file_total; i++)
  {
    COMPLEX tmp[maxline], result[maxline];
    for (size_t j = 0; j < maxline; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    read_bin(datalist[i], maxline, tmp);

    for (size_t ix = 0; ix < spacelength; ix++)
      for (size_t iy = 0; iy < spacelength; iy++)
        for (size_t iz = 0; iz < spacelength; iz++)
        {
          correlator(result, ix, iy, iz, spacelength) = a1_sym(tmp, ix, iy, iz, spacelength);
        }

    write_bin(r_datalist[i], maxline, result);
  }
}

void normalization(char *datalist[], char *r_datalist[], int spacelength, int file_total)
{
  int maxline = int(pow(spacelength, 3));

  for (size_t i = 0; i < file_total; i++)
  {
    COMPLEX tmp[maxline], result[maxline];
    for (size_t j = 0; j < maxline; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    read_bin(datalist[i], maxline, tmp);
    for (size_t j = 0; j < maxline; j++)
    {
      result[j] = tmp[j] / sqrt((tmp[j] * std::conj(tmp[j])).real());
    }

    write_bin(r_datalist[i], maxline, result);
  }
}

void cartesian_to_spherical(const char *ifname, const char *ofname, int spacelength)
{
  int maxline = pow(spacelength, 3);

  COMPLEX tmp[maxline];
  for (size_t j = 0; j < maxline; j++) // Initialize the empty arrays
  {
    tmp[j] = 0.0;
  }
  read_bin(ifname, maxline, tmp);

  char pname[2048];
  gen_print_name(ofname, pname);
  fprintf(stderr, "Writing data to '%s'... \n", pname);

  FILE *fp = fopen(ofname, "w");
  if (fp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  for (size_t i = 0; i < spacelength / 2 + 1; i++)
    for (size_t j = i; j < spacelength / 2 + 1; j++)
      for (size_t k = j; k < spacelength / 2 + 1; k++)
      {
        double value, variance, distance = 0.0;

        distance = sqrt(pow(double(i), 2) + pow(double(j), 2) + pow(double(k), 2));
        value = correlator(tmp, i, j, k, spacelength).real();
        variance = correlator(tmp, i, j, k, spacelength).imag();

        fprintf(fp, "%1.16e %1.16e %1.16e\n", distance, value, variance);
      }

  fclose(fp);
}
