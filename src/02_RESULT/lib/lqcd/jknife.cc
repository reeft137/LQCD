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

#include "jknife.h"

// Jackknife resampling
void jackknife_resample(char *rawdlist[], char *reallist[], int maxline, int N_totalfile)
{
  DVARRAY sum(maxline), sum_square(maxline), value(maxline), var(maxline);
  sum = sum_square = value = var = 0.0;

  // First round: Get sum and sum^2 of all data
  for (int i = 0; i < N_totalfile; i++)
  {
    CVARRAY tmp(maxline);
    tmp = 0.0;
    read_bin(rawdlist[i], maxline, tmp);

    DVARRAY rtmp(maxline);
    rtmp = 0.0;
    keep_real(tmp, rtmp, maxline);

    sum += rtmp;
    sum_square += pow(rtmp, 2);
  }

  // Second round: Generate the Jackknife sampled data and calculate the variance
  //               Also, save files to reallist[]
  for (int i = 0; i < N_totalfile; i++)
  {
    CVARRAY tmp(maxline);
    tmp = 0.0;
    read_bin(rawdlist[i], maxline, tmp);

    DVARRAY rtmp(maxline);
    rtmp = 0.0;
    keep_real(tmp, rtmp, maxline);

    value = (sum - rtmp) / (N_totalfile - 1.0);
    // About this variance, please refer to eq.(7.37) on P.383, Montvay LQCD book
    var = sqrt(((sum_square - pow(rtmp, 2)) / DOUBLE(N_totalfile - 1.0) - pow(value, 2)) / DOUBLE(N_totalfile - 2.0));

    CVARRAY result(maxline);
    result = 0.0;
    for (int j = 0; j < maxline; j++)
    {
      result[j].real(value[j]);
      result[j].imag(var[j]);
    }

    write_bin(reallist[i], maxline, result);
  }
}

// Another version that also keeps the imaginary part
void jackknife_resample(char *rawdlist[], char *reallist[], char *imaglist[], int maxline, int N_totalfile)
{
  DVARRAY rsum(maxline), rsum_square(maxline), rvalue(maxline), rvar(maxline);
  DVARRAY isum(maxline), isum_square(maxline), ivalue(maxline), ivar(maxline);
  rsum = rsum_square = rvalue = rvar = 0.0;
  isum = isum_square = ivalue = ivar = 0.0;

  // First round: Get sum and sum^2 of all data
  for (int i = 0; i < N_totalfile; i++)
  {
    CVARRAY tmp(maxline);
    tmp = 0.0;
    read_bin(rawdlist[i], maxline, tmp);

    DVARRAY rtmp(maxline), itmp(maxline);
    rtmp = itmp = 0.0;
    keep_real(tmp, rtmp, maxline);
    keep_imag(tmp, itmp, maxline);

    rsum += rtmp;
    rsum_square += pow(rtmp, 2);

    isum += itmp;
    isum_square += pow(itmp, 2);
  }

  // Second round: Generate the Jackknife sampled data and calculate the variance
  //               Also, save files to reallist[]
  for (int i = 0; i < N_totalfile; i++)
  {
    CVARRAY tmp(maxline);
    tmp = 0.0;
    read_bin(rawdlist[i], maxline, tmp);

    DVARRAY rtmp(maxline), itmp(maxline);
    rtmp = itmp = 0.0;
    keep_real(tmp, rtmp, maxline);
    keep_imag(tmp, itmp, maxline);

    rvalue = (rsum - rtmp) / (N_totalfile - 1.0);
    // About this variance, please refer to eq.(7.37) on P.383, Montvay LQCD book
    rvar = sqrt(((rsum_square - pow(rtmp, 2)) / DOUBLE(N_totalfile - 1.0) - pow(rvalue, 2)) / DOUBLE(N_totalfile - 2.0));

    CVARRAY rresult(maxline);
    rresult = 0.0;
    for (int j = 0; j < maxline; j++)
    {
      rresult[j].real(rvalue[j]);
      rresult[j].imag(rvar[j]);
    }

    write_bin(reallist[i], maxline, rresult);

    ivalue = (isum - itmp) / (N_totalfile - 1.0);
    // About this variance, please refer to eq.(7.37) on P.383, Montvay LQCD book
    ivar = sqrt(((isum_square - pow(itmp, 2)) / DOUBLE(N_totalfile - 1.0) - pow(ivalue, 2)) / DOUBLE(N_totalfile - 2.0));

    CVARRAY iresult(maxline);
    iresult = 0.0;
    for (int j = 0; j < maxline; j++)
    {
      iresult[j].real(ivalue[j]);
      iresult[j].imag(ivar[j]);
    }

    write_bin(imaglist[i], maxline, iresult);
  }
}

// Jackknife average
void jackknife_average(char *rawdlist[], const char *result, int maxline, int N_totalfile, DATATYPE type)
{
  DVARRAY mean(maxline), var(maxline);
  mean = var = 0.0;

  if (type == SINGLE_LINE)
  {
    for (int i = 0; i < N_totalfile; i++)
    {
      DVARRAY tmp(maxline);
      tmp = 0.0;
      read_bin(rawdlist[i], maxline, tmp);

      mean += tmp / DOUBLE(N_totalfile);
    }

    for (int i = 0; i < N_totalfile; i++)
    {
      DVARRAY tmp(maxline);
      tmp = 0.0;
      read_bin(rawdlist[i], maxline, tmp);

      var += pow((tmp - mean), 2);
    }

    var = sqrt(var * DOUBLE(N_totalfile - 1) / DOUBLE(N_totalfile));

    // Output the final result to txt file
    CVARRAY jkout(maxline);
    jkout = 0.0;

    for (int i = 0; i < maxline; i++)
    {
      jkout[i].real(mean[i]);
      jkout[i].imag(var[i]);
    }

    write_bin(result, maxline, jkout);
  }

  if (type == DOUBLE_LINE)
  {
    for (int i = 0; i < N_totalfile; i++)
    {
      CVARRAY tmp(maxline);
      tmp = 0.0;
      read_bin(rawdlist[i], maxline, tmp);

      DVARRAY rtmp(maxline);
      rtmp = 0.0;
      keep_real(tmp, rtmp, maxline);

      mean += rtmp / DOUBLE(N_totalfile);
    }

    for (int i = 0; i < N_totalfile; i++)
    {
      CVARRAY tmp(maxline);
      tmp = 0.0;
      read_bin(rawdlist[i], maxline, tmp);

      DVARRAY rtmp(maxline);
      rtmp = 0.0;
      keep_real(tmp, rtmp, maxline);

      var += pow((rtmp - mean), 2);
    }

    var = sqrt(var * DOUBLE(N_totalfile - 1) / DOUBLE(N_totalfile));

    // Output the final result to txt file
    CVARRAY jkout(maxline);
    jkout = 0.0;

    for (int i = 0; i < maxline; i++)
    {
      jkout[i].real(mean[i]);
      jkout[i].imag(var[i]);
    }

    write_bin(result, maxline, jkout);
  }
}
