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

  double _Complex data, denominator;
  __real__ data = 10.0;
  __imag__ data = 8.0;

  int N = 8;
  __real__ denominator = N;
  __imag__ denominator = 0.0;

  fprintf(stdout, "(%f, %fi)\n", __real__ denominator, __imag__ denominator);

  data = data / denominator;

  fprintf(stdout, "(%f, %fi)\n", __real__ data, __imag__ data);

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