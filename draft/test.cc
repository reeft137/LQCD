#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <complex>
#include <valarray>
#include <sys/stat.h>
#include <math.h>
#include <sys/types.h>

typedef std::valarray<double> VARRAY_DOUBLE;

int main(int argc, char *argv[])
{
  argc--;
  argv++;

  FILE *fp = NULL;
  fp = fopen("out.txt", "wb");
  int data[10];
  for (size_t i = 0; i < 10; i++)
  {
    data[i] = i;
  }

  int C = fwrite(data, sizeof(int), 20, fp);
  fclose(fp);
  printf("%d\n", C);

  FILE *ofp = NULL;
  ofp = fopen("wtest.txt", "rb");
  int datadata[20];
  for (size_t i = 0; i < 10; i++)
  {
    datadata[i] = 0;
  }

  int D = fread(datadata, sizeof(int), 20, ofp);
  fclose(ofp);
  for (size_t i = 0; i < 20; i++)
  {
    printf("%d\n", datadata[i]);
  }
  printf("%d\n", D);

  // ifp = fopen(argv[0], "r");
  // ofp = fopen(ofname, "w");

  // char content[20];
  // fscanf(ifp, "%s", content);
  // fprintf(ofp, "12323232132131, %s", content);

  return 0;
}