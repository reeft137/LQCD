#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <complex>
#include <valarray>
#include <sys/stat.h>
#include <math.h>
#include <sys/types.h>

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// COMPLEX CORR(const COMPLEX *data, int xsite, int ysite, int zsite, int spacelength)
// {
//   int x = (xsite + spacelength) % spacelength;
//   int y = (ysite + spacelength) % spacelength;
//   int z = (zsite + spacelength) % spacelength;
//   return data[x + spacelength * (y + spacelength * z)];
// }

// inline COMPLEX &TEST(COMPLEX *data, int index)
// {
//   return data[index];
// }

int main(int argc, char *argv[])
{
  argc--;
  argv++;

  using namespace std::complex_literals;

  double data[4] = {1.0, 2.0, 3.0, 4.0};
  // data[0] = data[0] / 4.0;

  double *out_d = NULL;
  double *out_c = NULL;

  out_d = &data[1];
  // out_c = &data[1];
  out_c = (double *)&((COMPLEX *)data)[1];

  fprintf(stderr, "(%f, %f), (%f, %f)\n", data[0], data[1], data[3], data[4]);
  fprintf(stderr, "out_d: %p\nout_c: %p\n", out_d, out_c);
  fprintf(stderr, "out_d: %f\nout_c: %f\n", *out_d, *out_c);
  // fprintf(stderr, "data[0]: (%f, %fi)\n", data[0].real(), data[0].imag());
  // fprintf(stderr, "data[1]: (%f, %fi)\n", data[1].real(), data[1].imag());

  // int out = 0.0;
  // for (size_t i = 0; i < 17; i++)
  // for (size_t j = i; j < 17; j++)
  // for (size_t k = j; k < 17; k++)
  // {
  //   out ++;
  // }

  // COMPLEX test_array[3];

  // fprintf(stderr, "data[0]: (%f, %fi)\n", data[0].real(), data[0].imag());
  // fprintf(stderr, "data[1]: (%f, %fi)\n", data[1].real(), data[1].imag());

  // int xsite = 31;
  // int ysite = 12;
  // int zsite = 23;

  // int spacelength = 32;

  // int index = xsite + spacelength * (ysite + spacelength * zsite);
  // int x, y, z = 0;

  // x = index % spacelength;
  // index = (index - x) / spacelength;
  // y = index % spacelength;
  // index = (index - y) / spacelength;
  // z = index % spacelength;

  // double distance = sqrt(pow(double(x), 2) + pow(double(y), 2) + pow(double(z), 2));

  // FILE *fp = NULL;
  // fp = fopen("out.txt", "wb");
  // int data[10];
  // for (size_t i = 0; i < 10; i++)
  // {
  //   data[i] = i;
  // }

  // int C = fwrite(data, sizeof(int), 20, fp);
  // fclose(fp);
  // printf("%d\n", C);

  // FILE *ofp = NULL;
  // ofp = fopen("wtest.txt", "rb");
  // int datadata[20];
  // for (size_t i = 0; i < 10; i++)
  // {
  //   datadata[i] = 0;
  // }

  // int D = fread(datadata, sizeof(int), 20, ofp);
  // fclose(ofp);
  // for (size_t i = 0; i < 20; i++)
  // {
  //   printf("%d\n", datadata[i]);
  // }
  // printf("%d\n", D);

  // ifp = fopen(argv[0], "r");
  // ofp = fopen(ofname, "w");

  // char content[20];
  // fscanf(ifp, "%s", content);
  // fprintf(ofp, "12323232132131, %s", content);

  return 0;
}