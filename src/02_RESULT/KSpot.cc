/**
 * @file KSpot.cc
 * @author TC (reeft137@gmail.com)
 * @brief Central potential:            (1/4)(3V_{v} + V_{ps})
 *        Spin-dependent potential:     V_{v} - V_{ps}
 * @version 0.1
 * @date 2023-02-02
 *
 */

// Custom libraries
#include <lqcd/misc.h>
#include <lqcd/dataio.h>

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
                  "    -v0/vs:                v0: central; vs: spin-dependent\n"
                  "    -spacelength (int):    # of space sites\n");
}

// Global variables to store the contents of options
static const char *ofname = NULL;
int spacelength = 0;
enum POTTYPE
{
  NONE = 0,
  V0 = 1,
  VS = 2
};
POTTYPE pot_type = NONE;

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

    if (strcmp(argv[0], "-v0") == 0)
    {
      pot_type = V0;
      argc--;
      argv++;
      continue;
    }

    if (strcmp(argv[0], "-vs") == 0)
    {
      pot_type = VS;
      argc--;
      argv++;
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

  if (argc != 2 || pot_type == NONE)
  {
    usage(program_name);
    exit(1);
  }

  // Initialization
  int maxline = spacelength * spacelength * spacelength;

  // Main part
  if (pot_type == V0)
  {
    COMPLX out[maxline], v[maxline], ps[maxline];
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
  }

  if (pot_type == VS)
  {
    COMPLX out[maxline], v[maxline], ps[maxline];
    for (int i = 0; i < maxline; i++)
    {
      out[i] = v[i] = ps[i] = 0.0;
    }

    read_bin(argv[0], maxline, v);
    read_bin(argv[1], maxline, ps);

    for (int i = 0; i < maxline; i++)
    {
      out[i] = v[i] - ps[i];
    }

    write_bin(ofname, maxline, out);
  }
  
  return 0;
}
