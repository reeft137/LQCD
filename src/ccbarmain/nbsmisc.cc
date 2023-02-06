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
// Extract the value of the correlation function on a specific space site
inline COMPLEX CORR(const COMPLEX *data, int xsite, int ysite, int zsite, int spacelength)
{
  xsite = (xsite + spacelength) % spacelength;
  ysite = (ysite + spacelength) % spacelength;
  zsite = (zsite + spacelength) % spacelength;
  return data[xsite + spacelength * (ysite + spacelength * zsite)];
}

inline double CORR(const double *data, int xsite, int ysite, int zsite, int spacelength)
{
  xsite = (xsite + spacelength) % spacelength;
  ysite = (ysite + spacelength) % spacelength;
  zsite = (zsite + spacelength) % spacelength;
  return data[xsite + spacelength * (ysite + spacelength * zsite)];
}

// Simplification for the A1+ projection
inline COMPLEX SPHERE(const COMPLEX *data, int xsite, int ysite, int zsite, int spacelength)
{
  return CORR(data, xsite, ysite, zsite, spacelength) + CORR(data, ysite, zsite, xsite, spacelength) + CORR(data, zsite, xsite, ysite, spacelength) + CORR(data, xsite, zsite, ysite, spacelength) + CORR(data, zsite, ysite, xsite, spacelength) + CORR(data, ysite, xsite, zsite, spacelength);
}

inline COMPLEX A1(const COMPLEX *data, int xsite, int ysite, int zsite, int spacelength)
{
  return SPHERE(data, xsite, ysite, zsite, spacelength) + SPHERE(data, xsite, ysite, spacelength - zsite, spacelength) + SPHERE(data, xsite, spacelength - ysite, zsite, spacelength) + SPHERE(data, xsite, spacelength - ysite, spacelength - zsite, spacelength) + SPHERE(data, spacelength - xsite, ysite, zsite, spacelength) + SPHERE(data, spacelength - xsite, ysite, spacelength - zsite, spacelength) + SPHERE(data, spacelength - xsite, spacelength - ysite, zsite, spacelength) + SPHERE(data, spacelength - xsite, spacelength - ysite, spacelength - zsite, spacelength);
}

void a1_plus(const char *datalist[], int spacelength, int N, char *r_datalist[])
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
          CORR(result, ix, iy, iz, spacelength) = A1(tmp, ix, iy, iz, spacelength);
        }
      }
    }

    char ofname[2048];
    add_prefix(datalist[i], "A1+", ofname);

    write_bin(ofname, maxline, result);

    strncpy(r_datalist[i], ofname, 2047);
  }
}

void normalize(const char *datalist[], int spacelength, int N, char *r_datalist[])
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

  FILE *ifp = fopen(fname, "r");
  if (ifp == NULL)
  {
    perror(fname);
    exit(1);
  }

  char ofname[2048];
  add_prefix(fname, "sphere", ofname);
  fprintf(stderr, "Transfer data from '%s' to file '%s'... \n", fname, ofname);

  FILE *ofp = fopen(ofname, "w");
  if (ofp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  for (size_t i = 0; i < maxline; i++)
  {
    double value, variance, distance = 0.0;
    int index, x, y, z = 0;

    fscanf(ifp, "%02d %1.16e %1.16e\n", index, value, variance);

    x = index % spacelength;
    index = (index - x) / spacelength;
    y = index % spacelength;
    index = (index - y) / spacelength;
    z = index % spacelength;

    distance = sqrt(pow(double(x), 2) + pow(double(y), 2) + pow(double(z), 2));

    fprintf(ofp, "%1.16d %1.16e %1.16e\n", distance, value, variance);
  }
  fclose(ifp);
  fclose(ofp);
}
