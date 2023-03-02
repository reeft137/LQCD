/**
 * @file data_io.cc
 * @author TC (reeft137@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-17
 *
 */

#include "dataio.h"

void read_bin(const char *datafile, int maxline, DOUBLE *data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(datafile, "rb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fread(data, sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}
void read_bin(const char *datafile, int maxline, COMPLX *data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(datafile, "rb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fread(data, sizeof(COMPLX), maxline, fp);
  fclose(fp);
}
void read_bin(const char *datafile, int maxline, DVARRAY &data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(datafile, "rb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fread(&data[0], sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}
void read_bin(const char *datafile, int maxline, CVARRAY &data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Reading data from file '%s'... \n", pname);

  FILE *fp = fopen(datafile, "rb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fread(&data[0], sizeof(COMPLX), maxline, fp);
  fclose(fp);
}

void write_bin(const char *datafile, int maxline, const DOUBLE *data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Writing data to file   '%s'... \n", pname);

  FILE *fp = fopen(datafile, "wb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fwrite(data, sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}
void write_bin(const char *datafile, int maxline, const COMPLX *data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Writing data to file   '%s'... \n", pname);

  FILE *fp = fopen(datafile, "wb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fwrite(data, sizeof(COMPLX), maxline, fp);
  fclose(fp);
}
void write_bin(const char *datafile, int maxline, const DVARRAY &data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Writing data to file   '%s'... \n", pname);

  FILE *fp = fopen(datafile, "wb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fwrite(&data[0], sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}
void write_bin(const char *datafile, int maxline, const CVARRAY &data)
{
  char pname[2048];
  gen_print_name(datafile, pname);
  fprintf(stderr, "Writing data to file   '%s'... \n", pname);

  FILE *fp = fopen(datafile, "wb");
  if (fp == NULL)
  {
    perror(datafile);
    exit(1);
  }

  fwrite(&data[0], sizeof(COMPLX), maxline, fp);
  fclose(fp);
}

void bin2txt(const char *namebin, const char *nametxt, int maxline)
{
  char ipname[2048];
  gen_print_name(namebin, ipname);
  fprintf(stderr, "Reading data from file '%s'... \n", ipname);

  COMPLX data[maxline];
  for (int i = 0; i < maxline; i++)
  {
    data[i] = 0.0;
  }

  FILE *ifp = fopen(namebin, "rb");
  if (ifp == NULL)
  {
    perror(namebin);
    exit(1);
  }

  fread(data, sizeof(COMPLX), maxline, ifp);
  fclose(ifp);

  char opname[2048];
  gen_print_name(nametxt, opname);
  fprintf(stderr, "Writing data to file   '%s'... \n", opname);

  FILE *ofp = fopen(nametxt, "w");
  if (ofp == NULL)
  {
    perror(nametxt);
    exit(1);
  }

  for (int i = 0; i < maxline; i++)
  {
    fprintf(ofp, "%d %1.16e %1.16e\n", i, data[i].real(), data[i].imag());
  }
  fclose(ofp);
}

void keep_real(CVARRAY &data, DVARRAY &realdata, int maxline)
{
  for (int i = 0; i < maxline; i++)
  {
    realdata[i] = data[i].real();
  }
}

void keep_imag(CVARRAY &data, DVARRAY &imagdata, int maxline)
{
  for (int i = 0; i < maxline; i++)
  {
    imagdata[i] = data[i].imag();
  }
}
