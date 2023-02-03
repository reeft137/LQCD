//---------------------------------------------------------------------
/**
 * @file effmassh.cc
 * @author Tianchen (zhangtc@rcnp.osaka-u.ac.jp)
 * @brief Calculate the effective mass of the cosh type
 * @version 0.1
 * @date 2021-11-10
 * 
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <math.h>
#include <valarray>

#include "data_io.h"

typedef std::valarray<double> VAL; 

void usage();

//---------------------------------------------------------------------
/**
 * @brief fortran used, to be developed 
 * 
 */
//---------------------------------------------------------------------
extern "C" {
    double rtbis_(double (*func)(double&), double &x1, double &x2, double &xacc);
    double func(double& m);
}

static double g0;
static double g1;
static double t0;
static double t1;
static int    Tsites;

double func(double &m)
{
  return g0/g1 - cosh(m*(Tsites/2.0 - t0))/cosh(m*(Tsites/2.0 - t1));
}

inline double rtbis(double (*f)(double&), double x1, double x2, double xacc)
{
    return rtbis_(f, x1, x2, xacc);
}

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
double cosh_type(int t0, int t1, double g0, double g1, int Tsites)
{
    ::t0     = t0;
    ::t1     = t1;
    ::g0     = g0;
    ::g1     = g1;
    ::Tsites = Tsites;

     return rtbis(func, 0.001, 10.0, 1.0e-12);
}

int main(int argc, char *argv[])
{
    argc--; 
    argv++;

    // -h and --help options and syntax check
    if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0 || argc != 2) 
    {
        usage();
        exit(1);
    }

    int Tsites = atoi(argv[0]);
    if (Tsites == 0)
    {
        usage();
        exit(1);
    }

    char *ifname = argv[1]; 

    VAL effmass(Tsites); effmass = 0.0; 
    VAL data = val_read(ifname, Tsites); 

    for (int i = 0; i < Tsites; i++)
    {
        int t1 = i;
        int t2 = (i+1)%Tsites;
        effmass[i] = cosh_type(t1, t2, data[t1], data[t2], Tsites);
    }
    
    char tmp[1024]; 
    char dir[1024]; 
    char base[1024]; 
    char ofname[1024]; 

    strncpy(tmp, ifname, 1023);
    strncpy(dir, dirname(tmp), 1023);
    strncpy(tmp, ifname, 1023);
    strncpy(base, basename(tmp), 1023);

    sprintf(ofname, "%s/effmassh.%s", dir, base);

    val_write_s(ofname, Tsites, effmass);
    
    return 0;
}

void usage()
{
    printf("usage: effmassh [Tsites] (filename))\n");
}
