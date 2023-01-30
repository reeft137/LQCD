//---------------------------------------------------------------------
/**
 * @file bin2txt.cc
 * @author Tianchen (zhangtc@rcnp.osaka-u.ac.jp)
 * @brief Transfer binary files to text files.
 *        Only applicable to the files storing complex double numbers,
 *        which in my case, I use it for the correlation function data
 * @version 0.1
 * @date 2022-01-16
 *
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <valarray>
#include <string.h>

typedef double _Complex Complex;
typedef std::valarray<Complex> VAL;

void usage(char *);

//---------------------------------------------------------------------
/**
 * @brief global variables to store the contents of options
 *
 */
//---------------------------------------------------------------------
static const char *ifname = NULL;
int maxrow = 0;

int main(int argc, char *argv[])
{
  char program_name[2048]; 
  strncpy(program_name, basename(argv[0]), 2047);
  argc--;
  argv++;

  // -h and --help options and syntax check
  if (argc < 2 || strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0)
  {
    usage(program_name);
    exit(1);
  }

  maxrow = atoi(argv[0]); // atoi(): convert ASCII string to integer
  if (!maxrow)
  {
    usage(program_name);
    exit(1);
  }
  argc--;
  argv++;

  ifname = argv[0];
  char dir[2048];
  char base[2048];
  strncpy(dir, dirname(argv[0]), 2047);
  strncpy(base, basename(argv[0]), 2047);
  argc--;
  argv++;

  fprintf(stderr, "reading %s\n", ifname); // print the input file name (print to the stderr file)
  FILE *ifp = NULL;
  ifp = fopen(ifname, "rb");
  if (ifp == NULL) // Deal with wrong file name
  {
    perror(ifname);
    exit(1);
  }

  VAL data(maxrow);
  data = 0.0;

  fread(&data[0], sizeof(Complex), maxrow, ifp); // Use "fread" to read data
  fclose(ifp);

  char ofname[2048];
  sprintf(ofname, "%s/txt.%s", dir, base);

  fprintf(stderr, "generating %s\n", ofname); // print the output file name (print to the stderr file)

  FILE *ofp = NULL;
  ofp = fopen(ofname, "w");
  if (ofp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  for (int i = 0; i < maxrow; i++)
  {
    fprintf(ofp, "%d  %1.16e  %1.16e\n", i, __real__ data[i], __imag__ data[i]);
  }

  fclose(ofp);
  
  return 0;
}

void usage(char *name)
{
  fprintf(stderr, "SYNOPSIS\n"); 
  fprintf(stderr, "    %s maxrow ifname\n", name);
  fprintf(stderr, "DESCRIPTION\n"); 
  fprintf(stderr, "    maxrow: number of the rows of the data\n");
}