//---------------------------------------------------------------------
/**
 * @file
 * @brief  Three dimensional FFT by using FFTW3 and MPI (stand-alone).
 *         For FFTW3 library, https://www.fftw.org
 * @author Noriyoshi Ishii
 * @since  Tue Jun 16 12:15:56 JST 2020
 *
 */
//---------------------------------------------------------------------

#define Flag_x true
#define Flag_y true
#define Flag_z true

#include <mpi.h>
#include <fftw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <complex.h>
typedef double _Complex Complex;

#include "fft3d.h"

int fft3d::Xsites        = 0;
int fft3d::Ysites        = 0;
int fft3d::Zsites        = 0;

int fft3d::Xnodes        = 0;
int fft3d::Ynodes        = 0;
int fft3d::Znodes        = 0;

int fft3d::XnodeSites    = 0;
int fft3d::YnodeSites    = 0;
int fft3d::ZnodeSites    = 0;

int fft3d::XnodeCoor     = 0;
int fft3d::YnodeCoor     = 0;
int fft3d::ZnodeCoor     = 0;

int fft3d::XYZsites      = 0;
int fft3d::XYZnodes      = 0;
int fft3d::XYZnodeSites  = 0;
int fft3d::XYZnodeCoor   = 0;



MPI_Comm fft3d::comm_x   = MPI_COMM_NULL;
MPI_Comm fft3d::comm_y   = MPI_COMM_NULL;
MPI_Comm fft3d::comm_z   = MPI_COMM_NULL;
MPI_Comm fft3d::comm_xyz = MPI_COMM_NULL;

int fft3d::rank_x   = -1;
int fft3d::rank_y   = -1;
int fft3d::rank_z   = -1;
int fft3d::rank_xyz = -1;

int fft3d::size_x   = -1;
int fft3d::size_y   = -1;
int fft3d::size_z   = -1;
int fft3d::size_xyz = -1;

fftw_plan fft3d::plan_x_forward;
fftw_plan fft3d::plan_y_forward;
fftw_plan fft3d::plan_z_forward;

fftw_plan fft3d::plan_x_backward;
fftw_plan fft3d::plan_y_backward;
fftw_plan fft3d::plan_z_backward;

fftw_complex *in_x = NULL;
fftw_complex *in_y = NULL;
fftw_complex *in_z = NULL;

fftw_complex *out_x = NULL;
fftw_complex *out_y = NULL;
fftw_complex *out_z = NULL;

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
static void copy(Complex *dst, Complex *src, int len)
{
  for(int i = 0; i < len; i++){
    dst[i] = src[i];
  }
}

//---------------------------------------------------------------------
/**
 * @brief swap the ordering of the indices:
 *        Complex dst[length2][length1][length3];
 *        Complex src[length1][length2][length3];
 */
//---------------------------------------------------------------------
static void change_order_123_to_213(Complex *dst,
				    Complex *src,
				    int length1, int length2, int length3)
{
  // Complex (*Dst)[length1][length3] = (Complex (*)[length1][length3]) dst;
  // Complex (*Src)[length2][length3] = (Complex (*)[length2][length3]) src;
#define Dst(i, j, k) ((Complex*)dst)[i + length3*(j + length1*(k))]
#define Src(i, j, k) ((Complex*)src)[i + length3*(j + length2*(k))]

  for(  int i = 0; i < length1; i++){
    for(int j = 0; j < length2; j++){
      // Complex *ptr_src = &Src[i][j][0];
      // Complex *ptr_dst = &Dst[j][i][0];
      Complex *ptr_src = &Src(0, j, i);
      Complex *ptr_dst = &Dst(0, i, j);
      for(int k = 0; k < length3; k++){
	ptr_dst[k] = ptr_src[k];
      }
    }
  }

#undef Src
#undef Dst
}

//---------------------------------------------------------------------
/**
 * @brief swap the ordering of the indices:
 *        Complex dst[length1][length3][length2];
 *        Complex src[length1][length2][length3];
 */
//---------------------------------------------------------------------
static void change_order_123_to_132(Complex *dst,
				    Complex *src,
				    int length1, int length2, int length3)
{
  // Complex (*Dst)[length3][length2] = (Complex (*)[length3][length2]) dst;
  // Complex (*Src)[length2][length3] = (Complex (*)[length2][length3]) src;
#define Dst(i, j, k) ((Complex*)dst)[i + length2*(j + length3*(k))]
#define Src(i, j, k) ((Complex*)src)[i + length3*(j + length2*(k))]
  
  for(    int i = 0; i < length1; i++){
    for(  int j = 0; j < length2; j++){
      for(int k = 0; k < length3; k++){
	// Dst[i][k][j] = Src[i][j][k];
	Dst(j,k,i) = Src(k,j,i);
      }
    }
  }

#undef Src
#undef Dst
}

//---------------------------------------------------------------------
/**
 * @brief Initialization of fft
 */
//---------------------------------------------------------------------
void fft3d::initialize(int Xsites,    int Ysites,    int Zsites,
		       int Xnodes,    int Ynodes,    int Znodes,
		       int XnodeCoor, int YnodeCoor, int ZnodeCoor,
		       MPI_Comm &comm)
{
  if(MPI_COMM_NULL != comm_x){
    fprintf(stderr, "ERROR: attempting to reinitialize fft3d\n");
    exit(1);
  }

  MPI_Comm_dup(comm, &comm_xyz);

  fft3d::Xsites = Xsites;
  fft3d::Ysites = Ysites;
  fft3d::Zsites = Zsites;

  fft3d::Xnodes = Xnodes;
  fft3d::Ynodes = Ynodes;
  fft3d::Znodes = Znodes;

  fft3d::XnodeCoor = XnodeCoor;
  fft3d::YnodeCoor = YnodeCoor;
  fft3d::ZnodeCoor = ZnodeCoor;

  XnodeSites = Xsites / Xnodes;
  YnodeSites = Ysites / Ynodes;
  ZnodeSites = Zsites / Znodes;



  XYZsites     = Xsites     * Ysites     * Zsites;
  XYZnodes     = Xnodes     * Ynodes     * Znodes;
  XYZnodeSites = XnodeSites * YnodeSites * ZnodeSites;
  XYZnodeCoor  = XnodeCoor + Xnodes*(YnodeCoor + Ynodes*ZnodeCoor);

  //---------------------------------------------------------------------
  // Initializations of 1D communicator and fftw plan
  //---------------------------------------------------------------------

  int XYnodeCoor = XnodeCoor + Xnodes *YnodeCoor;
  int YZnodeCoor = YnodeCoor + Ynodes *ZnodeCoor;
  int ZXnodeCoor = ZnodeCoor + Znodes *XnodeCoor;

  MPI_Comm_split(comm_xyz, YZnodeCoor, XnodeCoor, &comm_x);
  MPI_Comm_split(comm_xyz, ZXnodeCoor, YnodeCoor, &comm_y);
  MPI_Comm_split(comm_xyz, XYnodeCoor, ZnodeCoor, &comm_z);
  
  MPI_Comm_rank(comm_x, &rank_x);
  MPI_Comm_rank(comm_y, &rank_y);
  MPI_Comm_rank(comm_z, &rank_z);

  MPI_Comm_size(comm_x, &size_x);
  MPI_Comm_size(comm_y, &size_y);
  MPI_Comm_size(comm_z, &size_z);

  in_x = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Xsites);
  in_y = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Ysites);
  in_z = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Zsites);

  out_x = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Xsites);
  out_y = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Ysites);
  out_z = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Zsites);

  plan_x_forward  = fftw_plan_dft_1d(Xsites, in_x, out_x, FFTW_FORWARD,  FFTW_ESTIMATE);
  plan_y_forward  = fftw_plan_dft_1d(Ysites, in_y, out_y, FFTW_FORWARD,  FFTW_ESTIMATE);
  plan_z_forward  = fftw_plan_dft_1d(Zsites, in_z, out_z, FFTW_FORWARD,  FFTW_ESTIMATE);

  plan_x_backward = fftw_plan_dft_1d(Xsites, in_x, out_x, FFTW_BACKWARD, FFTW_ESTIMATE);
  plan_y_backward = fftw_plan_dft_1d(Ysites, in_y, out_y, FFTW_BACKWARD, FFTW_ESTIMATE);
  plan_z_backward = fftw_plan_dft_1d(Zsites, in_z, out_z, FFTW_BACKWARD, FFTW_ESTIMATE);
}

//---------------------------------------------------------------------
/**
 *
 */
//---------------------------------------------------------------------
void fft3d::initialize(int Xsites, int Ysites, int Zsites,
		       int Xnodes, int Ynodes, int Znodes,
		       MPI_Comm &comm)
{
  if(MPI_COMM_NULL != comm_x){
    fprintf(stderr, "ERROR: attempting to reinitialize fft3d\n");
    exit(1);
  }

  int rank, size;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  int tmp = rank;
  XnodeCoor = tmp % Xnodes; tmp /= Xnodes;
  YnodeCoor = tmp % Ynodes; tmp /= Ynodes;
  ZnodeCoor = tmp % Znodes;

  fft3d::initialize(Xsites,    Ysites,    Zsites,
		    Xnodes,    Ynodes,    Znodes,
		    XnodeCoor, YnodeCoor, ZnodeCoor,
		    comm);
}

//---------------------------------------------------------------------
/**
 * @brief Finalization of fft
 */
//---------------------------------------------------------------------
void fft3d::finalize()
{
  if(MPI_COMM_NULL == comm_x) return;

  if(MPI_COMM_NULL != comm_x)   MPI_Comm_free(&comm_x);   comm_x   = MPI_COMM_NULL;
  if(MPI_COMM_NULL != comm_y)   MPI_Comm_free(&comm_y);   comm_y   = MPI_COMM_NULL;
  if(MPI_COMM_NULL != comm_z)   MPI_Comm_free(&comm_z);   comm_z   = MPI_COMM_NULL;
  if(MPI_COMM_NULL != comm_xyz) MPI_Comm_free(&comm_xyz); comm_xyz = MPI_COMM_NULL;
  
  fftw_destroy_plan(plan_x_forward);
  fftw_destroy_plan(plan_y_forward);
  fftw_destroy_plan(plan_z_forward);

  fftw_destroy_plan(plan_x_backward);
  fftw_destroy_plan(plan_y_backward);
  fftw_destroy_plan(plan_z_backward);

  fftw_free(in_x); in_x = NULL;
  fftw_free(in_y); in_y = NULL;
  fftw_free(in_z); in_z = NULL;

  fftw_free(out_x); out_x = NULL;
  fftw_free(out_y); out_y = NULL;
  fftw_free(out_z); out_z = NULL;
}

//---------------------------------------------------------------------
/**
 * @brief Execute 3D FFT
 */
//---------------------------------------------------------------------
void fft3d::run(double *ptr, int nin2, int nex, int dir)
{
  Complex *tmp = new Complex[XYZnodeSites * nin2*nex];

  // Complex (*src)      [XYZnodeSites][nin2] = (Complex (*)      [XYZnodeSites][nin2])ptr;
  // Complex (*dst)[nin2][XYZnodeSites]       = (Complex (*)[nin2][XYZnodeSites])      tmp;
  Complex *src = (Complex*)ptr;
  Complex *dst = (Complex*)tmp;
  
  change_order_123_to_132((Complex*)dst, (Complex*)src, nex, XYZnodeSites, nin2);

  fft3d::run((double*)tmp, nin2*nex, dir);

  change_order_123_to_132((Complex*)src, (Complex*)dst, nex, nin2, XYZnodeSites);

  delete[] tmp;
}

//---------------------------------------------------------------------
/**
 * @brief Execute 3D FFT
 */
//---------------------------------------------------------------------
void fft3d::run(double* ptr, int length, int dir)
{
  // Complex ptr[length][XYZnodesites]

  int length_x = YnodeSites * ZnodeSites * length;
  int length_y = ZnodeSites * XnodeSites * length;
  int length_z = XnodeSites * YnodeSites * length;

  if(length_x % Xnodes != 0) length_x = (length_x/Xnodes + 1)*Xnodes;
  if(length_y % Ynodes != 0) length_y = (length_y/Ynodes + 1)*Ynodes;
  if(length_z % Znodes != 0) length_z = (length_z/Znodes + 1)*Znodes;

  int size = XnodeSites * length_x;
  if(size < YnodeSites * length_y) size = YnodeSites * length_y;
  if(size < ZnodeSites * length_z) size = ZnodeSites * length_z;

  Complex *ptr1 = new Complex[size];
  Complex *ptr2 = new Complex[size];
  Complex *ptr3 = new Complex[size];

  //---------------------------------------------------------------------
  // FFT along x-direction
  //---------------------------------------------------------------------
  {
    // Complex ptr [length_x               ][XnodeSites]
    //      == ptr [Xnodes *length_x/Xnodes][XnodeSites]
    // Complex ptr1[Xnodes][length_x/Xnodes][XnodeSites]

    MPI_Alltoall((double*)ptr,  2*XnodeSites *length_x/Xnodes, MPI_DOUBLE,
		 (double*)ptr1, 2*XnodeSites *length_x/Xnodes, MPI_DOUBLE, comm_x);

    // rearrangement (forward)--> Fourier transformation --> rearrangement (backward)
    // Complex ptr1[Xnodes] [length_x/Xnodes]         [XnodeSites]
    // Complex ptr2         [length_x/Xnodes][Xnodes] [XnodeSites]

    // rearrangement (forward)
    change_order_123_to_213(ptr2,
			    ptr1,
			    Xnodes, length_x/Xnodes, XnodeSites);
#if Flag_x
    // Fourier transformation
    for(  int ith = 0; ith < length_x/Xnodes; ith++){
      for(int iX  = 0; iX  < Xsites;          iX ++){
	((Complex*)in_x)[iX] = ptr2[iX + Xsites*ith];
      }
      //
      switch(dir){
      case FFTW_FORWARD:  fftw_execute(plan_x_forward);  break;
      case FFTW_BACKWARD: fftw_execute(plan_x_backward); break;
      case 1024:          copy((Complex*)out_x, (Complex*)in_x, Xsites); break;
      default:
	fprintf(stderr, "ERROR(fft3d::run) dir=%d, line=%d\n", dir, __LINE__);
	exit(1); break;
      }
      //
      for(int iX = 0; iX < Xsites; iX++){
	ptr2[iX + Xsites*ith] = ((Complex*)out_x)[iX];
      }
    } // for(ith = 0; ...)
#endif
    // rearrangement (backward)
    change_order_123_to_213(ptr1,
			    ptr2,
			    length_x/Xnodes, Xnodes, XnodeSites);

    MPI_Alltoall((double*)ptr1, 2*XnodeSites *length_x/Xnodes, MPI_DOUBLE,
		 (double*)ptr2, 2*XnodeSites *length_x/Xnodes, MPI_DOUBLE, comm_x);
  }

  //---------------------------------------------------------------------
  // rearrangement: XYZ --> YZX
  //---------------------------------------------------------------------
  {
    // Complex ptr2[length] [ZnodeSites][YnodeSites][XnodeSites];
    // Complex ptr3[length] [Xnodesites][ZnodeSites][YnodeSites];

    change_order_123_to_132(ptr3,
			    ptr2,
			    length, ZnodeSites*YnodeSites, XnodeSites);
  }
  //---------------------------------------------------------------------
  // FFT along y-direction
  //---------------------------------------------------------------------
  {
    // Complex ptr3[length_y               ][YnodeSites]
    //      => ptr3[Ynodes][length_y/Ynodes][YnodeSites]
    // Complex ptr1[Ynodes][length_y/Ynodes][YnodeSites]

    MPI_Alltoall((double*)ptr3, 2*YnodeSites *length_y/Ynodes, MPI_DOUBLE,
		 (double*)ptr1, 2*YnodeSites *length_y/Ynodes, MPI_DOUBLE, comm_y);

    // rearrangement (forward) --> Fourier transformation --> rearrangement (backward)
    // Complex ptr1[Ynodes]         [length_y/Ynodes]  [YnodeSites]
    // Complex ptr2[length_y/Ynodes][Ynodes]           [YnodeSites]

    // rearrangement (forward)
    change_order_123_to_213(ptr2,
			    ptr1,
			    Ynodes, length_y/Ynodes, YnodeSites);
#if Flag_y
    // Fourier transformation
    for(  int ith = 0; ith < length_y/Ynodes; ith++){
      for(int iY  = 0; iY  < Ysites;          iY ++){
	((Complex*)in_y)[iY] = ptr2[iY + Ysites*ith];
      }
      //
      switch(dir){
      case FFTW_FORWARD:  fftw_execute(plan_y_forward);  break;
      case FFTW_BACKWARD: fftw_execute(plan_y_backward); break;
      case 1024:          copy((Complex*)out_y, (Complex*)in_y, Ysites); break;
      default:
	fprintf(stderr, "ERROR(fftw3_xyz::run) dir=%d, line=%d\n", dir, __LINE__);
	exit(1); break;
      }
      //
      for(int iY = 0; iY < Ysites; iY++){
	ptr2[iY + Ysites*ith] = ((Complex*)out_y)[iY];
      }
    } // for(int ith = 0; ...)
#endif
    // rearrangement (backward)
    change_order_123_to_213(ptr1,
			    ptr2,
			    length_y/Ynodes, Ynodes, YnodeSites);

    MPI_Alltoall((double*)ptr1, 2*YnodeSites *length_y/Ynodes, MPI_DOUBLE,
		 (double*)ptr2, 2*YnodeSites *length_y/Ynodes, MPI_DOUBLE, comm_y);
  }

  //---------------------------------------------------------------------
  // Rearrangement: YZX --> ZXY
  //---------------------------------------------------------------------
  {
    // Complex ptr2[length] [XnodeSites][ZnodeSites][YnodeSites]
    // Complex ptr [length] [YnodeSites][XnodeSites][ZnodeSites]

    change_order_123_to_132(ptr3,
			    ptr2,
			    length, XnodeSites*ZnodeSites, YnodeSites);
  }
  //---------------------------------------------------------------------
  // FFT along z-direction
  //---------------------------------------------------------------------
  {
    // Complex ptr3[length_z               ] [ZnodeSites]
    //      => ptr3[Znodes][length_z/Znodes] [ZnodeSites]
    // Complex ptr1[Znodes][length_z/Znodes] [ZnodeSites]

    MPI_Alltoall((double*)ptr3, 2*ZnodeSites *length_z/Znodes, MPI_DOUBLE,
		 (double*)ptr1, 2*ZnodeSites *length_z/Znodes, MPI_DOUBLE, comm_z);

    // rearrangement (forward)）--> Fourier transformation --> rearrangement (backward)
    // Complex ptr1[Znodes]         [length_z/Znodes]  [ZnodeSites]
    // Complex ptr2[length_z/Znodes][Znodes]           [ZnodeSites]
    
    // rearrangement (forward)
    change_order_123_to_213(ptr2,
			    ptr1,
			    Znodes, length_z/Znodes, ZnodeSites);
#if Flag_z
    // Frourier transformation
    for(  int ith = 0; ith < length_z/Znodes; ith++){
      for(int iZ  = 0; iZ  < Zsites;          iZ ++){
	((Complex*)in_z)[iZ] = ptr2[iZ + Zsites*ith];
      }
      //
      switch(dir){
      case FFTW_FORWARD:  fftw_execute(plan_z_forward);  break;
      case FFTW_BACKWARD: fftw_execute(plan_z_backward); break;
      case 1024:          copy((Complex*)out_z, (Complex*)in_z, Zsites); break;
      default:
	fprintf(stderr, "ERROR(fftw3_xyz::run) dir=%d, line=%d\n", dir, __LINE__);
	exit(1); break;
      }
      //
      for(int iZ = 0; iZ < Zsites; iZ++){
	ptr2[iZ + Zsites*ith] = ((Complex*)out_z)[iZ];
      }
    } // for(int ith = 0; ...)
#endif
    // Rearrangement (backward)
    change_order_123_to_213(ptr1,
			    ptr2,
			    length_z/Znodes, Znodes, ZnodeSites);
    
    MPI_Alltoall((double*)ptr1, 2*ZnodeSites *length_z/Znodes, MPI_DOUBLE,
		 (double*)ptr2, 2*ZnodeSites *length_z/Znodes, MPI_DOUBLE, comm_z);
  }
  
  //---------------------------------------------------------------------
  // Rearrengement: ZXY --> XYZ
  //---------------------------------------------------------------------
  {
    // Complex ptr2[length] [YnodeSites][XnodeSites][ZnodeSites]
    // Complex ptr [length] [ZnodeSites][YnodeSites][XnodeSites]

    change_order_123_to_132((Complex*)ptr,
			    ptr2,
			    length, YnodeSites*XnodeSites, ZnodeSites);
  }

  if(ptr1) delete[] ptr1; ptr1 = NULL;
  if(ptr2) delete[] ptr2; ptr2 = NULL;
  if(ptr3) delete[] ptr3; ptr3 = NULL;
}
  

//---------------------------------------------------------------------
// for debug
//---------------------------------------------------------------------

#ifdef USE_AS_MAIN
#include <vector>
#include <string.h>
#include <mpi.h>

#include "rcnp_0000.h"
#include "rcnp_bunkatsu_xyz.h"
#include "rcnp_bunkatsu_xyzt.h"

static MPI_Comm comm_xyz;
static int      rank_xyz;
static int      size_xyz;

static void usage()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(rank == 0){
    fprintf(stderr,
	    "usage: debug_fft3d "
	    " (Xsites) (Ysites) (Zsites) (Tsites)"
	    " (Xnodes) (Ynodes) (Znodes) (Tnodes)"
	    " (nin2)   (nex)\n"
	    " (ifname) (ofname)\n");
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc,char* argv[])
{
  MPI_Init(&argc, &argv);

  argc--; argv++;

  if(argc != 12) {
    usage(); exit(1);
  }

  int Xsites = atoi(argv[0]);
  int Ysites = atoi(argv[1]);
  int Zsites = atoi(argv[2]);
  int Tsites = atoi(argv[3]);
  
  int Xnodes = atoi(argv[4]);
  int Ynodes = atoi(argv[5]);
  int Znodes = atoi(argv[6]);
  int Tnodes = atoi(argv[7]);

  int nin2   = atoi(argv[8]);
  int nex    = atoi(argv[9]);

  const char* ifname = argv[10];
  const char* ofname = argv[11];

  argc -= 12; argv += 12;

  if(argc % Tnodes != 0){
    fprintf(stderr,
	    "ERROR: Tnodes = %d must devide number of input files = %d\n",
	    Tnodes,
	    argc);
    exit(1);
  }

  using namespace fft3d;
  using namespace rcnp;

  rcnp ::initialize(Xsites,    Ysites,    Zsites,    Tsites,
		    Xnodes,    Ynodes,    Znodes,    Tnodes);

  fft3d::initialize(Xsites,    Ysites,    Zsites,    Tsites,
		    Xnodes,    Ynodes,    Znodes,    Tnodes,
		    XnodeCoor, YnodeCoor, ZnodeCoor, TnodeCoor);
  
  Complex *ptr = new Complex[nin2 *XYZTnodeSites *nex];

  rcnp::fread_xyzt(ptr, nin2, nex, ifname);

  fft3d::run(ptr, nin2, nex*TnodeSites, fft3d::Forward);

  rcnp::fwrite_xyzt(ptr, nin2, nex, ofname);

  delete[] ptr;

  fft3d::finalize();
  rcnp ::finalize();

  MPI_Finalize();

  return 0;
}

#endif
