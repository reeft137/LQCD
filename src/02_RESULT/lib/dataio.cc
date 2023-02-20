/**
 * @file data_io.cc
 * @author TC (reeft137@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-17
 *
 */

#include "dataio.h"

void read_bin(const char *fname, int maxline, DOUBLE *data)
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

  fread(data, sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}

void read_bin(const char *fname, int maxline, COMPLX *data)
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

  fread(data, sizeof(COMPLX), maxline, fp);
  fclose(fp);
}

void read_bin(const char *fname, int maxline, DVARRAY &data)
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

  fread(&data[0], sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}

void read_bin(const char *fname, int maxline, CVARRAY &data)
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

  fread(&data[0], sizeof(COMPLX), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const DOUBLE *data)
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

  fwrite(data, sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const COMPLX *data)
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

  fwrite(data, sizeof(COMPLX), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const DVARRAY &data)
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

  fwrite(&data[0], sizeof(DOUBLE), maxline, fp);
  fclose(fp);
}

void write_bin(const char *fname, int maxline, const CVARRAY &data)
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

  fwrite(&data[0], sizeof(COMPLX), maxline, fp);
  fclose(fp);
}

void bin2txt(const char *ifname, const char *ofname, int maxline)
{
  char ipname[2048];
  gen_print_name(ifname, ipname);
  fprintf(stderr, "Reading data from file '%s'... \n", ipname);

  COMPLX data[maxline];
  for (int i = 0; i < maxline; i++)
  {
    data[i] = 0.0;
  }

  FILE *ifp = fopen(ifname, "rb");
  if (ifp == NULL)
  {
    perror(ifname);
    exit(1);
  }

  fread(data, sizeof(COMPLX), maxline, ifp);
  fclose(ifp);

  char opname[2048];
  gen_print_name(ofname, opname);
  fprintf(stderr, "Writing data to file   '%s'... \n", opname);

  FILE *ofp = fopen(ofname, "w");
  if (ofp == NULL)
  {
    perror(ofname);
    exit(1);
  }

  for (int i = 0; i < maxline; i++)
  {
    data[i] = 0.0;
    fprintf(ofp, "%d %1.16e %1.16e\n", i, data[i].real(), data[i].imag());
  }
  fclose(ofp);
}

void keep_real(CVARRAY &data, DVARRAY &rdata, int maxline)
{
  for (int i = 0; i < maxline; i++)
  {
    rdata[i] = data[i].real();
  }
}

void keep_imag(CVARRAY &data, DVARRAY &idata, int maxline)
{
  for (int i = 0; i < maxline; i++)
  {
    idata[i] = data[i].imag();
  }
}
