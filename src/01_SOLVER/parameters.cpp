//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Thu Jun  4 03:15:50 UTC 2020
 */
//---------------------------------------------------------------------

#include <string>
#include "bridge.h"
#include "parameters.h"
using Bridge::vout;

Parameters    parameters::params_spectrum;
Parameters    parameters::params_gfix;
Parameters    parameters::params_fopr;
Parameters    parameters::params_solver;
Parameters    parameters::params_source;

string        parameters::str_gconf_status;
string        parameters::str_gconf_read;
//string      parameters::readfile;
string        parameters::str_rand_type;
unsigned long parameters::seed;
string        parameters::str_vlevel;

string        parameters::str_gfix_type;
string        parameters::str_fopr_type;
string        parameters::str_gmset_type;
string        parameters::str_solver_type;
string        parameters::str_source_type;

Bridge::VerboseLevel parameters::vl;

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void parameters::initialize(const Parameters& params_all)
{
  params_spectrum  = params_all.lookup("Spectrum");
  params_gfix      = params_all.lookup("GaugeFixing");
  params_fopr      = params_all.lookup("Fopr");
  params_solver    = params_all.lookup("Solver");
  params_source    = params_all.lookup("Source");

  str_gconf_status = params_spectrum.get_string("gauge_config_status");
  str_gconf_read   = params_spectrum.get_string("gauge_config_type_input");
  //readfile       = params_spectrum.get_string("config_filename_input");
  str_rand_type    = params_spectrum.get_string("random_number_type");
  seed             = params_spectrum.get_unsigned_long("seed_for_random_number");
  str_vlevel       = params_spectrum.get_string("verbose_level");

  str_gfix_type    = params_gfix.get_string("gauge_fixing_type");
  str_fopr_type    = params_fopr.get_string("fermion_type");
  str_gmset_type   = params_fopr.get_string("gamma_matrix_type");
  str_solver_type  = params_solver.get_string("solver_type");
  str_source_type  = params_source.get_string("source_type");

  vl = vout.set_verbose_level(str_vlevel);

  //- print input parameters
  vout.general(vl, "Measurement parameters\n");
  vout.general(vl, "  gconf_status = %s\n", str_gconf_status.c_str());
  vout.general(vl, "  gconf_read   = %s\n", str_gconf_read.c_str());
  //vout.general(vl, "  readfile     = %s\n", readfile.c_str());
  vout.general(vl, "  rand_type    = %s\n", str_rand_type.c_str());
  vout.general(vl, "  seed         = %lu\n", seed);
  vout.general(vl, "  vlevel       = %s\n", str_vlevel.c_str());
  vout.general(vl, "  gfix_type    = %s\n", str_gfix_type.c_str());
  vout.general(vl, "  gmset_type   = %s\n", str_gmset_type.c_str());
  vout.general(vl, "  solver_type  = %s\n", str_solver_type.c_str());
  vout.general(vl, "  source_type  = %s\n", str_source_type.c_str());

  //- input parameter check
  int err = 0;
  err += ParameterCheck::non_NULL(str_gconf_status);

  if (err) {
    vout.crucial(vl, "Error in yaml file: input parameters have not been set\n");
    exit(EXIT_FAILURE);
  }

  if (str_solver_type == "CG") {
    vout.crucial(vl, "Error in yaml file: CG can not be adopted. Use CGNE,CGNR, instead.\n");
    exit(EXIT_FAILURE);
  }
}

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void parameters::finalize()
{
}

