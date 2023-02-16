//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Thu Apr 14 16:31:22 JST 2022
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "bridge.h"

#ifndef IS_INCLUDED_SOLVER_H
#define IS_INCLUDED_SOLVER_H

extern void bridge_prop_to_flat(const std::vector<Field_F> &bridge, double *flat);

#endif
