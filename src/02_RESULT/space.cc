/**
 * @file space.cc
 * @author TC (reeft137@gmail.com)
 * @brief Main program to calculate the spacial correlator and "pre-potential".
 *        Also, this program can change the result from Cartesian coordinate to spherical cooridinate.
 * @version 0.1
 * @date 2023-02-02
 *
 */

// Custom libraries
#include <lqcd/misc.h>
#include <lqcd/dataio.h>
#include <lqcd/jknife.h>

// Usage function
void usage(char *name)
{
  fprintf(stderr, "USAGE: \n"
                  "    %s [OPTIONS] ifname_1 ifname_2 [ifname_3...]\n",
          name);
  fprintf(stderr, "OPTIONS: \n"
                  "    -spacelength (int):  total# of space sites\n"
                  "    -corr (ofname):      output file name of correlator\n"
                  "    -sphout (ofname):    output file name for spherical coor results\n"
                  "    [-h, --help]:        see usage\n");
}

// __________________________________
//     .________|______|________.
//     |                        |
//     |    Custom functions    |
//     |________________________|

// Get the value of correlator on a specific spacial site
COMPLX &correlator(COMPLX *data, int x, int y, int z, int spacelength);

// Simplification for the A1+ projection
COMPLX sphere_sym(COMPLX *data, int x, int y, int z, int spacelength);
COMPLX a1_sym(COMPLX *data, int x, int y, int z, int spacelength);

// A1+ projection main
void a1_plus(char *rawdlist[], char *resultlist[], int spacelength, int N_totalfile);
// L2 normalization
void normalization(char *rawdlist[], char *resultlist[], int spacelength, int N_totalfile);
// Discrete laplacian
void laplacian(char *rawdlist[], char *resultlist[], int spacelength, int N_totalfile);
// Change to spherical coordinate
void cartesian_to_spherical(const char *ifname, const char *ofname, int spacelength);

// Create dir and delete files
inline void mk755dir(const char *dir)
{
  if (mkdir(dir, 0000755))
    perror(dir);
}

inline void rm(const char *dir)
{
  if (remove(dir))
    perror(dir);
}

// __________________________________
//     .________|______|________.
//     |                        |
//     |    Global Variables    |
//     |________________________|

int spacelength = 0;
static const char *corr_ofname = NULL;
static const char *sph_ofname = NULL;

// __________________________________
//     .________|______|________.
//     |                        |
//     |      Main Function     |
//     |________________________|

int main(int argc, char *argv[])
{
  char program_name[1024];
  strncpy(program_name, basename(argv[0]), 1023);
  argc--;
  argv++;
  // __________________________________
  //     .________|______|________.
  //     |                        |
  //     |  Dealing with Options  |
  //     |________________________|

  while (argc > 0 && argv[0][0] == '-')
  {
    // -h and --help options and syntax check
    if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0)
    {
      usage(program_name);
      exit(0);
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

    if (strcmp(argv[0], "-corr") == 0)
    {
      corr_ofname = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-sphout") == 0)
    {
      sph_ofname = argv[1];
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
  int maxline = int(pow(spacelength, 3));
  const int N_totalfile = argc; // total # of files
  fprintf(stderr, "##################################################\n");
  fprintf(stderr, "# Total of data files: %d\n", N_totalfile);
  fprintf(stderr, "# # of space sites:    %d\n", spacelength);
  fprintf(stderr, "##################################################\n\n");

  // .________________________________.
  // |                                |
  // |           Main Body            |
  // |________________________________|
  //     .________|______|________.
  //     |                        |
  //     |    4pt and laplacian   |
  //     |________________________|

  if (corr_ofname)
  {
    // Create some string arrays for temparory file names (A1+, L2, jackknife, laplacian)
    char *a1_tmp_datalist[N_totalfile], *l2_tmp_datalist[N_totalfile], *js_tmp_datalist[N_totalfile], *lap_tmp_datalist[N_totalfile];
    for (int i = 0; i < N_totalfile; i++)
    {
      a1_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char)); // malloc: allocate memory for a pointer
      add_prefix(argv[i], "A1+", a1_tmp_datalist[i]);
      l2_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char));
      add_prefix(argv[i], "n2", l2_tmp_datalist[i]);
      js_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char));
      add_prefix(argv[i], "js", js_tmp_datalist[i]);
      lap_tmp_datalist[i] = (char *)malloc(4096 * sizeof(char));
      add_prefix(argv[i], "lap", lap_tmp_datalist[i]);
    }

    // __________________________________
    //     .________|______|________.
    //     |                        |
    //     |     A1+ projection     |
    //     |________________________|

    a1_plus(argv, a1_tmp_datalist, spacelength, N_totalfile);

    // __________________________________
    //     .________|______|________.
    //     |                        |
    //     |    L2 and Jackknife    |
    //     |________________________|

    normalization(a1_tmp_datalist, l2_tmp_datalist, spacelength, N_totalfile);

    jackknife_resample(l2_tmp_datalist, js_tmp_datalist, maxline, N_totalfile);
    for (int i = 0; i < N_totalfile; i++) // Remove L2 normalization temporary files
      rm(l2_tmp_datalist[i]);

    char tmp_result[4096];
    add_prefix(corr_ofname, "tmp", tmp_result);
    jackknife_average(js_tmp_datalist, tmp_result, maxline, N_totalfile, DOUBLE_LINE);
    for (int i = 0; i < N_totalfile; i++) // Remove JS resample temporary files
      rm(js_tmp_datalist[i]);

    cartesian_to_spherical(tmp_result, corr_ofname, spacelength);
    rm(tmp_result); // Remove result temporary file

    // __________________________________
    //     .________|______|________.
    //     |                        |
    //     |   Discrete Laplacian   |
    //     |________________________|

    jackknife_resample(a1_tmp_datalist, js_tmp_datalist, maxline, N_totalfile);
    for (int i = 0; i < N_totalfile; i++) // Remove A1+ projection temporary files
      rm(a1_tmp_datalist[i]);

    laplacian(js_tmp_datalist, lap_tmp_datalist, spacelength, N_totalfile);
    for (int i = 0; i < N_totalfile; i++) // Remove JS resample temporary files
      rm(js_tmp_datalist[i]);

    char lap_tmp_result[4096];
    add_prefix(corr_ofname, "laptmp", lap_tmp_result);
    jackknife_average(lap_tmp_datalist, lap_tmp_result, maxline, N_totalfile, DOUBLE_LINE);

    char lap_result[4096];
    add_prefix(corr_ofname, "lap", lap_result);
    cartesian_to_spherical(lap_tmp_result, lap_result, spacelength);
    rm(lap_tmp_result); // Remove result temporary file

    fprintf(stderr, "Finished! \n\n");

    // Finalization for the string arrays
    for (int i = 0; i < N_totalfile; i++)
    {
      free(a1_tmp_datalist[i]);
      free(l2_tmp_datalist[i]);
      free(js_tmp_datalist[i]);
      free(lap_tmp_datalist[i]);
    }
  }

  // __________________________________
  //     .________|______|________.
  //     |                        |
  //     |       Cart to sph      |
  //     |________________________|

  if (sph_ofname)
  {
    // __________________________________
    //     .________|______|________.
    //     |                        |
    //     |        Jackknife       |
    //     |________________________|

    char tmp_result[4096];
    add_prefix(sph_ofname, "tmp", tmp_result);
    jackknife_average(argv, tmp_result, maxline, N_totalfile, DOUBLE_LINE);

    cartesian_to_spherical(tmp_result, sph_ofname, spacelength);
    rm(tmp_result);

    fprintf(stderr, "Finished! \n\n");
  }

  return 0;
}

// __________________________________
//     .________|______|________.
//     |                        |
//     |  Custom Functions Def  |
//     |________________________|

inline COMPLX &correlator(COMPLX *data, int x, int y, int z, int spacelength)
{
  x = (x + spacelength) % spacelength;
  y = (y + spacelength) % spacelength;
  z = (z + spacelength) % spacelength;
  COMPLX &corr_r = data[x + spacelength * (y + spacelength * z)];
  return corr_r;
}

inline COMPLX sphere_sym(COMPLX *data, int x, int y, int z, int spacelength)
{
  return (correlator(data, x, y, z, spacelength) + correlator(data, y, z, x, spacelength) + correlator(data, z, x, y, spacelength) + correlator(data, x, z, y, spacelength) + correlator(data, z, y, x, spacelength) + correlator(data, y, x, z, spacelength)) / 6.0;
}

inline COMPLX a1_sym(COMPLX *data, int x, int y, int z, int spacelength)
{
  return (sphere_sym(data, x, y, z, spacelength) + sphere_sym(data, x, y, spacelength - z, spacelength) + sphere_sym(data, x, spacelength - y, z, spacelength) + sphere_sym(data, x, spacelength - y, spacelength - z, spacelength) + sphere_sym(data, spacelength - x, y, z, spacelength) + sphere_sym(data, spacelength - x, y, spacelength - z, spacelength) + sphere_sym(data, spacelength - x, spacelength - y, z, spacelength) + sphere_sym(data, spacelength - x, spacelength - y, spacelength - z, spacelength)) / 8.0;
}

void a1_plus(char *rawdlist[], char *resultlist[], int spacelength, int N_totalfile)
{
  int maxline = int(pow(spacelength, 3));

  for (int i = 0; i < N_totalfile; i++)
  {
    COMPLX tmp[maxline], result[maxline];
    for (int j = 0; j < maxline; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    read_bin(rawdlist[i], maxline, tmp);

    for (int ix = 0; ix < spacelength; ix++)
      for (int iy = 0; iy < spacelength; iy++)
        for (int iz = 0; iz < spacelength; iz++)
        {
          correlator(result, ix, iy, iz, spacelength) = a1_sym(tmp, ix, iy, iz, spacelength);
        }

    write_bin(resultlist[i], maxline, result);
  }
}

void normalization(char *rawdlist[], char *resultlist[], int spacelength, int N_totalfile)
{
  int maxline = int(pow(spacelength, 3));

  for (int i = 0; i < N_totalfile; i++)
  {
    COMPLX tmp[maxline], result[maxline];
    for (int j = 0; j < maxline; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    read_bin(rawdlist[i], maxline, tmp);

    DOUBLE norm = 0.0;
    for (int j = 0; j < maxline; j++)
    {
      norm += sqrt((tmp[j] * std::conj(tmp[j])).real());
    }

    for (int j = 0; j < maxline; j++)
    {
      result[j] = tmp[j] / norm;
    }

    write_bin(resultlist[i], maxline, result);
  }
}

void laplacian(char *rawdlist[], char *resultlist[], int spacelength, int N_totalfile)
{
  int maxline = int(pow(spacelength, 3));

  for (int i = 0; i < N_totalfile; i++)
  {
    COMPLX tmp[maxline], result[maxline];
    for (int j = 0; j < maxline; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    read_bin(rawdlist[i], maxline, tmp);

    for (int ix = 0; ix < spacelength; ix++)
      for (int iy = 0; iy < spacelength; iy++)
        for (int iz = 0; iz < spacelength; iz++)
        {
          correlator(result, ix, iy, iz, spacelength) = (correlator(tmp, ix + 1, iy, iz, spacelength) + correlator(tmp, ix - 1, iy, iz, spacelength) + correlator(tmp, ix, iy + 1, iz, spacelength) + correlator(tmp, ix, iy - 1, iz, spacelength) + correlator(tmp, ix, iy, iz + 1, spacelength) + correlator(tmp, ix, iy, iz - 1, spacelength) - 6.0 * correlator(tmp, ix, iy, iz, spacelength)) / correlator(tmp, ix, iy, iz, spacelength);
        }

    write_bin(resultlist[i], maxline, result);
  }
}

void cartesian_to_spherical(const char *ifname, const char *ofname, int spacelength)
{
  int maxline = pow(spacelength, 3);

  COMPLX tmp[maxline];
  for (int j = 0; j < maxline; j++) // Initialize the empty arrays
  {
    tmp[j] = 0.0;
  }
  read_bin(ifname, maxline, tmp);

  char pname[2048];
  gen_print_name(ofname, pname);
  fprintf(stderr, "Writing data to '%s'... \n", pname);

  FILE *fp = fopen(ofname, "w");
  if (fp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  for (int i = 0; i < spacelength / 2 + 1; i++)
    for (int j = i; j < spacelength / 2 + 1; j++)
      for (int k = j; k < spacelength / 2 + 1; k++)
      {
        DOUBLE value, variance, distance = 0.0;

        distance = sqrt(pow(DOUBLE(i), 2) + pow(DOUBLE(j), 2) + pow(DOUBLE(k), 2));
        value = correlator(tmp, i, j, k, spacelength).real();
        variance = correlator(tmp, i, j, k, spacelength).imag();

        fprintf(fp, "%1.16e %1.16e %1.16e\n", distance, value, variance);
      }

  fclose(fp);
}
