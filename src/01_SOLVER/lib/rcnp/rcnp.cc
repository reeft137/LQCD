//---------------------------------------------------------------------
/**
 * @file
 * @brief
 * @author Noriyoshi Ishii
 * @since  Sat Dec 18 03:40:55 JST 2021
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>
#include <rcnp/rcnp.h>

namespace rcnp {
  bool is_initialized = false;

  int Xsites = 0;     //< number of sites along X direction
  int Ysites = 0;     //< number of sites along Y direction
  int Zsites = 0;     //< number of sites along Z direction
  int Tsites = 0;     //< number of sites along T direction

  int Xnodes = 0;     //< number of nodes along X direction
  int Ynodes = 0;     //< number of nodes along Y direction
  int Znodes = 0;     //< number of nodes along Z direction
  int Tnodes = 0;     //< number of nodes along T direction

  int XnodeSites = 0; //< number of sites in each node along X direction
  int YnodeSites = 0; //< number of sites in each node along Y direction
  int ZnodeSites = 0; //< number of sites in each node along Z direction
  int TnodeSites = 0; //< number of sites in each node along T direction

  int XnodeCoor = -1; //< node coordinate along X direction
  int YnodeCoor = -1; //< node coordinate along Y direction
  int ZnodeCoor = -1; //< node coordinate along Z direction
  int TnodeCoor = -1; //< node coordinate along T direction

  int XYZTsites     = 0;  //< number of sites (4D)
  int XYZTnodes     = 0;  //< number of nodes (4D)
  int XYZTnodeSites = 0;  //< number of sites in each nodes (4D)
  int XYZTnodeCoor  = -1; //< rank (4D)

  int XYZsites      = 0;  //< number of sites (3D)
  int XYZnodes      = 0;  //< number of nodes (3D)
  int XYZnodeSites  = 0;  //< number of sites in each node (3D)
  int XYZnodeCoor   = -1; //< rank (3D)

  // 4D communicator
  MPI_Comm comm_XYZT = MPI_COMM_NULL; //< 4D communicator
  int      rank_XYZT = -1;            //< rank (4D)
  int      size_XYZT = 0;             //< number of MPI nodes (4D)

  // 3D communicator
  MPI_Comm comm_XYZ  = MPI_COMM_NULL; //< 3D communicator
  int      rank_XYZ  = -1;            //< rank (3D)
  int      size_XYZ  = 0;             //< number of MPI nodes (3D)

  // 1D communicator
  MPI_Comm comm_T    = MPI_COMM_NULL; //< 1D communicator
  int      rank_T    = -1;            //< rank (1D)
  int      size_T    = 0;             //< number of MPI nodes (1D)

  MPI_Comm comm_X    = MPI_COMM_NULL; //< 1D communicator
  int      rank_X    = -1;            //< rank (1D)
  int      size_X    = 0;             //< number of MPI nodes (1D)

  MPI_Comm comm_Y    = MPI_COMM_NULL; //< 1D communicator
  int      rank_Y    = -1;            //< rank (1D)
  int      size_Y    = 0;             //< number of MPI nodes (1D)

  MPI_Comm comm_Z    = MPI_COMM_NULL; //< 1D communicator
  int      rank_Z    = -1;            //< rank (1D)
  int      size_Z    = 0;             //< number of MPI nodes (1D)
}






namespace rcnp {
  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  void initialize(int Xsites,    int Ysites,    int Zsites,    int Tsites,
		  int Xnodes,    int Ynodes,    int Znodes,    int Tnodes,
		  int XnodeCoor, int YnodeCoor, int ZnodeCoor, int TnodeCoor,
		  MPI_Comm comm)
  {
    if(is_initialized == true) return;

    rcnp::Xsites = Xsites;
    rcnp::Ysites = Ysites;
    rcnp::Zsites = Zsites;
    rcnp::Tsites = Tsites;

    rcnp::Xnodes = Xnodes;
    rcnp::Ynodes = Ynodes;
    rcnp::Znodes = Znodes;
    rcnp::Tnodes = Tnodes;

    rcnp::XnodeCoor = XnodeCoor;
    rcnp::YnodeCoor = YnodeCoor;
    rcnp::ZnodeCoor = ZnodeCoor;
    rcnp::TnodeCoor = TnodeCoor;

    XnodeSites = Xsites/Xnodes;  assert(Xsites % Xnodes == 0);
    YnodeSites = Ysites/Ynodes;  assert(Ysites % Ynodes == 0);
    ZnodeSites = Zsites/Znodes;  assert(Zsites % Znodes == 0);
    TnodeSites = Tsites/Tnodes;  assert(Tsites % Tnodes == 0);

    // 4D part
    MPI_Comm_dup(comm, &comm_XYZT);
    MPI_Comm_size(comm_XYZT, &size_XYZT);
    MPI_Comm_rank(comm_XYZT, &rank_XYZT);

    XYZTsites     = Xsites     *Ysites     *Zsites     *Tsites;
    XYZTnodes     = Xnodes     *Ynodes     *Znodes     *Tnodes;
    XYZTnodeSites = XnodeSites *YnodeSites *ZnodeSites *TnodeSites;
    XYZTnodeCoor  = XnodeCoor + Xnodes *(YnodeCoor + Ynodes *(ZnodeCoor + Znodes *TnodeCoor));

    // 3D part
    comm4D_to_comm3D(comm_XYZT,
		     Xnodes,    Ynodes,    Znodes,    Tnodes,
		     XnodeCoor, YnodeCoor, ZnodeCoor, TnodeCoor,
		     comm_XYZ);

    MPI_Comm_size(comm_XYZ, &size_XYZ);
    MPI_Comm_rank(comm_XYZ, &rank_XYZ);

    XYZsites     = Xsites     *Ysites     *Zsites;
    XYZnodes     = Xnodes     *Ynodes     *Znodes;
    XYZnodeSites = XnodeSites *YnodeSites *ZnodeSites;
    XYZnodeCoor  = XnodeCoor + Xnodes *(YnodeCoor + Ynodes *ZnodeCoor);

    // 1D part
    comm4D_to_comm1D(comm_XYZT,
		     Xnodes,    Ynodes,    Znodes,    Tnodes,
		     XnodeCoor, YnodeCoor, ZnodeCoor, TnodeCoor,
		     comm_T);

    MPI_Comm_size(comm_T, &size_T);
    MPI_Comm_rank(comm_T, &rank_T);


    comm4D_to_comm1D(comm_XYZT,
		     Tnodes,    Ynodes,    Znodes,    Xnodes,
		     TnodeCoor, YnodeCoor, ZnodeCoor, XnodeCoor,
		     comm_X);

    MPI_Comm_size(comm_X, &size_X);
    MPI_Comm_rank(comm_X, &rank_X);


    comm4D_to_comm1D(comm_XYZT,
		     Xnodes,    Tnodes,    Znodes,    Ynodes,
		     XnodeCoor, TnodeCoor, ZnodeCoor, YnodeCoor,
		     comm_Y);

    MPI_Comm_size(comm_Y, &size_Y);
    MPI_Comm_rank(comm_Y, &rank_Y);


    comm4D_to_comm1D(comm_XYZT,
		     Xnodes,    Ynodes,    Tnodes,    Znodes,
		     XnodeCoor, YnodeCoor, TnodeCoor, ZnodeCoor,
		     comm_Z);

    MPI_Comm_size(comm_Z, &size_Z);
    MPI_Comm_rank(comm_Z, &rank_Z);

    is_initialized = true;
  }

  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  void initialize(int Xsites, int Ysites, int Zsites, int Tsites,
		  int Xnodes, int Ynodes, int Znodes, int Tnodes,
		  MPI_Comm comm)
  {
    if(is_initialized == true) return;

    int XnodeCoor, YnodeCoor, ZnodeCoor, TnodeCoor;

    CommsInit(Xnodes,    Ynodes,    Znodes,    Tnodes,
	      XnodeCoor, YnodeCoor, ZnodeCoor, TnodeCoor,
	      comm);

    initialize(Xsites, Ysites, Zsites, Tsites,
	       Xnodes, Ynodes, Znodes, Tnodes,
	       XnodeCoor, YnodeCoor, ZnodeCoor, TnodeCoor,
	       comm);
  }

  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  void finalize()
  {
    assert(is_initialized == true);

    Xsites     = Ysites     = Zsites     = Tsites     = 0;
    Xnodes     = Ynodes     = Znodes     = Tnodes     = 0;
    XnodeSites = YnodeSites = ZnodeSites = TnodeSites = 0;
    XnodeCoor  = YnodeCoor  = ZnodeCoor  = TnodeCoor  = -1;

    XYZTsites = XYZTnodes = XYZTnodeSites = 0;
    XYZTnodeCoor = -1;

    XYZsites  = XYZnodes  = XYZnodeSites  = 0;
    XYZnodeCoor  = -1;

    MPI_Comm_free(&comm_XYZT); comm_XYZT = MPI_COMM_NULL;
    MPI_Comm_free(&comm_XYZ);  comm_XYZ  = MPI_COMM_NULL;
    MPI_Comm_free(&comm_T);    comm_T    = MPI_COMM_NULL;
    MPI_Comm_free(&comm_X);    comm_X    = MPI_COMM_NULL;
    MPI_Comm_free(&comm_Y);    comm_Y    = MPI_COMM_NULL;
    MPI_Comm_free(&comm_Z);    comm_Z    = MPI_COMM_NULL;

    rank_XYZT = rank_XYZ = rank_T = rank_X = rank_Y = rank_Z = -1;
    size_XYZT = size_XYZ = size_T = size_X = size_Y = size_Z = 0;

    is_initialized = false;
  }
}
