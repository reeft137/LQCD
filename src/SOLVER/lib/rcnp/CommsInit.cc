//---------------------------------------------------------------------
/**
 * @file
 * @brief  An example of assigning 4D grid coordinate from global MPI rank
 * @author Noriyoshi Ishii
 * @since  Tue Feb 12 20:58:14 JST 2013
 * @date   Sat Dec 18 00:41:42 JST 2021
 *
 * Copyright (C) 2013 by Noriyoshi Ishii, All Rights Reserved.
 *
 */
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

namespace rcnp {
  //---------------------------------------------------------------------
  /**
   * @brief Node coordinate is assigned to each node according to the MPI rank.
   *
   * @param[in]  Xnodes    Number of nodes along X direction
   * @param[in]  Ynodes    Number of nodes along Y direction
   * @param[in]  Znodes    Number of nodes along Z direction
   * @param[in]  Tnodes    Number of nodes along T direction
   *
   * @param[out] XnodeCoor Node coordinate along X direction 
   * @param[out] YnodeCoor Node coordinate along Y direction 
   * @param[out] ZnodeCoor Node coordinate along Z direction 
   * @param[out] TnodeCoor Node coordinate along T direction
   *
   * @param[in]  comm      The communicator
   */
  //---------------------------------------------------------------------
  void CommsInit(int  Xnodes,    int  Ynodes,    int  Znodes,    int  Tnodes,
		 int& XnodeCoor, int& YnodeCoor, int& ZnodeCoor, int& TnodeCoor,
		 MPI_Comm comm)
  {
    int rank, size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    
    // https://www.cplusplus.com/reference/cassert/assert/
    assert(size == Xnodes *Ynodes *Znodes *Tnodes);
    
    XnodeCoor = rank % Xnodes; rank /= Xnodes; // node coordinate along x direction
    YnodeCoor = rank % Ynodes; rank /= Ynodes; // node coordinate along y direction
    ZnodeCoor = rank % Znodes; rank /= Znodes; // node coordinate along z direction
    TnodeCoor = rank % Tnodes; rank /= Tnodes; // node coordinate along t direction
  }
}
