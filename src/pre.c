/**
 * @file pre.c
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
#include <libgen.h>
#include <complex.h>

void gen_print_name(const char *ifname, char *ofname);
void read_bin(const char *fname, int maxline, double complex *data);
void write_bin(const char *fname, int maxline, const double complex *data);
void usage(char *name);

// Global variables to store the contents of options
static const char *ofname = NULL;
int maxline = 0;

int main(int argc, char *argv[])
{
  char program_name[2048];
  strncpy(program_name, basename(argv[0]), 2047);
  argc--;
  argv++;

  // deal with all options regardless of their order
  while (argc > 0 && argv[0][0] == '-') 
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

  // Initialization
  int N = argc;
  double complex Nc = argc;

  double complex out[maxline];
  for (size_t i = 0; i < maxline; i++)
  {
    out[i] = 0.0;
  }

  for (size_t i = 0; i < N; i++)
  {
    double complex tmp[maxline];
    for (size_t j = 0; j < maxline; j++)
    {
      tmp[j] = 0.0;
    }

    read_bin(argv[i], maxline, tmp);

    for (size_t j = 0; j < maxline; j++)
    {
      out[j] += tmp[j] / Nc;
    }
  }

  write_bin(ofname, maxline, out);

  return 0;
}

// Function definition 
void gen_print_name(const char *ifname, char *ofname)
{
  char stmp[2048], dir[2048], base[2048];
  // dirname() and basename() will change the original string, 
  // so we need to copy from ifname twice.
  strncpy(stmp, ifname, 2047);
  strncpy(dir, dirname(stmp), 2047);
  strncpy(stmp, ifname, 2047);
  strncpy(base, basename(stmp), 2047);
  snprintf(ofname, 2047, "%s/%s", dir, base);
}

void read_bin(const char *fname, int maxline, double complex *data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(fname, "rb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fread(data, sizeof(double complex), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const double complex *data)
{
  char pname[2048];
  gen_print_name(fname, pname);
  fprintf(stderr, "Writing data to file   '%s'... \n", pname);

  FILE *fp = fopen(fname, "wb");
  if (fp == NULL)
  {
    perror(fname);
    exit(1);
  }

  fwrite(data, sizeof(double complex), maxline, fp);
  fclose(fp);
}

void usage(char *name)
{
  fprintf(stderr, "USAGE: \n"
                  "    %s [OPTIONS] ifname_1 [ifname_2] \n",
          name);
  fprintf(stderr, "OPTIONS: \n"
                  "  MAN: \n"
                  "    [-h, --help]:          see usage\n"
                  "    [-ofname] (filename):  output filename\n"
                  "  MUST: \n"
                  "    -maxline (maxline):    \n");
}