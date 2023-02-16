//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Thu Mar 17 19:08:14 JST 2022
 */
//---------------------------------------------------------------------

#ifndef IS_INCLUDED_MISC_H_220317
#define IS_INCLUDED_MISC_H_220317

#include <stddef.h>

namespace rcnp {
  extern void change_endian(double *ptr, size_t len);
  extern void reshape_123_to_132(double *dst, const double *src,
				 size_t size1, size_t size2, size_t size3);
}


#endif
