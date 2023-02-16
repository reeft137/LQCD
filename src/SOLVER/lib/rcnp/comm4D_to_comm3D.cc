#include <mpi.h>

namespace rcnp {
  void comm4D_to_comm3D(MPI_Comm comm4D,
			int Xnodes,    int Ynodes,    int Znodes,    int Tnodes,
			int XnodeCoor, int YnodeCoor, int ZnodeCoor, int TnodeCoor,
			MPI_Comm &comm3D)
  {
    int XYZnodeCoor = XnodeCoor + Xnodes *(YnodeCoor + Ynodes *ZnodeCoor);
    
    // https://www.mpich.org/static/docs/latest/www3/MPI_Comm_split.html
    MPI_Comm_split(comm4D,      // original communicator to be splitted
		   TnodeCoor,   // color: different communicator for different value
		   XYZnodeCoor, // key:   rank of the new commumicator is determined
		                //        according to the order of this value
		   &comm3D);    // new communicator (output)
  }
}
