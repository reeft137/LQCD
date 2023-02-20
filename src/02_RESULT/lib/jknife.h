//---------------------------------------------------------------------
/**
 * @file jackknife.h
 * @author TC (reeft137@gmail.com)
 * @brief Provide functions of jackknife sample and average.
 *        Also, these functions will provide the file names
 *        of the sampled data.
 * @version 0.1
 * @date 2023-01-16
 *
 */
//---------------------------------------------------------------------

#ifndef IS_INCLUDED_JACKKNIFE_H
#define IS_INCLUDED_JACKKNIFE_H

// Custom libraries
#include "dataio.h"

// enum for judging the data type
enum DATATYPE
{
  SINGLE_LINE = 1, DOUBLE_LINE = 2
};

// Jackknife resampling
void jackknife_resample(char *rawdata[], char *realresult[], int maxline, int totalfile);
void jackknife_resample(char *rawdata[], char *realresult[], char *imagresult[], int maxline, int totalfile);

// Jackknife average
void jackknife_average(char *rawdata[], const char *result, int maxline, int totalfile, DATATYPE datatype);

#endif