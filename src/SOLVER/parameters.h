//---------------------------------------------------------------------
/**
 * @file
 * @brief  For a systematic management of many bridge++ parameters
 * @author Noriyoshi Ishii
 * @since  Thu Jun  4 03:05:56 UTC 2020
 */
//---------------------------------------------------------------------

#ifndef IS_INCLUDED_PARAMETERS_H_200604
#define IS_INCLUDED_PARAMETERS_H_200604

#include <string>
#include "bridge.h"
using Bridge::vout;

namespace parameters {
  extern Parameters    params_spectrum;
  extern Parameters    params_gfix;
  extern Parameters    params_fopr;
  extern Parameters    params_solver;
  extern Parameters    params_source;
  
  extern string        str_gconf_status;
  extern string        str_gconf_read;
  //extern string      readfile;
  extern string        str_rand_type;
  extern unsigned long seed;
  extern string        str_vlevel;

  extern string        str_gfix_type;
  extern string        str_fopr_type;
  extern string        str_gmset_type;
  extern string        str_solver_type;
  extern string        str_source_type;
  
  extern Bridge::VerboseLevel vl;

  extern void  initialize(const Parameters& params_all);
  extern void  finalize();
}

#endif
