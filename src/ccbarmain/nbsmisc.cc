/**
 * @file nbsmisc.cc
 * @author TC (reeft137@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-05
 *
 */

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libgen.h>

// C++ libraries
#include <complex>
#include <valarray>

// Custom libraries
#include "data_io.h"
#include "misc.h"

typedef std::complex<double> COMPLEX;
typedef std::valarray<double> VARRAY_DOUBLE;
typedef std::valarray<COMPLEX> VARRAY_COMPLEX;

// inline COMPLEX CORR(const COMPLEX* ptr, int ix, int iy, int iz, int Llength)
// {
//     ix = (ix + Llength) % Llength;
//     iy = (iy + Llength) % Llength;
//     iz = (iz + Llength) % Llength;

//     return ptr[ix + Llength*(iy + Llength*iz)];
// }

// inline COMPLEX CORR2(const COMPLEX* ptr, int ix, int iy, int iz, int Llength)
// {
//       return CORR(ptr, ix, iy, iz, Llength)
//         +    CORR(ptr, iy, iz, ix, Llength)
//         +    CORR(ptr, iz, ix, iy, Llength)

//         +    CORR(ptr, ix, iz, iy, Llength)
//         +    CORR(ptr, iz, iy, ix, Llength)
//         +    CORR(ptr, iy, ix, iz, Llength);
// }

//     COMPLEX raw[xyzSites];

//     // size_t len =
//     fread(raw, sizeof(COMPLEX), xyzSites, ifp); // Use "fread" to read data
//     // assert(len == xyzSites);

//     fclose(ifp);

//     // COMPLEX out[xyzSites];
//     std::valarray<COMPLEX> out(xyzSites); out = 1.0e128;

// #define Corr(ptr, x, y, z)   \
//     ((COMPLEX*)ptr)[((x)%Llength) + Llength*(((y)%Llength) + Llength*((z)%Llength))]

//     for (int ix = 0; ix < Llength; ix++){
//     for (int iy = 0; iy < Llength; iy++){
//     for (int iz = 0; iz < Llength; iz++){
//         Corr(&out[0], ix, iy, iz) = CORR2(raw,         ix,         iy,         iz, Llength)
//                                    +CORR2(raw,         ix,         iy, Llength-iz, Llength)
//                                    +CORR2(raw,         ix, Llength-iy,         iz, Llength)
//                                    +CORR2(raw,         ix, Llength-iy, Llength-iz, Llength)
//                                    +CORR2(raw, Llength-ix,         iy,         iz, Llength)
//                                    +CORR2(raw, Llength-ix,         iy, Llength-iz, Llength)
//                                    +CORR2(raw, Llength-ix, Llength-iy,         iz, Llength)
//                                    +CORR2(raw, Llength-ix, Llength-iy, Llength-iz, Llength);

//     }}}

//     //---------------------------------------------------------------------------------
//     /**
//      * @brief Add "A1." prefix to the output filename
//      *
//      */
//     //---------------------------------------------------------------------------------
//     char tmp[1024];
//     char dir[1024];
//     char base[1024];
//     char ofname[1024];

//     strncpy(tmp, ifname, 1023);
//     strncpy(dir, dirname(tmp), 1023);
//     strncpy(tmp, ifname, 1023);
//     strncpy(base, basename(tmp), 1023);

//     sprintf(ofname, "%s/A1.%s", dir, base);

//     //---------------------------------------------------------------------------------
//     /**
//      * @brief Write data to the txt file
//      *
//      */
//     //---------------------------------------------------------------------------------
//     FILE *ofp = NULL;
//     ofp = fopen(ofname, "w");

//     fwrite(&out[0], sizeof(COMPLEX), xyzSites, ofp);

//     fclose(ofp);

//     return 0;
// }

// #define Corr(ptr, x, y, z) ((COMPLEX *)ptr)[(x) + maxline * ((y) + maxline * (z))]

//     FILE *fp = NULL;
//     fp = fopen(corr_4pt, "rb");

//     COMPLEX raw[maxline3];

//     fread(raw, sizeof(COMPLEX), maxline3, fp); // Use "fread" to read data
//     fclose(fp);

//     int sum = 0;

//     for (int i = 0; i < maxline / 2 + 1; i++)
//       for (int j = i; j < maxline / 2 + 1; j++)
//         for (int k = j; k < maxline / 2 + 1; k++)
//         {
//           sum++;
//         }

//     double leng[sum];
//     double real[sum];
//     double imag[sum];

//     int index = 0;

//     for (int ix = 0; ix < maxline / 2 + 1; ix++)
//       for (int iy = ix; iy < maxline / 2 + 1; iy++)
//         for (int iz = iy; iz < maxline / 2 + 1; iz++)
//         {
//           double temp = ix * ix + iy * iy + iz * iz;
//           temp = pow(temp, 0.5);
//           leng[index] = temp;
//           real[index] = Corr(raw, ix, iy, iz).real();
//           imag[index] = Corr(raw, ix, iy, iz).imag();

//           index++;
//         }

//     FILE *ofp = NULL;
//     ofp = fopen("testresult.txt", "w");

//     for (int i = 0; i < index; i++)
//     {
//       fprintf(ofp, "%1.16e %1.16e\n", leng[i], real[i]);
//     }