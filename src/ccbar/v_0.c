/**
 * @file v_0.c
 * @author TC (reeft137@gmail.com)
 * @brief 1/4 * (3V_{v} + V_{ps})
 * @version 0.1
 * @date 2023-02-02
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

// Usage function
void usage(char *name)
{
  fprintf(stderr, "USAGE: \n"
                  "    %s [OPTIONS] V_{v} V_{ps} \n",
          name);
  fprintf(stderr, "OPTIONS: \n"
                  "  MAN: \n"
                  "    [-h, --help]:          see usage\n"
                  "  MUST: \n"
                  "    -ofname (filename):    output filename\n"
                  "    -spacelength (int):    # of space sites\n");
}

// Global variables to store the contents of options
static const char *ofname = NULL;
int spacelength = 0;

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

    if (strcmp(argv[0], "-spacelength") == 0)
    {
      spacelength = atoi(argv[1]); // atoi(): convert ASCII string to integer
      if (!spacelength)
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

  if (argc != 2)
  {
    usage(program_name);
    exit(1);
  }

  // Initialization
  int maxline = spacelength * spacelength * spacelength;

  // Main part
  double complex out[maxline], v[maxline], ps[maxline];
  for (int i = 0; i < maxline; i++)
  {
    out[i] = v[i] = ps[i] = 0.0;
  }

  read_bin(argv[0], maxline, v);
  read_bin(argv[1], maxline, ps);

  for (int i = 0; i < maxline; i++)
  {
    out[i] = (3.0 * v[i] + ps[i]) / 4.0;
  }

  write_bin(ofname, maxline, out);

  return 0;
}

// Function definition
void gen_print_name(const char *ifname, char *ofname)
{
  char stmp[1024], dir[1024], base[1024];
  // dirname() and basename() will change the original string,
  // so we need to copy from ifname twice.
  strncpy(stmp, ifname, 1023);
  strncpy(dir, dirname(stmp), 1023);
  strncpy(stmp, ifname, 1023);
  strncpy(base, basename(stmp), 1023);
  snprintf(ofname, 2048, "%s/%s", dir, base);
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
