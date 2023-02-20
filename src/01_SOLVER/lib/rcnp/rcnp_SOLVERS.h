//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Tue Jul 28 12:24:18 JST 2020
 */
//---------------------------------------------------------------------

#include "bridge.h"
using Bridge::vout;

#include <mpi.h>
#include <libgen.h>

#include <rcnp/rcnp.h>


#ifndef IS_INCLUDED_RCNP_SOLVERS_H
#define IS_INCLUDED_RCNP_SOLVERS_H

namespace rcnp {
  extern void sink_smearing(double *ret,
			    const double *prop,
			    const double *func);



  extern void invoke_solvers(Field_G    *U,
			    double     *prop,
			    Parameters &params_fopr,
			    Parameters &params_solver,
			    Parameters &params_source);
  
  extern void invoke_solvers_eo(Field_G    *U,
			       double     *prop,
			       Parameters &params_fopr,
			       Parameters &params_solver,
			       Parameters &params_source);
  
  
  
  extern void invoke_solvers(Field_G    *U,
			    double     *prop,
			    Parameters &params_fopr,
			    Parameters &params_solver,
			    double     *func);
  
  extern void invoke_solvers_eo(Field_G    *U,
			       double     *prop,
			       Parameters &params_fopr,
			       Parameters &params_solver,
			       double     *func);


  
}
#endif

