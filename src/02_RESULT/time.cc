/**
 * @file time.cc
 * @author TC (reeft137@gmail.com)
 * @brief Main program to calculate the temporal corr_ofname and masses
 * @version 0.1
 * @date 2023-02-02
 *
 */

// Custom libraries
#include "lib/misc.h"
#include "lib/dataio.h"
#include "lib/jknife.h"

// Usage function
void usage(char *name)
{
  fprintf(stderr, "USAGE: \n"
                  "    %s [OPTIONS] ifname_1 ifname_2 [ifname_3...]\n",
          name);
  fprintf(stderr, "OPTIONS: \n"
                  "    -timelength (int):     total # of time sites\n"
                  "    -corr (ofname):        output file name of correlator\n"
                  "    -fitmass (ofname):     output file name of mass fitted\n"
                  "    -fitrange (min max):  do fit and set the range for fitting mass\n"
                  "    [-h, --help]:          see usage\n");
}

// __________________________________
//     .________|______|________.
//     |                        |
//     |    Custom functions    |
//     |________________________|

// Time reversal
void time_reverse_2pt(char *datalist[], char *r_datalist[], int timelength, int totalfile);

// Effective mass
void exp_mass(char *datalist[], char *r_datalist[], int timelength, int totalfile);
void csh_mass(char *datalist[], char *r_datalist[], int timelength, int totalfile);

// __________________________________
//     .________|______|________.
//     |                        |
//     |    Global Variables    |
//     |________________________|

int timelength = 0;
static const char *corr_ofname = NULL;
static const char *fitmass_ofname = NULL;
int fitmin = 0;
int fitmax = 0;

// __________________________________
//     .________|______|________.
//     |                        |
//     |      Main Function     |
//     |________________________|

int main(int argc, char *argv[])
{
  char program_name[2048];
  strncpy(program_name, basename(argv[0]), 2047);
  argc--;
  argv++;
  // __________________________________
  //     .________|______|________.
  //     |                        |
  //     |  Dealing with Options  |
  //     |________________________|

  while (argc > 0 && argv[0][0] == '-') // deal with all options regardless of their order
  {
    // -h and --help options and syntax check
    if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0)
    {
      usage(program_name);
      exit(0);
    }

    if (strcmp(argv[0], "-corr") == 0)
    {
      corr_ofname = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-fitmass") == 0)
    {
      fitmass_ofname = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-fitrange") == 0)
    {
      fitmin = atoi(argv[1]);
      fitmax = atoi(argv[2]);
      if (!fitmin || !fitmax || fitmin > fitmax || fitmin == fitmax)
      {
        usage(program_name);
        exit(1);
      }
      argc -= 3;
      argv += 3;
      continue;
    }

    if (strcmp(argv[0], "-timelength") == 0)
    {
      timelength = atoi(argv[1]); // atoi(): convert ASCII string to integer
      if (!timelength)
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
  int totalfile = argc; // # of data files

  // .________________________________.
  // |                                |
  // |           Main Body            |
  // |________________________________|
  //     .________|______|________.
  //     |                        |
  //     |     2pt and effmass    |
  //     |________________________|

  if (corr_ofname)
  {
    // Create some string arrays for temparory file names (time reversed, jackknife, exp mass, csh mass)
    char *tr_tmp_datalist[totalfile], *js_tmp_datalist[totalfile], *exp_tmp_datalist[totalfile], *csh_tmp_datalist[totalfile];
    for (int i = 0; i < totalfile; i++)
    {
      tr_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char)); // malloc: allocate memory for a pointer
      add_prefix(argv[i], "tr", tr_tmp_datalist[i]);
      js_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char)); // malloc: allocate memory for a pointer
      add_prefix(argv[i], "js", js_tmp_datalist[i]);
      exp_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char)); // malloc: allocate memory for a pointer
      add_prefix(argv[i], "exp", exp_tmp_datalist[i]);
      csh_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char)); // malloc: allocate memory for a pointer
      add_prefix(argv[i], "csh", csh_tmp_datalist[i]);
    }

    // __________________________________
    //     .________|______|________.
    //     |                        |
    //     |    TR and Jackknife    |
    //     |________________________|

    time_reverse_2pt(argv, tr_tmp_datalist, totalfile, timelength);

    jackknife_resample(tr_tmp_datalist, js_tmp_datalist, timelength, totalfile);
    for (int i = 0; i < totalfile; i++) // Remove time reversed temporary files
      rm(tr_tmp_datalist[i]);

    char tmp_result[4096];
    add_prefix(corr_ofname, "tmp", tmp_result);
    jackknife_average(js_tmp_datalist, tmp_result, timelength, totalfile, DOUBLE_LINE);

    bin2txt(tmp_result, corr_ofname, timelength);
    rm(tmp_result); // Remove result temporary file

    // __________________________________
    //     .________|______|________.
    //     |                        |
    //     |     Effective mass     |
    //     |________________________|

    char expm_tmp[4096], cshm_tmp[4096], expm[4096], cshm[4096];
    add_prefix(corr_ofname, "expmass", expm);
    add_prefix(corr_ofname, "cshmass", cshm);
    add_prefix(corr_ofname, "etmp", expm_tmp);
    add_prefix(corr_ofname, "ctmp", cshm_tmp);

    exp_mass(js_tmp_datalist, exp_tmp_datalist, timelength, totalfile);
    csh_mass(js_tmp_datalist, csh_tmp_datalist, timelength, totalfile);

    jackknife_average(exp_tmp_datalist, expm_tmp, timelength, totalfile, DOUBLE_LINE);
    jackknife_average(csh_tmp_datalist, cshm_tmp, timelength, totalfile, DOUBLE_LINE);
    for (int i = 0; i < totalfile; i++) // Remove exp effmass temporary files
    {
      rm(exp_tmp_datalist[i]);
      rm(csh_tmp_datalist[i]);
    }

    bin2txt(expm_tmp, expm, timelength);
    bin2txt(cshm_tmp, cshm, timelength);
    rm(expm_tmp); // Remove temporary file
    rm(cshm_tmp);

    // Finalization for the string arrays
    for (int i = 0; i < totalfile; i++)
    {
      free(tr_tmp_datalist[i]);
      free(js_tmp_datalist[i]);
      free(exp_tmp_datalist[i]);
      free(csh_tmp_datalist[i]);
    }
  }

  // Fit the mass and get the final result
  if (fitmass_ofname && fitmin && fitmax)
  {
    fprintf(stderr, "Fitting under construction!\n");
  }
  else
  {
    fprintf(stderr, "Fitting skipped!\n");
  }

  return 0;
}

// __________________________________
//     .________|______|________.
//     |                        |
//     |  Custom Functions Def  |
//     |________________________|

void time_reverse_2pt(char *datalist[], char *r_datalist[], int timelength, int totalfile)
{
  for (int i = 0; i < totalfile; i++)
  {
    CVARRAY data;
    data = 0.0;
    read_bin(datalist[i], timelength, data);

    for (int i = 0; i < timelength / 2 + 1; i++)
    {
      data[i] = (data[i] + data[(timelength - i) % timelength]) * 0.5;
    }

    write_bin(r_datalist[i], timelength, data);
  }
}

void exp_mass(char *datalist[], char *r_datalist[], int timelength, int totalfile)
{
  for (int i = 0; i < totalfile; i++)
  {
    CVARRAY raw(timelength);
    raw = 0.0;
    read_bin(datalist[i], timelength, raw);

    DVARRAY data(timelength), effmass(timelength);
    data = effmass = 0.0;

    keep_real(raw, data, timelength);

    for (int i = 0; i < timelength / 2 + 1; i++)
    {
      effmass[i] = log(data[i] / data[(i + 1) % timelength]);
    }

    write_bin(r_datalist[i], timelength, data);
  }
}

DOUBLE coshtype_mass(int t1, int t2, DOUBLE corr1, DOUBLE corr2, int timelength)
{
#define JMAX 4000
#define M0 0.001
#define M1 10.0
#define MACC 1.0e-12
#define coshtype(m) corr1 / corr2 - cosh(m * (timelength / 2.0 - t1)) / cosh(m * (timelength / 2.0 - t2))

  DOUBLE dm, f, fmid, mmid, mass;

  f = coshtype(M0);
  fmid = coshtype(M1);
  if (f * fmid >= 0.0)
  {
    fprintf(stderr, "Root must be bracketed for bisection in RTBIS");
    exit(1);
  }
  mass = f < 0.0 ? (dm = M1 - M0, M0) : (dm = M0 - M1, M1);
  for (int j = 1; j <= JMAX; j++)
  {
    mmid = mass + (dm *= 0.5);
    fmid = coshtype(mmid);
    if (fmid <= 0.0)
      mass = mmid;
    if (fabs(dm) < MACC || fmid == 0.0)
      return mass;
  }
  fprintf(stderr, "Too many bisections in RTBIS");
  return 0.0;
}

void csh_mass(char *datalist[], char *r_datalist[], int timelength, int totalfile)
{
  for (int i = 0; i < totalfile; i++)
  {
    CVARRAY raw(timelength);
    raw = 0.0;
    read_bin(datalist[i], timelength, raw);

    DVARRAY data(timelength), effmass(timelength);
    data = effmass = 0.0;

    keep_real(raw, data, timelength);

    for (int i = 0; i < timelength / 2 + 1; i++)
    {
      int t1 = i;
      int t2 = (i + 1) % timelength;
      effmass[i] = coshtype_mass(t1, t2, data[t1], data[t2], timelength);
    }

    write_bin(r_datalist[i], timelength, data);
  }
}