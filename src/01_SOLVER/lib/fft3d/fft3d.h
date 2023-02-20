//---------------------------------------------------------------------
/**
 * @file
 * @brief  Three dimensional FFT by using FFTW3 and MPI (stand-alone)
 * @author Noriyoshi Ishii
 * @since  Tue Jun 16 12:10:36 JST 2020
 */
//---------------------------------------------------------------------


#ifndef IS_INCLUDED_FFT3D_H_200616
#define IS_INCLUDED_FFT3D_H_200616

#include <fftw3.h>

//---------------------------------------------------------------------
/**
 * @brief Three Dimensional FFT using MPI
 */
//---------------------------------------------------------------------
namespace fft3d {
  enum {Forward=FFTW_FORWARD, Backward=FFTW_BACKWARD};

  extern int Xsites;
  extern int Ysites;
  extern int Zsites;

  extern int Xnodes;
  extern int Ynodes;
  extern int Znodes;

  extern int XnodeSites;
  extern int YnodeSites;
  extern int ZnodeSites;

  extern int XnodeCoor;
  extern int YnodeCoor;
  extern int ZnodeCoor;

  extern int XYZsites;
  extern int XYZnodes;
  extern int XYZnodeSites;
  extern int XYZnodeCoor;

  extern MPI_Comm comm_x;
  extern MPI_Comm comm_y;
  extern MPI_Comm comm_z;
  extern MPI_Comm comm_xyz;

  extern int rank_x;
  extern int rank_y;
  extern int rank_z;
  extern int rank_xyz;

  extern int size_x;
  extern int size_y;
  extern int size_z;
  extern int size_xyz;

  extern fftw_plan plan_x_forward;
  extern fftw_plan plan_x_backward;

  extern fftw_plan plan_y_forward;
  extern fftw_plan plan_y_backward;

  extern fftw_plan plan_z_forward;
  extern fftw_plan plan_z_backward;




  extern void initialize(int Xsites,    int Ysites,    int Zsites,
			 int Xnodes,    int Ynodes,    int Znodes,
			 int XnodeCoor, int YnodeCoor, int ZnodeCoor,
			 MPI_Comm &comm);
  extern void initialize(int Xsites,    int Ysites,    int Zsites,
			 int Xnodes,    int Ynodes,    int Znodes,
			 MPI_Comm &comm);
  extern void finalize();

  extern void run(double *ptr, int length,          int dir);
  extern void run(double *ptr, int nin2,   int nex, int dir);
}


#endif
