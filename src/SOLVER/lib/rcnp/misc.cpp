//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Thu Mar 17 19:11:36 JST 2022
 */
//---------------------------------------------------------------------

#include "misc.h"

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void rcnp::change_endian(double *ptr, size_t len)
{
  int  size=sizeof(double);
  char tmp[size];

  for(size_t i = 0; i < len; i++){
    for(int j = 0; j < size; j++){
      tmp[j] = ((char*)&ptr[i])[size - j - 1];
    }
    ptr[i] = ((double*)tmp)[0];
  }
}

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void rcnp::reshape_123_to_132(double *dst, const double *src,
			     size_t size1,
			     size_t size2,
			     size_t size3)
{
  for(    size_t k = 0; k < size3; k++){
    for(  size_t j = 0; j < size2; j++){
      for(size_t i = 0; i < size1; i++){
	dst[    i + size1*(k + size3*(j))]
	  = src[i + size1*(j + size2*(k))];
      }
    }
  }
}
