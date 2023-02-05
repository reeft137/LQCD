/**
 * @file pre.cc
 * @author TC (reeft137@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-05
 *
 */

// C libraries
#include <stdio.h>
#include <stdlib.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"
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
                  "  MAN: \n"
                  "    [-h, --help]:       see usage\n"
                  "    [-ofname]:          output filename\n"
                  "  MUST: \n"
                  "    -maxline (maxline): 2pt: # of time sites; 4pt: # of space sites\n");
}

// Global variables to store the contents of options
static const char *ofname = NULL;
int maxline = 0;

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

    if (strcmp(argv[0], "-ofname") == 0)
    {
      ofname = argv[1];
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
  int N = argc; // # of data files

  VARRAY_COMPLEX out(maxline);
  out = 0.0;

  for (size_t i = 0; i < N; i++)
  {
    VARRAY_COMPLEX tmp(maxline);
    tmp = 0.0;

    read_bin(argv[i], maxline, tmp);
    out += tmp * 0.5;
  }

  write_bin(ofname, maxline, out);

  return 0;
}
