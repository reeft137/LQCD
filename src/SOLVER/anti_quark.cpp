//---------------------------------------------------------------------
/**
 * @file
 * @brief  Propagator for "anti-quark"
 * @author Noriyoshi Ishii
 * @since  Mon Jun  8 10:24:15 UTC 2020
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>

#include "anti_quark.h"
#include <rcnp/rcnp_gamma.h>

typedef double _Complex Complex;

//---------------------------------------------------------------------
/**
 * @brief @f$\langle q(x)\bar q(y)\rangle \Rightarrow \langle \bar q(x) q(y)\rangle @f$
 */
//---------------------------------------------------------------------
void rcnp::anti_quark_propagator(const double *quark, double *antiquark, int Lxyzt)
{
#define Prop(ptr, c, alpha, cP, alphaP, ixyzt) \
  ((Complex *)ptr)[c + 3 * (alpha + 4 * (cP + 3 * (alphaP + 4 * (ixyzt))))]

  const gamma &gamma5 = rcnp::gamma::G5;

  for (int ixyzt = 0; ixyzt < Lxyzt; ixyzt++)
  {
    for (int alpha = 0; alpha < 4; alpha++)
    {
      for (int betaP = 0; betaP < 4; betaP++)
      {
        int beta = gamma5.b[alpha];
        int alphaP = gamma5.b[betaP];
        for (int cP = 0; cP < 3; cP++)
        {
          for (int c = 0; c < 3; c++)
          {
            Prop(antiquark, cP, alphaP, c, alpha, ixyzt) =
                -gamma5.a[alpha] * gamma5.a[betaP] * conj(Prop(quark, cP, betaP, c, beta, ixyzt));
          } // for(int c  = 0; ...)
        }   // for(int cP = 0; ...)
      }     // for(int betaP  = 0; ...)
    }       // for(int alpha  = 0; ...)
  }         // for(int ixyzt = 0; ...)

#undef Prop
}
