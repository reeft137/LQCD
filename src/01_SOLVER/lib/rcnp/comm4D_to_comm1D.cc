#include <mpi.h>

namespace rcnp {
  void comm4D_to_comm1D(MPI_Comm comm4D,
			int Xnodes,    int Ynodes,    int Znodes,    int Tnodes,
			int XnodeCoor, int YnodeCoor, int ZnodeCoor, int TnodeCoor,
			MPI_Comm &comm1D)
  {
    int XYZnodeCoor = XnodeCoor + Xnodes *(YnodeCoor + Ynodes *ZnodeCoor);

    // https://www.mpich.org/static/docs/latest/www3/MPI_Comm_split.html
    MPI_Comm_split(comm4D,      // the original communicator
		   XYZnodeCoor, // color: different communicator for different value
		   TnodeCoor,   // key:   MPI rank is assigned according to the ordering of this value
		   &comm1D);    // new communicator
  }
}
