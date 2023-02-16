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
void rcnp::change_endian(double *ptr, int len)
{
  int  size=sizeof(double);
  char tmp[size];

  for(int i = 0; i < len; i++){
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
			     int size1,
			     int size2,
			     int size3)
{
  for(    int k = 0; k < size3; k++){
    for(  int j = 0; j < size2; j++){
      for(int i = 0; i < size1; i++){
	dst[    i + size1*(k + size3*(j))]
	  = src[i + size1*(j + size2*(k))];
      }
    }
  }
}
