/**
 * @file average.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#include <complex.h>
#include <valarray>

typedef double _Complex Complex;

int main(int argc, char *argv[])
{
    // Throw away the information of the command itself
    argc--; 
    argv++; 

    //---------------------------------------------------------------------------------
    /**
     * @brief Open file to read the data
     * 
     */
    //---------------------------------------------------------------------------------
    char *ifname1 = argv[1]; 
    char *ifname2 = argv[2]; 

    int Llength = atoi(argv[0]); // atoi(): convert ASCII string to integer
    int xyzSites = Llength * Llength * Llength; 

    std::valarray<Complex> in1(xyzSites); in1 = 0.0;
    std::valarray<Complex> in2(xyzSites); in2 = 0.0;
    std::valarray<Complex> out(xyzSites); out = 0.0;

    FILE *ifp = NULL; 
    ifp = fopen(ifname1, "rb");
    if(ifp == NULL) // Deal with wrong file name 
    {
        perror(ifname1);
        exit(1);
    }

    fread(&in1[0], sizeof(Complex), xyzSites, ifp); // Use "fread" to read data
    fclose(ifp);

    ifp = fopen(ifname2, "rb");
    if(ifp == NULL) // Deal with wrong file name 
    {
        perror(ifname2);
        exit(1);
    }

    fread(&in2[0], sizeof(Complex), xyzSites, ifp); // Use "fread" to read data
    fclose(ifp);

    out = (in1 + in2)/2;

    //---------------------------------------------------------------------------------
    /**
     * @brief Add "SUB." prefix to the output filename
     * 
     */
    //---------------------------------------------------------------------------------
    char tmp[1024]; 
    char dir[1024]; 
    char base[1024]; 
    char ofname[1024];

    strncpy(tmp, ifname1, 1023);
    strncpy(dir, dirname(tmp), 1023);
    strncpy(tmp, ifname1, 1023);
    strncpy(base, basename(tmp), 1023);

    sprintf(ofname, "%s/AVE.%s", dir, base);
    
    //---------------------------------------------------------------------------------
    /**
     * @brief Write data to the txt file
     * 
     */
    //---------------------------------------------------------------------------------
    FILE *ofp = NULL; 
    ofp = fopen(ofname, "w");

    fwrite(&out[0], sizeof(Complex), xyzSites, ofp);
    
    fclose(ofp);

    return 0; 
}

