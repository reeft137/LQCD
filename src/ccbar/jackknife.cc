//---------------------------------------------------------------------
/**
 * @file jackknife.cc
 * @author TC (reeft137@gmail.com)
 * @brief Convert original data to jackknife sampled data
 * @version 0.1
 * @date 2023-01-16
 *
 */
//---------------------------------------------------------------------

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

// Time reverse function
VARRAY_DOUBLE time_reverse(VARRAY_COMPLEX &valdata, int maxline)
{
  VARRAY_DOUBLE valtmp(maxline);
  valtmp = 0.0;
  for (int i = 0; i < maxline; i++)
  {
    valtmp[i] = valdata[i].real();
  }
  for (int i = 0; i < maxline / 2 + 1; i++)
  {
    valtmp[i] = (valtmp[i] + valtmp[(maxline - i) % maxline]) * 0.5;
  }
  return valtmp;
}

// Jackknife sample
void jackknife_sample(char *datalist[], int maxline, int N, char *r_datalist[])
{
  VARRAY_DOUBLE sum(maxline), sum_square(maxline), value(maxline), var(maxline);
  sum = sum_square = value = var = 0.0;

  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp = read_complex_varray(datalist[i], maxline);
    sum += time_reverse(tmp, maxline);
    sum_square += pow(time_reverse(tmp, maxline), 2);
  }

  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp = read_complex_varray(datalist[i], maxline);
    value = (sum - time_reverse(tmp, maxline)) / (N - 1.0);
    var = (sum_square - pow(time_reverse(tmp, maxline), 2)) / double(N - 1);
    var = sqrt((var - pow(value, 2)) / double(N - 2));

    VARRAY_COMPLEX result(maxline);
    result = 0.0;
    for (int j = 0; j < maxline; j++)
    {
      result[j].real(value[j]);
      result[j].imag(var[j]);
    }

    char ofname[2048];
    add_prefix(datalist[i], "js", ofname);
    write_complex_varray(ofname, maxline, result);

    strncpy(r_datalist[i], ofname, 2047);
  }
}

// Jackknife average
void jackknife_average(char *datalist[], int maxline, int N, const char *ofname)
{
  VARRAY_DOUBLE mean(maxline), var(maxline);
  mean = var = 0.0;

  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp = read_complex_varray(datalist[i], maxline);
    VARRAY_DOUBLE realtmp(maxline);
    realtmp = 0.0;
    for (int i = 0; i < maxline; i++)
    {
      realtmp[i] = tmp[i].real();
    }
    mean += realtmp / double(N);
  }

  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp = read_complex_varray(datalist[i], maxline);
    VARRAY_DOUBLE realtmp(maxline);
    realtmp = 0.0;
    for (int i = 0; i < maxline; i++)
    {
      realtmp[i] = tmp[i].real();
    }
    var += pow((realtmp - mean), 2);
  }

  var = sqrt(var * double(N - 1) / double(N));

  // Output the final result to txt file
  VARRAY_COMPLEX result(maxline);
  result = 0.0;

  for (int i = 0; i < maxline; i++)
  {
    result[i].real(mean[i]);
    result[i].imag(var[i]);
  }

  write_txt(ofname, maxline, result);
}