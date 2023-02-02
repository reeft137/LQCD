//---------------------------------------------------------------------
/**
 * @file jackknife_sampling.cc
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
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "valarray_io.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// Inline functions
inline VARRAY_DOUBLE time_reverse(VARRAY_COMPLEX &valdata, int maxline)
{
  VARRAY_DOUBLE valtmp(maxline);
  valtmp = 0.0;
  for (int j = 0; j < maxline; j++)
  {
    valtmp[j] = real(valdata[j]);
  }
  for (int j = 0; j < maxline / 2 + 1; j++)
  {
    valtmp[j] = (valtmp[j] + valtmp[(maxline - j) % maxline]) * 0.5;
  }
  return valtmp;
}

inline void mkchdir(const char *basename)
{
  if (mkdir(basename, 0777))
    perror("testdir");
  if (chmod(basename, 0000755))
    perror("testdir");
}

void usage(char *);

//---------------------------------------------------------------------
/**
 * @brief global variables to store the contents of options
 *
 */
//---------------------------------------------------------------------
int maxline = 0;
static const char *ofbase_2pt = NULL;
static const char *ofbase_4pt = NULL;

int main(int argc, char *argv[])
{
  char program_name[4096];
  strncpy(program_name, basename(argv[0]), 4095);
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

    if (strcmp(argv[0], "-ofbase_2pt") == 0)
    {
      ofbase_2pt = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-ofbase_4pt") == 0)
    {
      ofbase_4pt = argv[1];
      argc -= 2;
      argv += 2;
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
    }

    fprintf(stderr, "Error: Unknown option '%s'\n", argv[0]);
    exit(1);
  }

  if (argc < 2)
  {
    usage(program_name);
    exit(1);
  }

  int N = argc; // # of data files

  /**
   * @brief Do jackknife sampling and average 
   * 
   */
  if (ofbase_2pt)
  {
    VARRAY_DOUBLE sum(maxline), sum_square(maxline), out(maxline), var(maxline);
    sum = sum_square = out = var = 0.0;

    /**
     * @brief read the files for the first round to get the sum and sum^2,
     *        also the time reverse is done
     *
     */
    for (int i = 0; i < N; i++)
    {
      char *ifname = argv[i];
      VARRAY_COMPLEX tmp = read_complex_varray(ifname, maxline);

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
      char *ifname = argv[i];
      VARRAY_COMPLEX tmp = read_complex_varray(ifname, maxline);

      out = (sum - time_reverse(tmp, maxline)) / (N - 1.0);
      var = (sum_square - time_reverse(tmp, maxline) * time_reverse(tmp, maxline)) / double(N - 1);
      var = sqrt((var - out * out) / double(N - 2));

      write_double_varray(ifname, "sample", maxline, out);
    }
  }

//   if (ofbase_4pt)
//   {
//     maxline = maxline * maxline * maxline;

//     VARRAY_COMPLEX sum(maxline);
//     sum = 0.0;
//     VARRAY_COMPLEX out(maxline);
//     out = 0.0;

//     for (int i = 0; i < N; i++)
//     {
//       char *ifname = argv[i];
//       VARRAY_COMPLEX tmp = read_corr_complex(ifname, maxline);

//       sum += tmp;
//     }

//     for (int i = 0; i < N; i++)
//     {
//       char *ifname = argv[i];
//       VARRAY_COMPLEX tmp = read_corr_complex(ifname, maxline);

//       tmp = read_corr_complex(ifname, maxline);
//       out = (sum - tmp) / (N - 1.0);

//       write_corr_complex(ifname, "sample", maxline, out);
//     }
//   }

//   return 0;
// }

void usage(char *name)
{
  fprintf(stderr, "USAGE: \n"
                  "    %s [OPTIONS] ifname_1 ifname_2 [ifname_3...]\n",
          name);
  fprintf(stderr, "OPTIONS: \n"
                  "    -h, --help:             see usage\n"
                  "    -ofbase_2pt (basename): output directory for 2pt correlator\n"
                  "    -ofbase_4pt (basename): output directory for 4pt correlator\n"
                  "    -maxline (int maxline): 2pt: # of time sites; 4pt: # of space sites\n");
}