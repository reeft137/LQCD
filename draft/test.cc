#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <libgen.h>
#include <complex>
#include <valarray>
#include <sys/stat.h>
#include <math.h>
// #include <sys/types.h>

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

int main(int argc, char *argv[])
{
  argc--;
  argv++;

  // VARRAY_COMPLEX test(3);
  // VARRAY_DOUBLE rtest(4);
  // test = 0.0;
  // rtest = 0.0;
  const char *test1 = "hello!";
  const char *test2 = "pre.";
  std::string cpptest1 = test1;
  std::string cpptest2 = test2;
  // printf("%lu\n", sizeof(test[0])/sizeof(double));

  std::string cpptest3 = cpptest2 + cpptest1;
  printf("%s\n", test1);
  std::cout << cpptest3 << "\n";

  // FILE* fp = NULL;
  // fp = fopen("./test.txt", "w");
  // if(fp == NULL)
  // {
  //   perror("./test.txt");
  //   exit(1);
  // }

  // fprintf(fp, "hello, world!");

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
  // for (int i = 0; i < 10; i++)
  // {
  //   data[i] = i;
  // }

  // int C = fwrite(data, sizeof(int), 20, fp);
  // fclose(fp);
  // printf("%d\n", C);

  // FILE *ofp = NULL;
  // ofp = fopen("wtest.txt", "rb");
  // int datadata[20];
  // for (int i = 0; i < 10; i++)
  // {
  //   datadata[i] = 0;
  // }

  // int D = fread(datadata, sizeof(int), 20, ofp);
  // fclose(ofp);
  // for (int i = 0; i < 20; i++)
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