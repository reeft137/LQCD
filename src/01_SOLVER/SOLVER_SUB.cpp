//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Thu Apr 14 16:19:38 JST 2022
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include "bridge.h"


#include <rcnp/rcnp.h>

#include "SOLVER_SUB.h"



//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void bridge_prop_to_flat(const std::vector<Field_F> &bridge, double *flat)
{
  using namespace rcnp;

  for(int cd_I = 0; cd_I < 3*4; cd_I++){
    const double *src = (double*)bridge[cd_I].ptr(0);
    double       *dst = flat;

    for(int ixyzt = 0; ixyzt < XYZTnodeSites; ixyzt++){
      for(int ccd_F = 0; ccd_F < 2*3*4; ccd_F++){
	dst[ccd_F + 2*3*4*(cd_I + 3*4*ixyzt)] = src[ccd_F + 2*3*4*ixyzt];
      }
    } // for(int ixyzt = 0; ...)
  } // for(int cd_I = 0; ...)
}

