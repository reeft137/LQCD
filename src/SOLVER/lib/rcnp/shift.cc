//---------------------------------------------------------------------
/**
 * @file
 * @brief  To shift spatially the gauge configurations and others
 * @author Noriyoshi Ishii
 * @since  Thu May 26 14:30:27 JST 2022
 */
//---------------------------------------------------------------------

#include <mpi.h>
#include <assert.h>
#include <rcnp/rcnp.h>

static void shift(double *ptr, MPI_Comm &comm, int ishift, int nin, int length, int nex);

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void rcnp::shift(double *ptr, int dir, int ishift, int nin, int nex)
{
  int      Nin, length, Nex;
  MPI_Comm comm;

  switch(dir){
  case rcnp::Xdir:
    Nin    = nin;
    length = XnodeSites;
    Nex    = YnodeSites *ZnodeSites *TnodeSites *nex;
    comm   = comm_X;
    break;
  case rcnp::Ydir:
    Nin    = nin *XnodeSites;
    length = YnodeSites;
    Nex    = ZnodeSites *TnodeSites *nex;
    comm   = comm_Y;
    break;
  case rcnp::Zdir:
    Nin    = nin *XnodeSites *YnodeSites;
    length = ZnodeSites;
    Nex    = TnodeSites *nex;
    comm   = comm_Z;
    break;
  case rcnp::Tdir:
    Nin    = nin *XnodeSites *YnodeSites *ZnodeSites;
    length = TnodeSites;
    Nex    = nex;
    comm   = comm_T;
    break;
  default:
    assert(false && "void rcnp::shift(ptr, dir, ishift, nin, nex)");
    break;
  }

  ::shift(ptr, comm, ishift, Nin, length, Nex);
}

// //---------------------------------------------------------------------
// /**
//  *
//  */
// //---------------------------------------------------------------------
// static void shift(double *ptr, MPI_Comm &comm, int ishift, int Nin, int length, int Nex)
// {
//   int rank, size;
//   MPI_Comm_rank(comm, &rank);
//   MPI_Comm_size(comm, &size);

//   typedef double (*Global)[Nex][length][Nin];
//   typedef double (*Local)      [length][Nin];

//   Global Tmp = (Global)new double[Nin *length *Nex *size];
//   Local  Ptr = (Local) ptr;

//   MPI_Allgather(ptr,              Nin *length *Nex, MPI_DOUBLE,
// 		&Tmp[0][0][0][0], Nin *length *Nex, MPI_DOUBLE,
// 		comm);

//   for(  int iex = 0; iex < Nex;    iex++){
//     for(int is  = 0; is  < length; is++ ){
//       int iS = (length *rank + is + ishift + 1024*(length*size)) % (length*size);
//       int is2   = iS % length;
//       int irank = iS / length;
//       double *src = &Tmp[irank][iex][is2][0];
//       double *dst = &Ptr[       iex][is ][0];
//       for(int iin = 0; iin < Nin; iin++){
// 	dst[iin] = src[iin];
//       }
//     } // for(int is  = 0; ...)
//   }   // for(int iex = 0; ...)

//   delete[] Tmp;
// }

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
static void shift(double *ptr, MPI_Comm &comm, int ishift, int Nin, int length, int Nex)
{
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  //typedef double (*Global)[Nex][length][Nin];
  //typedef double (*Local)      [length][Nin];
  //Global Tmp = (Global)new double[Nin *length *Nex *size];
  //Local  Ptr = (Local) ptr;

  double *tmp = new double[Nin *length *Nex *size];
  
#define Global(iin, is, iex, irank) tmp[iin + Nin*(is + length*(iex + Nex*(irank)))]
#define Local( iin, is, iex)        ptr[iin + Nin*(is + length*(iex))              ]
  
  MPI_Allgather(ptr, Nin *length *Nex, MPI_DOUBLE,
		tmp, Nin *length *Nex, MPI_DOUBLE,
		comm);

  for(  int iex = 0; iex < Nex;    iex++){
    for(int is  = 0; is  < length; is++ ){
      int iS = (length *rank + is + ishift + 1024*(length*size)) % (length*size);
      int is2   = iS % length;
      int irank = iS / length;
      double *src = &Global(0, is2, iex, irank);
      double *dst = &Local( 0, is,  iex);
      for(int iin = 0; iin < Nin; iin++){
	dst[iin] = src[iin];
      }
    } // for(int is  = 0; ...)
  }   // for(int iex = 0; ...)

  delete[] tmp;
}

