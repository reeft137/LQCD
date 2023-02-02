#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <complex>
#include <valarray>
#include <sys/stat.h>
#include <sys/types.h>

typedef std::valarray<double> VARRAY_DOUBLE;

int main()
{
  VARRAY_DOUBLE sum(64), sum_square(64), out(64), var(64);
  sum = sum_square = out = var = 0.0;

  if (mkdir("testdir", 0777))
    perror("testdir");
  if (chmod("testdir", 0000744))
    perror("testdir");

    printf("dfdfdf""dfdfdfd");

  // printf("%.1f %.1f\n", real(z1), imag(z1));

  return 0;
}