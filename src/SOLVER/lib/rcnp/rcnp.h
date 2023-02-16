//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Sat Dec 18 03:17:35 JST 2021
 */
//---------------------------------------------------------------------

#ifndef IS_INCLUDED_RCNP_H_211218
#define IS_INCLUDED_RCNP_H_211218

#include <mpi.h>
#include <stddef.h>

namespace rcnp {
  enum {Xdir=0, Ydir=1, Zdir=2, Tdir=3};

  extern bool     is_initialized;

  extern int      Xsites,     Ysites,     Zsites,     Tsites;
  extern int      Xnodes,     Ynodes,     Znodes,     Tnodes;
  extern int      XnodeSites, YnodeSites, ZnodeSites, TnodeSites;
  extern int      XnodeCoor,  YnodeCoor,  ZnodeCoor,  TnodeCoor;
  
  extern int      XYZTsites,  XYZTnodes,  XYZTnodeSites, XYZTnodeCoor;
  extern int      XYZsites,   XYZnodes,   XYZnodeSites,  XYZnodeCoor;

  extern MPI_Comm comm_XYZT;
  extern int      rank_XYZT,  size_XYZT;

  extern MPI_Comm comm_XYZ;
  extern int      rank_XYZ,   size_XYZ;

  extern MPI_Comm comm_X;
  extern int      rank_X,     size_X;
  
  extern MPI_Comm comm_Y;
  extern int      rank_Y,     size_Y;

  extern MPI_Comm comm_Z;
  extern int      rank_Z,     size_Z;

  extern MPI_Comm comm_T;
  extern int      rank_T,     size_T;

  //---------------------------------------------------------------------
  // functions
  //---------------------------------------------------------------------
  extern void initialize(int Xsites, int Ysites, int Zsites, int Tsites,
			 int Xnodes, int Ynodes, int Znodes, int Tnodes,
			 MPI_Comm comm);

  extern void initialize(int Xsites,    int Ysites,    int Zsites,    int Tsites,
			 int Xnodes,    int Ynodes,    int Znodes,    int Tnodes,
			 int XnodeCoor, int YnodeCoor, int ZnodeCoor, int TnodeCoor,
			 MPI_Comm comm);

  extern void finalize();
  
  extern void CommsInit(int  Xnodes,    int  Ynodes,    int  Znodes,    int  Tnodes,
			int &XnodeCoor, int &YnodeCoor, int &ZnodeCoor, int &TnodeCoor,
			MPI_Comm comm);
  
  extern void comm4D_to_comm3D(MPI_Comm comm4D,
			       int Xnodes,    int Ynodes,    int Znodes,    int Tnodes,
			       int XnodeCoor, int YnodeCoor, int ZnodeCoor, int TnodeCoor,
			       MPI_Comm &comm_XYZ);

  extern void comm4D_to_comm1D(MPI_Comm comm4D,
			       int Xnodes,    int Ynodes,    int Znodes,    int Tnodes,
			       int XnodeCoor, int YnodeCoor, int ZnodeCoor, int TnodeCoor,
			       MPI_Comm &comm_T);

  // I/O
  extern void mpiread_XYZT( int nin, int nex, double *ptr, MPI_File fh, int offset=0);
  extern void mpiwrite_XYZT(int nin, int nex, double *ptr, MPI_File fh, int offset=0);

  extern void mpiread_XYZ(  int nin, int nex, double *ptr, MPI_File fh, int offset=0);
  extern void mpiwrite_XYZ( int nin, int nex, double *ptr, MPI_File fh, int offset=0);

  // shift
  extern void shift(double *ptr, int dir, int ishift, int nin, int nex);
}

#endif

#include "misc.h"

