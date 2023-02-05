//---------------------------------------------------------------------
/**
 * @file jackknife.cc
 * @author TC (reeft137@gmail.com)
 * @brief
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

// Keep the real part of the data
void keep_real(VARRAY_COMPLEX &data, VARRAY_DOUBLE &rdata, int maxline)
{
  for (size_t i = 0; i < maxline; i++)
  {
    rdata[i] = data[i].real();
  }
}

// Jackknife sample
void jackknife_resample(const char *datalist[], int maxline, int N, char *r_datalist[])
{
  VARRAY_DOUBLE sum(maxline), sum_square(maxline), value(maxline), var(maxline);
  sum = sum_square = value = var = 0.0;

  // First round: Get sum and sum^2 of all data
  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp;
    tmp = 0.0;
    read_bin(datalist[i], maxline, tmp);

    VARRAY_DOUBLE rtmp;
    rtmp = 0.0;
    keep_real(tmp, rtmp, maxline);

    sum += rtmp;
    sum_square += pow(rtmp, 2);
  }

  // Second round: Generate the Jackknife sampled data and calculate the variance
  //               Also, save files and copy the file name to r_datalist[]
  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp;
    tmp = 0.0;
    read_bin(datalist[i], maxline, tmp);

    VARRAY_DOUBLE rtmp;
    rtmp = 0.0;
    keep_real(tmp, rtmp, maxline);

    value = (sum - rtmp) / (N - 1.0);
    var = sqrt(((sum_square - pow(rtmp, 2)) / double(N - 1.0) - pow(value, 2)) / double(N - 2.0));

    VARRAY_COMPLEX result(maxline);
    result = 0.0;
    for (int j = 0; j < maxline; j++)
    {
      result[j].real(value[j]);
      result[j].imag(var[j]);
    }

    char ofname[2048];
    add_prefix(datalist[i], "js", ofname);

    write_bin(ofname, maxline, result);

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
    VARRAY_COMPLEX tmp(maxline);
    tmp = 0.0;
    read_bin(datalist[i], maxline, tmp);

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
    VARRAY_COMPLEX tmp(maxline);
    tmp = 0.0;
    read_bin(datalist[i], maxline, tmp);

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
