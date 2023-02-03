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

  for (int i = 0; i < argc; i++)
  {
    printf("%s\n", argv[i]);
  }

  printf("*****-----*****-----*****-----*****-----*****-----*****-----*****-----*****-----*****");
  char **tmp = NULL;
  tmp = argv;

  for (int i = 0; i < argc; i++)
  {
    printf("%s\n", tmp[i]);
  }
  // ifp = fopen(argv[0], "r");
  // ofp = fopen(ofname, "w");

  // char content[20];
  // fscanf(ifp, "%s", content);
  // fprintf(ofp, "12323232132131, %s", content);

  return 0;
}