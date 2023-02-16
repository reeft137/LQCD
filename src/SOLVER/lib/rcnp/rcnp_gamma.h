//---------------------------------------------------------------------
/**
 * @file
 * @brief  gamma matrices and Levi-Civita symbol
 * @author Noriyoshi Ishii
 * @since  Tue Jun  9 15:02:17 UTC 2020
 */
//---------------------------------------------------------------------

#ifndef IS_INCLUDED_RCNP_GAMMA_H
#define IS_INCLUDED_RCNP_GAMMA_H

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
// #include <complex>
#include <string>

typedef double _Complex Complex;
// typedef std::complex<double> Complex;

namespace rcnp {
  struct gamma;

  extern int eps[6][4];
}

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
struct rcnp::gamma {
  Complex a[4];
  int     b[4];
  
  ~gamma(){}
  gamma(){}
  gamma(const gamma& other);
  gamma(int     i0, int     i1, int     i2, int     i3,
	Complex r0, Complex r1, Complex r2, Complex r3);

  gamma& operator=(const gamma& other);
  gamma& operator*=(const gamma& other);
  gamma& transpose();
  gamma& dagger();
  gamma& dirac_conjugate();

  Complex operator()(int i, int j) const
  { return (j==b[i])? a[i]:0.0; }

  std::string to_string(const char* fmt="(%g %g)") const;

  static gamma G[5];
  static gamma G5;
  static gamma C;
  static gamma CG5;
  static void  initialize();
  static void  finalize();
};

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
inline rcnp::gamma operator*(rcnp::gamma lhs, const rcnp::gamma& rhs)
{ return lhs *= rhs; }

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
inline rcnp::gamma transpose(rcnp::gamma gam)
{ return gam.transpose(); }

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
inline rcnp::gamma dagger(rcnp::gamma gam)
{ return gam.dagger(); }

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
inline rcnp::gamma dirac_conjugate(rcnp::gamma gam)
{ return gam.dirac_conjugate(); }

#endif
