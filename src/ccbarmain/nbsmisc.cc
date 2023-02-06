/**
 * @file nbsmisc.cc
 * @author TC (reeft137@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-05
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
#include "misc.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// Inline functions
// Extract the value of the correlatorelation function on a specific space site
// !!! Important !!!
// If you want some function return an lvalue, then please declare the type of the function to be a reference
COMPLEX &correlator(COMPLEX *data, int x, int y, int z, int spacelength)
{
  x = (x + spacelength) % spacelength;
  y = (y + spacelength) % spacelength;
  z = (z + spacelength) % spacelength;
  COMPLEX &corr = data[x + spacelength * (y + spacelength * z)];
  return corr;
}

inline double &correlator(double *data, int x, int y, int z, int spacelength)
{
  x = (x + spacelength) % spacelength;
  y = (y + spacelength) % spacelength;
  z = (z + spacelength) % spacelength;
  double &corr = data[x + spacelength * (y + spacelength * z)];
  return corr;
}

// Simplification for the A1+ projection
inline COMPLEX sphere_sym(COMPLEX *data, int x, int y, int z, int spacelength)
{
  return (correlator(data, x, y, z, spacelength) + correlator(data, y, z, x, spacelength) + correlator(data, z, x, y, spacelength) + correlator(data, x, z, y, spacelength) + correlator(data, z, y, x, spacelength) + correlator(data, y, x, z, spacelength)) / 6.0;
}

inline COMPLEX a1_sym(COMPLEX *data, int x, int y, int z, int spacelength)
{
  return (sphere_sym(data, x, y, z, spacelength) + sphere_sym(data, x, y, spacelength - z, spacelength) + sphere_sym(data, x, spacelength - y, z, spacelength) + sphere_sym(data, x, spacelength - y, spacelength - z, spacelength) + sphere_sym(data, spacelength - x, y, z, spacelength) + sphere_sym(data, spacelength - x, y, spacelength - z, spacelength) + sphere_sym(data, spacelength - x, spacelength - y, z, spacelength) + sphere_sym(data, spacelength - x, spacelength - y, spacelength - z, spacelength)) / 8.0;
}

void a1_plus(char *datalist[], int spacelength, int N, char *r_datalist[])
{
  int maxline = int(pow(spacelength, 3));

  for (size_t i = 0; i < N; i++)
  {
    COMPLEX tmp[maxline], result[maxline];
    for (size_t j = 0; j < maxline; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    read_bin(datalist[i], maxline, tmp);

    for (size_t ix = 0; ix < spacelength; ix++)
    {
      for (size_t iy = 0; iy < spacelength; iy++)
      {
        for (size_t iz = 0; iz < spacelength; iz++)
        {
          correlator(result, ix, iy, iz, spacelength) = a1_sym(tmp, ix, iy, iz, spacelength);
        }
      }
    }

    char ofname[2048];
    add_prefix(datalist[i], "A1+", ofname);

    write_bin(ofname, maxline, result);

    strncpy(r_datalist[i], ofname, 2047);
  }
}

void normalize(char *datalist[], int spacelength, int N, char *r_datalist[])
{
  int maxline = int(pow(spacelength, 3));

  for (size_t i = 0; i < N; i++)
  {
    COMPLEX tmp[maxline], result[maxline];
    for (size_t j = 0; j < maxline; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    read_bin(datalist[i], maxline, tmp);
    for (size_t j = 0; j < maxline; j++)
    {
      result[j] = tmp[j] / sqrt(tmp[j] * std::conj(tmp[j]));
    }

    char ofname[2048];
    add_prefix(datalist[i], "N2", ofname);

    write_bin(ofname, maxline, result);

    strncpy(r_datalist[i], ofname, 2047);
  }
}

void cartesian_to_spherical(const char *fname, int spacelength)
{
  int maxline = pow(spacelength, 3);

  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading data from '%s'... \n", pname);

  FILE *ifp = fopen(fname, "r");
  if (ifp == NULL)
  {
    perror(fname);
    exit(1);
  }

  char ofname[2048];
  add_prefix(fname, "sphere", ofname);
  fprintf(stderr, "Writing data to '%s'... \n", ofname);

  FILE *ofp = fopen(ofname, "w");
  if (ofp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, ifp) != -1) // Use getline() to get each line of data
  {
    double value, variance, distance = 0.0;
    int index, x, y, z = 0;

    // !!! Important !!!
    // To use (s)scanf to store value, destination should be a pointer!
    sscanf(line, "%d %lf %lf\n", &index, &value, &variance);

    x = index % spacelength;
    index = (index - x) / spacelength;
    y = index % spacelength;
    index = (index - y) / spacelength;
    z = index % spacelength;

    distance = sqrt(pow(double(x), 2) + pow(double(y), 2) + pow(double(z), 2));

    fprintf(ofp, "%1.16e %1.16e %1.16e\n", distance, value, variance);
  }
  if (line)
    free(line);

  fclose(ifp);
  fclose(ofp);
}
