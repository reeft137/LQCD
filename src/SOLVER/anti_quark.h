//---------------------------------------------------------------------
/**
 * @file
 * @brief  Propagator for "anti-quark"
 * @author Noriyoshi Ishii
 * @since  Mon Jun  8 10:16:14 UTC 2020
 */
//---------------------------------------------------------------------
#ifndef IS_INCLUDED_ANTI_QUARK_H
#define IS_INCLUDED_ANTI_QUARK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>

#include <rcnp/rcnp_gamma.h>

typedef double _Complex Complex;

namespace rcnp {
  extern void anti_quark_propagator(const double *quark, double *antiquark, int Lxyzt);
}
#endif
