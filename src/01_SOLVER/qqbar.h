//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Tianchen
 * @since  Thu Apr 21 17:15:56 JST 2022
 */
//---------------------------------------------------------------------

#ifndef IS_INCLUDED_QQBAR_H_220421
#define IS_INCLUDED_QQBAR_H_220421

#include <complex.h>
#include <rcnp/rcnp_gamma.h>

typedef double _Complex Complex;

// extern void anti_quark_propagator(const double *quark, double *antiquark, int Lxyzt);

extern void corr_twopoint_qqbar(double *corr, 
                                const double *quark, 
                                const double *antiquark, 
                                const rcnp::gamma &gammaF, 
                                const rcnp::gamma &gammaI, 
                                int Lxyz, 
                                int Lt);

extern void corr_fourpoint_qqbar(double *corr,
                                 const double *quark,
                                 const double *antiquark,
                                 const rcnp::gamma &gammaF,
                                 const rcnp::gamma &gammaI,
                                 int Lxyz,
                                 int Lt);
#endif
