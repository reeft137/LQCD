//---------------------------------------------------------------------
/**
 * @file
 * @breif
 * @author Noriyoshi Ishii
 * @since  Sat Dec 18 23:52:38 JST 2021
 */
//---------------------------------------------------------------------

#include <mpi.h>
#include <rcnp/rcnp.h>

namespace rcnp {
  //---------------------------------------------------------------------
  /**
   * to read a gauge configurations, use
   * nin = 2 * 3*3 *4
   * nex = 1
   * offset = 0
   *
   * to read a quark propagators, use
   * nin = 2 * 3*4 *3*4
   * nex = 1
   * offset = 0
   */
  //---------------------------------------------------------------------
  void mpiread_XYZT(int nin, int nex, double *ptr, MPI_File fh, size_t offset)
  {
    MPI_Status stat;
    for(      int iex = 0; iex < nex;        iex++){
      for(    int it  = 0; it  < TnodeSites; it++ ){
	for(  int iz  = 0; iz  < ZnodeSites; iz++ ){
	  for(int iy  = 0; iy  < YnodeSites; iy++ ){
	    int ix    = 0;
	    int iin   = 0;
	    int ixyzt = ix + XnodeSites*(iy + YnodeSites*(iz + ZnodeSites*(it)));
	  
	    int iX    = ix + XnodeSites *XnodeCoor;
	    int iY    = iy + YnodeSites *YnodeCoor;
	    int iZ    = iz + ZnodeSites *ZnodeCoor;
	    int iT    = it + TnodeSites *TnodeCoor;
	    int iXYZT = iX + Xsites*(iY + Ysites*(iZ + Zsites*iT));
	    
	    // https://www.mpich.org/static/docs/v3.1.x/www3/MPI_File_read_at_all.html
	    MPI_File_read_at_all(fh,
				 offset + sizeof(double)*(iin + nin*(iXYZT + XYZTsites*iex)),
				 &ptr[iin + nin*(ixyzt + XYZTnodeSites*(iex))],
				 nin *XnodeSites,
				 MPI_DOUBLE,
				 &stat);	  
	  } // for(int iy = 0; ...)
	} // for(int iz = 0; ...)
      } // for(int it = 0; ...)
    } // for(int iex = 0; ...)
  }

  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  void mpiwrite_XYZT(int nin, int nex, double *ptr, MPI_File fh, size_t offset)
  {
    MPI_Status stat;
    for(      int iex = 0; iex < nex;        iex++){
      for(    int it  = 0; it  < TnodeSites; it++ ){
	for(  int iz  = 0; iz  < ZnodeSites; iz++ ){
	  for(int iy  = 0; iy  < YnodeSites; iy++ ){
	    int ix    = 0;
	    int iin   = 0;
	    int ixyzt = ix + XnodeSites*(iy + YnodeSites*(iz + ZnodeSites*it));

	    int iX    = ix + XnodeSites *XnodeCoor;
	    int iY    = iy + YnodeSites *YnodeCoor;
	    int iZ    = iz + ZnodeSites *ZnodeCoor;
	    int iT    = it + TnodeSites *TnodeCoor;
	    int iXYZT = iX + Xsites*(iY + Ysites*(iZ + Zsites*iT));

	    // https://www.mpich.org/static/docs/latest/www3/MPI_File_write_at_all.html
	    MPI_File_write_at_all(fh,
				  offset + sizeof(double)*(iin + nin*(iXYZT + XYZTsites*iex)),
				  &ptr[iin + nin*(ixyzt + XYZTnodeSites*iex)],
				  nin *XnodeSites,
				  MPI_DOUBLE,
				  &stat);
	  } // for(int iy = 0; ...)
	} // for(int iz = 0; ...)
      } // for(int it = 0; ...)
    } // for(int iex = 0; ...)
  }


  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  void mpiread_XYZ(int nin, int nex, double *ptr, MPI_File fh, size_t offset)
  {
    MPI_Status stat;
    for(int     iex = 0; iex < nex;        iex++){
      for(int   iz  = 0; iz  < ZnodeSites; iz++ ){
	for(int iy  = 0; iy  < YnodeSites; iy++ ){
	  int ix    = 0;
	  int iin   = 0;
	  int ixyz  = ix + XnodeSites*(iy + YnodeSites*iz);

	  int iX    = ix + XnodeSites *XnodeCoor;
	  int iY    = iy + YnodeSites *YnodeCoor;
	  int iZ    = iz + ZnodeSites *ZnodeCoor;
	  int iXYZ  = iX + Xsites*(iY + Ysites*iZ);

	  // https://www.mpich.org/static/docs/v3.1.x/www3/MPI_File_read_at_all.html
	  MPI_File_read_at_all(fh,
			       offset + sizeof(double)*(iin + nin*(iXYZ + XYZsites*iex)),
			       &ptr[iin + nin*(ixyz + XYZnodeSites*iex)],
			       nin *XnodeSites,
			       MPI_DOUBLE,
			       &stat);	  
	} // for(int iy = 0; ...)
      } // for(int iz = 0; ...)
    } // for(int iex = 0; ...)
  }

  //---------------------------------------------------------------------
  /**
   *
   */
  //---------------------------------------------------------------------
  void mpiwrite_XYZ(int nin, int nex, double *ptr, MPI_File fh, size_t offset)
  {
    MPI_Status stat;
    for(    int iex = 0; iex < nex;        iex++){
      for(  int iz  = 0; iz  < ZnodeSites; iz++ ){
	for(int iy  = 0; iy  < YnodeSites; iy++ ){
	  int ix    = 0;
	  int iin   = 0;
	  int ixyz  = ix + XnodeSites*(iy + YnodeSites*iz);

	  int iX    = ix + XnodeSites *XnodeCoor;
	  int iY    = iy + YnodeSites *YnodeCoor;
	  int iZ    = iz + ZnodeSites *ZnodeCoor;
	  int iXYZ  = iX + Xsites*(iY + Ysites*iZ);
	  
	  // https://www.mpich.org/static/docs/latest/www3/MPI_File_write_at_all.html
	  MPI_File_write_at_all(fh,
				offset + sizeof(double)*(iin + nin*(iXYZ + XYZsites *iex)),
				&ptr[iin + nin*(ixyz + XYZnodeSites *iex)],
				nin *XnodeSites,
				MPI_DOUBLE,
				&stat);
	} // for(int iy = 0; ...)
      } // for(int iz = 0; ...)
    } // for(int iex = 0; ...)
  }
}
