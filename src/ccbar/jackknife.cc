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
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "valarray_io.h"

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
    valtmp[i] = real(valdata[i]);
  }
  for (int i = 0; i < maxline / 2 + 1; i++)
  {
    valtmp[i] = (valtmp[i] + valtmp[(maxline - i) % maxline]) * 0.5;
  }
  return valtmp;
}

// Jackknife sample
void jackknife_sample(char *datalist[], int maxline, int N)
{
  VARRAY_DOUBLE sum(maxline), sum_square(maxline), value(maxline), var(maxline);
  sum = sum_square = value = var = 0.0;

  /**
   * @brief read the files for the first round to get the sum and sum^2,
   *        also the time reverse is done
   *
   */
  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp = read_complex_varray(datalist[i], maxline);

    sum += time_reverse(tmp, maxline);
    sum_square += time_reverse(tmp, maxline) * time_reverse(tmp, maxline);
  }

  /**
   * @brief read the files for the second round to get the sampled data and the variance,
   * 				also output the final file
   *
   */
  for (int i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp = read_complex_varray(datalist[i], maxline);

    value = (sum - time_reverse(tmp, maxline)) / (N - 1.0);
    var = (sum_square - time_reverse(tmp, maxline) * time_reverse(tmp, maxline)) / double(N - 1);
    var = sqrt((var - value * value) / double(N - 2));

    VARRAY_COMPLEX result(maxline);
    result = 0.0;

    for (int j = 0; j < maxline; j++)
    {
      result[j].real(value[j]);
      result[j].imag(var[j]);
    }

    write_complex_varray(datalist[i], "sample", maxline, result);
  }
}
