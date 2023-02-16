#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
typedef double _Complex Complex;

#include "qqbar.h"
#include <rcnp/rcnp_gamma.h>

void corr_twopoint_qqbar(double *corr,
                         const double *quark,
                         const double *antiquark,
                         const rcnp::gamma &gammaF,
                         const rcnp::gamma &gammaI,
                         int Lxyz,
                         int Lt)
{
#define Prop(ptr, c, alpha, cP, alphaP, ixyz, it) \
  ((Complex *)ptr)[c + 3 * (alpha + 4 * (cP + 3 * (alphaP + 4 * (ixyz + Lxyz * (it)))))]

  rcnp::gamma conj_gammaI(dirac_conjugate(gammaI));

  for (int it = 0; it < Lt; it++)
  {
    Complex sum = 0.0;

    for (int ixyz = 0; ixyz < Lxyz; ixyz++)
    {
      for (int betaP = 0; betaP < 4; betaP++)
      {
        for (int alpha = 0; alpha < 4; alpha++)
        {
          int beta = gammaF.b[alpha];
          int alphaP = conj_gammaI.b[betaP];
          for (int cP = 0; cP < 3; cP++)
          {
            for (int c = 0; c < 3; c++)
            {
              sum += gammaF.a[alpha] * conj_gammaI.a[betaP] * Prop(quark, c, beta, cP, betaP, ixyz, it) * Prop(antiquark, c, alpha, cP, alphaP, ixyz, it);
            }
          }
        }
      }
    } // for(int ixyz = 0; ...)
    ((Complex *)corr)[it] = sum;
  } // for(int it = 0; ...)

#undef Prop
}

void corr_fourpoint_qqbar(double *corr,
                          const double *quark,
                          const double *antiquark,
                          const rcnp::gamma &gammaF,
                          const rcnp::gamma &gammaI,
                          int Lxyz,
                          int Lt)
{
#define Prop(ptr, cF, alphaF, cI, alphaI, ixyz, it) \
  ((Complex *)ptr)[cF + 3 * (alphaF + 4 * (cI + 3 * (alphaI + 4 * (ixyz + Lxyz * (it)))))]
#define FourPoint(ptr, ixyz, it) \
  ((Complex *)ptr)[ixyz + Lxyz * (it)]

rcnp::gamma conj_gammaI(dirac_conjugate(gammaI));

    for (int it = 0; it < Lt; it++)
    {
      for (int ixyz = 0; ixyz < Lxyz; ixyz++)
      {
        Complex sum = 0.0;

        for (int alphaI = 0; alphaI < 4; alphaI++)
        {
          for (int betaF = 0; betaF < 4; betaF++)
          {
            int alphaF = gammaF.b[betaF];
            int betaI = conj_gammaI.b[alphaI];
            for (int cI = 0; cI < 3; cI++)
            {
              for (int cF = 0; cF < 3; cF++)
              {
                sum += gammaF.a[betaF] * conj_gammaI.a[alphaI] * Prop(quark, cF, alphaF, cI, alphaI, ixyz, it) * Prop(antiquark, cF, betaF, cI, betaI, ixyz, it);
              }
            }
          } // for(int betaF = 0; ...)
        }   // for(int alphaI = 0;...)

        FourPoint(corr, ixyz, it) = sum;

      } // for(int ixyz = 0; ...)
    }   // for(int it = 0; ...)

#undef FourPoint
#undef Prop
}