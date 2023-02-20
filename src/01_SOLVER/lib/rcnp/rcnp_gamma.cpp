//---------------------------------------------------------------------
/**
 * @file
 * @brief  gamma matrices and Levi-Civita symbol
 * @author Noriyoshi Ishii
 * @since  Tue Jun  9 15:15:25 UTC 2020
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <complex.h>
// #include <complex>

typedef double _Complex Complex;
// typedef std::complex<double> Complex;
// using namespace std::complex_literals;

#undef I
double tmp[2]={0.0,1.0}; Complex I = ((Complex*)tmp)[0];
// Complex I = _Complex_I;
// #define I 1i 
// Complex I(0.0,1.0);

#include "rcnp_gamma.h"

namespace rcnp {
  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  gamma::gamma(const gamma& other)
  {
    for(int i = 0; i < 4; i++){ a[i] = other.a[i]; }
    for(int i = 0; i < 4; i++){ b[i] = other.b[i]; }
  }
  
  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  gamma::gamma(int     i0, int     i1, int     i2, int     i3,
	       Complex r0, Complex r1, Complex r2, Complex r3)
  {
    b[0] = i0;
    b[1] = i1;
    b[2] = i2;
    b[3] = i3;
    
    a[0] = r0;
    a[1] = r1;
    a[2] = r2;
    a[3] = r3;
  }
  
  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  gamma& gamma::operator=(const gamma& other)
  {
    for(int i = 0; i < 4; i++){ a[i] = other.a[i]; }
    for(int i = 0; i < 4; i++){ b[i] = other.b[i]; }
    
    return *this;
  }
  
  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  gamma& gamma::operator*=(const gamma& other)
  {
    for(int i = 0; i < 4; i++){
      a[i] = a[i] * other.a[b[i]];
      b[i] = other.b[b[i]];
    }
    
    return *this;
  }
  
  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  gamma& gamma::transpose()
  {
    gamma self(*this);

    for(int j = 0; j < 4; j++){
      a[self.b[j]] = self.a[j];
      b[self.b[j]] = j;
    }

    return *this;
  }

  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  gamma& gamma::dagger()
  {
    transpose();
    for(int i = 0; i < 4; i++){
      a[i] = conj(a[i]);
    }

    return *this;
  }

  //---------------------------------------------------------------------
  /**
   * @brief Dirac conjugation: @f$A \mapsto \gamma_4 A^\dagger \gamma_4@f$
   */
  //---------------------------------------------------------------------
  gamma& gamma::dirac_conjugate()
  {
    dagger();
    (*this) = G[4] * (*this) * G[4];

    return *this;
  }


  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  // std::string gamma::to_string(const char* fmt) const
  // {
  //   typedef std::string string;
  //   char tmp[2048];

  //   string ret;
  //   for(  int i = 0; i < 4; i++){
  //     for(int j = 0; j < 4; j++){
	// // sprintf(tmp, fmt, __real__ (*this)(i,j), __imag__ (*this)(i,j));
	// sprintf(tmp, fmt, std::real(*this)(i,j), std::imag(*this)(i,j));
  // ret = ret + string(tmp) + string("\t");
  //     }
  //     ret += string("\n");
  //   }
  //   return ret;
  // }

  //---------------------------------------------------------------------
  // gamma matrices
  //---------------------------------------------------------------------
  gamma gamma::G[5]; //< generators (Euclidean)
  gamma gamma::G5;   //< gamma_5
  gamma gamma::C;    //< charge conjugation: C = gamma_4 * gamma_2
  gamma gamma::CG5;  //< C * gamma_5

  void gamma::initialize()
  {
    G[0] = gamma(0,   1,    2,    3,
		 1.0, 1.0, -1.0, -1.0);
    G[1] = gamma( 3,  2, 1, 0,
		  -I, -I, I, I);
    G[2] = gamma(3, 2, 1, 0,
		 -1.0, +1.0, +1.0, -1.0);
    G[3] = gamma(2, 3, 0, 1,
		 -I, +I, +I, -I);
    G[4] = G[0];
    G5   = gamma(2, 3, 0, 1,
		 -1.0, -1.0, -1.0, -1.0);
    C    = G[4]*G[2];
    CG5  = C*G5;
  };

  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  void gamma::finalize()
  {}


  //---------------------------------------------------------------------
  /**
   * Levi-Civita symbol
   */
  //---------------------------------------------------------------------
  int eps[6][4]={
    {0, 1, 2, 1},
    {1, 2, 0, 1},
    {2, 0, 1, 1},
    
    {0, 2, 1, -1},
    {2, 1, 0, -1},
    {1, 0, 2, -1}
  };
}


//---------------------------------------------------------------------
/**
 * for debugging
 */
//---------------------------------------------------------------------
#ifdef USE_AS_MAIN

int main(int argc, char* argv[])
{
  using namespace rcnp;

  gamma::initialize();

  printf("\n\n");

  for(int mu = 0; mu < 4; mu++){
    printf("mu = %d:\n", mu);
    printf("%s\n\n", gamma::G[mu].to_string().c_str());
  }

  printf("gamma5:\n");
  printf("%s\n\n", gamma::G5.to_string().c_str());
  
  printf("C:\n");
  printf("%s\n\n", gamma::C.to_string().c_str());

  printf("CG5:\n");
  printf("%s\n\n", gamma::CG5.to_string().c_str());

  printf("CG5^T:\n");
  printf("%s\n\n", transpose(gamma::CG5).to_string().c_str());

  gamma::finalize();
  return 0;
}
#endif
