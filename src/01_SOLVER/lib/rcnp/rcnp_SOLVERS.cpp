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
#include <complex>

typedef std::complex<double> Complex;

#include <rcnp/rcnp.h>
#include <fft3d/fft3d.h>
#include "rcnp_SOLVERS.h"

static void solver_convert_bridge_to_flat(const std::vector<Field_F> &bridge, double *flat);
static void source_set(Field_F &b, int icd, const double *func);

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void rcnp::sink_smearing(double       *dst,
			 const double *prop,
			 const double *func)
{
  using namespace rcnp;

  Complex *prop_fft = (Complex*)dst;
  Complex *func_fft = new Complex[XYZnodeSites];

  for(int i = 0; i < 2 *3*4 *3*4 *XYZnodeSites *TnodeSites; i++){
    ((double*)prop_fft)[i] = ((double*)prop)[i];
  }
  for(int i = 0; i < 2 *XYZnodeSites; i++){
    ((double*)func_fft)[i] = ((double*)func)[i];
  }

  fft3d::run((double*)prop_fft, 3*4 *3*4, TnodeSites, fft3d::Forward);
  fft3d::run((double*)func_fft, 1,        1,          fft3d::Forward);

  for(  int it   = 0; it   < TnodeSites;   it++  ){
    for(int ixyz = 0; ixyz < XYZnodeSites; ixyz++){
      for(int icdcd = 0; icdcd < 3*4 *3*4; icdcd++){
	prop_fft[icdcd + 3*4*3*4*(ixyz + XYZnodeSites*(it))]
	  *= func_fft[ixyz];
      }
    }
  }
  fft3d::run((double*)prop_fft, 3*4 *3*4, TnodeSites, fft3d::Backward);  

  for(int i = 0; i < 2 *3*4 *3*4 *XYZnodeSites *TnodeSites; i++){
    dst[i] /= double(XYZsites);
  }

  delete[] func_fft;
}



//---------------------------------------------------------------------
// solver invokers
//---------------------------------------------------------------------

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void rcnp::invoke_solvers(Field_G    *U,
			  double     *prop,
			  Parameters &params_fopr,
			  Parameters &params_solver,
			  Parameters &params_source)
{
  const unique_ptr<Timer> timer(new Timer(string("solver")));
  timer->start();

  // parameters
  const int Nc   = CommonParameters::Nc();
  const int Nd   = CommonParameters::Nd();
  const int Ndim = CommonParameters::Ndim();
  const int Nvol = CommonParameters::Nvol();

  string str_fopr_type    = params_fopr  .get_string("fermion_type"     );
  string str_gmset_type   = params_fopr  .get_string("gamma_matrix_type");
  string str_solver_type  = params_solver.get_string("solver_type"      );
  string str_source_type  = params_source.get_string("source_type"      );
  
  std::vector<Field_F> sq(Nc * Nd);
  for (int i_cd = 0; i_cd < Nc * Nd; ++i_cd) {
    sq[i_cd].set(0.0);
  }
  
  unique_ptr<GammaMatrixSet> gmset(GammaMatrixSet::New(str_gmset_type));
  
  unique_ptr<Fopr> fopr(Fopr::New(str_fopr_type, str_gmset_type));
  fopr->set_parameters(params_fopr);
  fopr->set_config(U);
  
  unique_ptr<Solver> solver(Solver::New(str_solver_type, fopr));
  solver->set_parameters(params_solver);
  
  const unique_ptr<Fprop> fprop_lex(new Fprop_Standard_lex(solver));
  
  const unique_ptr<Source> source(Source::New(str_source_type));
  source->set_parameters(params_source);
  
  vout.general("\n");
  vout.general("Solving quark propagator:\n");
  vout.general("  color spin   Nconv      diff           diff2\n");
  
  for (int ispin = 0; ispin < Nd; ++ispin) {
    for (int icolor = 0; icolor < Nc; ++icolor) {
      int i_cd = icolor + Nc * ispin;
      
      Field_F b;    // b.set(0.0);
      source->set(b, i_cd);
      
      int    Nconv;
      double diff;
      fprop_lex->invert_D(sq[i_cd], b, Nconv, diff);
      
      Field_F y(b);
      fopr->set_mode("D");
      fopr->mult(y, sq[i_cd]);   // y  = fopr->mult(sq[i_cd]);
      axpy(y, -1.0, b);          // y -= b;
      double diff2 = y.norm2() / b.norm2();
      
      vout.general("   %2d   %2d   %6d   %12.4e   %12.4e\n",
		   icolor, ispin, Nconv, diff, diff2);
    }
  }
  solver_convert_bridge_to_flat(sq, prop);
  timer->report();
}

//---------------------------------------------------------------------
/**
 * @brief eo version
 * 
 * cf) tests/src/Spectrum/test_Spectrum_2ptFunction_eo.cpp
 */
//---------------------------------------------------------------------
void rcnp::invoke_solvers_eo(Field_G    *U,
			     double     *prop,
			     Parameters &params_fopr,
			     Parameters &params_solver,
			     Parameters &params_source)
{
  const unique_ptr<Timer> timer(new Timer(string("solver_eo")));
  timer->start();

  // ####  parameter setup  ####
  const int Nc   = CommonParameters::Nc();
  const int Nd   = CommonParameters::Nd();
  const int Ndim = CommonParameters::Ndim();
  const int Nvol = CommonParameters::Nvol();

  //
  string str_fopr_type   = params_fopr  .get_string("fermion_type");
  string str_gmset_type  = params_fopr  .get_string("gamma_matrix_type");
  string str_solver_type = params_solver.get_string("solver_type");
  string str_source_type = params_source.get_string("source_type");

  std::vector<Field_F> sq(Nc * Nd);
  for(int i_cd = 0; i_cd < Nc * Nd; i_cd++){
    sq[i_cd].set(0.0);
  }

  // ####  object setup  #####
  unique_ptr<GammaMatrixSet> gmset(GammaMatrixSet::New(str_gmset_type));

  unique_ptr<Fopr> fopr(Fopr::New(str_fopr_type, str_gmset_type));
  fopr -> set_parameters(params_fopr);
  fopr -> set_config(U);

  unique_ptr<Fopr> fopr_eo(Fopr::New(str_fopr_type + "_eo", str_gmset_type));
  fopr_eo -> set_parameters(params_fopr);
  fopr_eo -> set_config(U);

  unique_ptr<Solver> solver(Solver::New(str_solver_type, fopr_eo));
  solver -> set_parameters(params_solver);

  const unique_ptr<Fprop> fprop_eo(new Fprop_Standard_eo(solver));

  const unique_ptr<Source> source(Source::New(str_source_type));
  source -> set_parameters(params_source);

  vout.general("\n");
  vout.general("Solving quark propagator:\n");
  vout.general("  color spin   Nconv      diff          diff2\n");



  for (  int ispin  = 0; ispin  < Nd; ++ispin)  {
    for (int icolor = 0; icolor < Nc; ++icolor) {
      int i_cd = icolor + Nc * ispin;
      
      Field_F b;  // b.set(0.0);
      source -> set(b, i_cd);
      
      int    Nconv;
      double diff;
      fprop_eo -> invert_D(sq[i_cd], b, Nconv, diff);
      
      Field_F y(b);
      fopr -> set_mode("D");
      fopr -> mult(y, sq[i_cd]); // y  = fopr[iq]->mult(sq[iq][i_cd]);
      axpy(y, -1.0, b);          // y -= b;
      double diff2 = y.norm2() / b.norm2();
      
      vout.general("   %2d   %2d   %6d   %12.4e   %12.4e\n",
		     icolor, ispin, Nconv, diff, diff2);
    }
  }
  solver_convert_bridge_to_flat(sq, prop);
  timer -> report();
}




//---------------------------------------------------------------------
//
//---------------------------------------------------------------------

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void rcnp::invoke_solvers(Field_G    *U,
			  double     *prop,
			  Parameters &params_fopr,
			  Parameters &params_solver,
			  double     *func)
{
  const unique_ptr<Timer> timer(new Timer(string("solver")));
  timer->start();

  // parameters
  const int Nc   = CommonParameters::Nc();
  const int Nd   = CommonParameters::Nd();
  const int Ndim = CommonParameters::Ndim();
  const int Nvol = CommonParameters::Nvol();

  string str_fopr_type    = params_fopr  .get_string("fermion_type"     );
  string str_gmset_type   = params_fopr  .get_string("gamma_matrix_type");
  string str_solver_type  = params_solver.get_string("solver_type"      );
  
  std::vector<Field_F> sq(Nc * Nd);
  for (int i_cd = 0; i_cd < Nc * Nd; ++i_cd) {
    sq[i_cd].set(0.0);
  }
  
  unique_ptr<GammaMatrixSet> gmset(GammaMatrixSet::New(str_gmset_type));
  
  unique_ptr<Fopr> fopr(Fopr::New(str_fopr_type, str_gmset_type));
  fopr->set_parameters(params_fopr);
  fopr->set_config(U);
  
  unique_ptr<Solver> solver(Solver::New(str_solver_type, fopr));
  solver->set_parameters(params_solver);
  
  const unique_ptr<Fprop> fprop_lex(new Fprop_Standard_lex(solver));
  
  vout.general("\n");
  vout.general("Solving quark propagator:\n");
  vout.general("  color spin   Nconv      diff           diff2\n");
  
  MPI_Barrier(MPI_COMM_WORLD);

  for (int ispin = 0; ispin < Nd; ++ispin) {
    for (int icolor = 0; icolor < Nc; ++icolor) {
      int i_cd = icolor + Nc * ispin;
      
      MPI_Barrier(MPI_COMM_WORLD);
      vout.general("@@@ step %d\n", 0);
      MPI_Barrier(MPI_COMM_WORLD);

      Field_F b;    // b.set(0.0);

      MPI_Barrier(MPI_COMM_WORLD);
      vout.general("@@@ step %d\n", 1);
      MPI_Barrier(MPI_COMM_WORLD);

      source_set(b, i_cd, func);
   //source->set(b, i_cd);
      
      MPI_Barrier(MPI_COMM_WORLD);
      vout.general("@@@ step %d\n", 2);
      MPI_Barrier(MPI_COMM_WORLD);


      int    Nconv;
      double diff;
      fprop_lex->invert_D(sq[i_cd], b, Nconv, diff);
      
      Field_F y(b);
      fopr->set_mode("D");
      fopr->mult(y, sq[i_cd]);   // y  = fopr->mult(sq[i_cd]);
      axpy(y, -1.0, b);          // y -= b;
      double diff2 = y.norm2() / b.norm2();
      
      vout.general("   %2d   %2d   %6d   %12.4e   %12.4e\n",
		   icolor, ispin, Nconv, diff, diff2);
    }
  }
  solver_convert_bridge_to_flat(sq, prop);
  timer->report();
}

//---------------------------------------------------------------------
/**
 * @brief eo version
 * 
 * cf) tests/src/Spectrum/test_Spectrum_2ptFunction_eo.cpp
 */
//---------------------------------------------------------------------
void rcnp::invoke_solvers_eo(Field_G    *U,
			     double     *prop,
			     Parameters &params_fopr,
			     Parameters &params_solver,
			     double     *func)
{
  const unique_ptr<Timer> timer(new Timer(string("solver_eo")));
  timer->start();

  // ####  parameter setup  ####
  const int Nc   = CommonParameters::Nc();
  const int Nd   = CommonParameters::Nd();
  const int Ndim = CommonParameters::Ndim();
  const int Nvol = CommonParameters::Nvol();

  //
  string str_fopr_type   = params_fopr  .get_string("fermion_type");
  string str_gmset_type  = params_fopr  .get_string("gamma_matrix_type");
  string str_solver_type = params_solver.get_string("solver_type");

  std::vector<Field_F> sq(Nc * Nd);
  for(int i_cd = 0; i_cd < Nc * Nd; i_cd++){
    sq[i_cd].set(0.0);
  }

  // ####  object setup  #####
  unique_ptr<GammaMatrixSet> gmset(GammaMatrixSet::New(str_gmset_type));

  unique_ptr<Fopr> fopr(Fopr::New(str_fopr_type, str_gmset_type));
  fopr -> set_parameters(params_fopr);
  fopr -> set_config(U);

  unique_ptr<Fopr> fopr_eo(Fopr::New(str_fopr_type + "_eo", str_gmset_type));
  fopr_eo -> set_parameters(params_fopr);
  fopr_eo -> set_config(U);

  unique_ptr<Solver> solver(Solver::New(str_solver_type, fopr_eo));
  solver -> set_parameters(params_solver);

  const unique_ptr<Fprop> fprop_eo(new Fprop_Standard_eo(solver));

  vout.general("\n");
  vout.general("Solving quark propagator:\n");
  vout.general("  color spin   Nconv      diff          diff2\n");



  for (  int ispin  = 0; ispin  < Nd; ++ispin)  {
    for (int icolor = 0; icolor < Nc; ++icolor) {
      int i_cd = icolor + Nc * ispin;
      
      Field_F b;  // b.set(0.0);
      source_set(b, i_cd, func);
      //source -> set(b, i_cd);
      
      int    Nconv;
      double diff;
      fprop_eo -> invert_D(sq[i_cd], b, Nconv, diff);
      
      Field_F y(b);
      fopr -> set_mode("D");
      fopr -> mult(y, sq[i_cd]); // y  = fopr[iq]->mult(sq[iq][i_cd]);
      axpy(y, -1.0, b);          // y -= b;
      double diff2 = y.norm2() / b.norm2();
      
      vout.general("   %2d   %2d   %6d   %12.4e   %12.4e\n",
		     icolor, ispin, Nconv, diff, diff2);
    }
  }
  solver_convert_bridge_to_flat(sq, prop);
  timer -> report();
}



//---------------------------------------------------------------------
// static functions
//---------------------------------------------------------------------


//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
static void solver_convert_bridge_to_flat(const std::vector<Field_F> &bridge,
					  double                     *flat)
{
  using namespace rcnp;

  for(int cd_I = 0; cd_I < 3*4; cd_I++){
    const double *src = (double*)bridge[cd_I].ptr(0);

    for(  int ixyzt = 0; ixyzt < XYZTnodeSites; ixyzt++){
      for(int ccd_F = 0; ccd_F < 2*3*4;         ccd_F++){
        flat[ccd_F + 2*3*4*(cd_I + 3*4*(ixyzt))] = src[ccd_F + 2*3*4*(ixyzt)];
      }
    }
  }
}

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
static void source_set(Field_F& b, int icd, const double *func)
{
  using namespace rcnp;

  b.set(0.0);
  if(TnodeCoor != 0) return;

  Complex *dst = (Complex*)b.ptr(0);
  for(int ixyz = 0; ixyz < XYZnodeSites; ixyz++){
    dst[icd + 12*ixyz] = ((Complex*)func)[ixyz];
  }  
}

