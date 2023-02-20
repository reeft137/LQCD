//---------------------------------------------------------------------
/**
 * @file SOLVER_MAIN.cpp
 * @brief The main code of solver (quark propagator generator)
 *        (2-point and 4-point included)
 */
//---------------------------------------------------------------------

#include "bridge.h"

#include <mpi.h>
#include <libgen.h>
#include <complex.h>
#include <string.h>

#include <fft3d/fft3d.h>
#include <rcnp/rcnp.h>
#include <rcnp/misc.h>
#include <rcnp/rcnp_gamma.h>
#include <rcnp/rcnp_SOLVERS.h>
#include "parameters.h"
#include "SOLVER_SUB.h"
#include "qqbar.h"
#include "anti_quark.h"

using Bridge::vout;
typedef double _Complex Complex;

//---------------------------------------------------------------------
// usage function
//---------------------------------------------------------------------
static void usage(const char *program_name)
{
  vout.general("\n\nusage: %s parameter_file "
               "[OPTIONS] (ifname1) (ifname2) ...\n\n",
               program_name);

  vout.general("OPTIONS:\n"
               "      -h\n"
               "      -ofbase_2pt (basename)\n"
               "      -ofbase_4pt  (basename)\n"
               "      -T_shift     (shift in T dir)");
}

//---------------------------------------------------------------------
// 2pt correlator save function
//---------------------------------------------------------------------
void data_save_2pt(const char *infile, const char *base, char *corr_type, int T, int shift, Complex *corr)
{
  FILE *fp = stdout;
  char ofname[2048];
  char tmp[2048];
  strncpy(tmp, infile, 2048);
  sprintf(ofname, "%s.%s.sft%+03d.%s", base, corr_type, shift, basename(tmp));
  fprintf(stderr, "generating %s\n", ofname);

  if ((fp = fopen(ofname, "w")) == NULL)
  {
    fprintf(stderr, "ERROR: cannot fopen '%s' for writing\n", ofname);
    exit(1);
  }

  fwrite(&corr[0], sizeof(Complex), T, fp);
  
  fclose(fp);
}

//---------------------------------------------------------------------
// 4pt correlator save function
//---------------------------------------------------------------------
void data_save_4pt(const char *infile, const char *base, char *corr_type, int TnodeSites, int TnodeCoor, int Lxyz, int shift, MPI_Comm &comm, double *corr)
{
  for (int it = 0; it < TnodeSites; it++)
  {
    int iT = it + TnodeSites * TnodeCoor;
    char ofname[2048];
    char tmp[2048];
    strncpy(tmp, infile, 2048);
    sprintf(ofname, "%s.%s.%+03d.sft%+03d.%s", base, corr_type, iT, shift, basename(tmp));

    MPI_File fh;
    MPI_File_open(comm,
                  ofname,
                  MPI_MODE_WRONLY | MPI_MODE_CREATE,
                  MPI_INFO_NULL,
                  &fh);
    rcnp::mpiwrite_XYZ(2, 1, &corr[Lxyz * it], fh, 0);
    MPI_File_sync(fh);
    MPI_File_close(&fh);
  }
}

//---------------------------------------------------------------------
// initialization for the options
//---------------------------------------------------------------------
static const char *ofbase_prop = NULL;
static const char *ofbase_2pt = NULL;
static const char *ofbase_4pt = NULL;
int T_shift = 0;
int T_times = 1;

//---------------------------------------------------------------------
// users main
//---------------------------------------------------------------------
int users_main(const Parameters &params_all, int argc, char *argv[])
{
  using namespace std;
  using namespace parameters;

  const char *program_name = argv[0];
  MPI_Barrier(MPI_COMM_WORLD);

  argc -= 2;
  argv += 2;

  // options
  while (argc > 0 && argv[0][0] == '-')
  {
    if (strcmp(argv[0], "-h") == 0)
    {
      usage(program_name);
      exit(0);
      argc--;
      argv++;
      continue;
    }

    if (strcmp(argv[0], "-ofbase_prop") == 0)
    {
      ofbase_prop = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-ofbase_2pt") == 0)
    {
      ofbase_2pt = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-ofbase_4pt") == 0)
    {
      ofbase_4pt = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp(argv[0], "-T_shift") == 0)
    {
      T_shift = atoi(argv[1]);
      argc -= 2;
      argv += 2;
      continue;
    }

    fprintf(stderr, "ERROR: unknown option '%s'\n", argv[0]);
    exit(1);
  }
  Bridge::vout.general("@@@@@@ %s\n", argv[0]);

  //---------------------------------------------------------------------
  // initializations
  //---------------------------------------------------------------------
  parameters ::initialize(params_all);
  RandomNumberManager::initialize(str_rand_type, seed);
  rcnp::gamma::initialize();
  rcnp ::initialize(CommonParameters::Lx(), // Number of sites
                    CommonParameters::Ly(),
                    CommonParameters::Lz(),
                    CommonParameters::Lt(),

                    Communicator::npe(0), // Number of nodes
                    Communicator::npe(1),
                    Communicator::npe(2),
                    Communicator::npe(3),

                    Communicator::ipe(0), // Node coordinate
                    Communicator::ipe(1),
                    Communicator::ipe(2),
                    Communicator::ipe(3),

                    MPI_COMM_WORLD);
  fft3d::initialize(rcnp::Xsites,
                    rcnp::Ysites,
                    rcnp::Zsites,

                    rcnp::Xnodes,
                    rcnp::Ynodes,
                    rcnp::Znodes,

                    rcnp::XnodeCoor,
                    rcnp::YnodeCoor,
                    rcnp::ZnodeCoor,

                    rcnp::comm_XYZ);

  MPI_Barrier(MPI_COMM_WORLD);
  Bridge::vout.general("@@@ initializations done\n");
  MPI_Barrier(MPI_COMM_WORLD);

  //---------------------------------------------------------------------
  // parameter setup
  //---------------------------------------------------------------------
  const int Nc = CommonParameters::Nc();
  const int Nd = CommonParameters::Nd();
  const int Ndim = CommonParameters::Ndim();
  const int Nvol = CommonParameters::Nvol();

  //---------------------------------------------------------------------
  // loop over gauge configurations
  //---------------------------------------------------------------------
  for (int iarg = 0; iarg < argc; iarg++)
  {
    const char *ifname = argv[iarg];

    //---------------------------------------------------------------------
    // loop over time shifts
    //---------------------------------------------------------------------
    Bridge::vout.general("@@@@@ calculating shift times\n");
    if (T_shift)
    {
      if (rcnp::Tsites % T_shift != 0)
      {
        fprintf(stderr, "ERROR: T_shift must be a factor of Tsites!");
        exit(1);
      }
      T_times = rcnp::Tsites / T_shift;
    }
    Bridge::vout.general("shift times: %d\n", T_times);

    for (int ishift = 0; ishift < T_times; ishift++)
    {

      //---------------------------------------------------------------------
      // reading a gauge configuration
      //---------------------------------------------------------------------
      unique_ptr<Field_G> U(new Field_G(Nvol, Ndim));
      Bridge::vout.general("@@@ reading %s\n", ifname);
      GaugeConfig(str_gconf_read).read(U, string(ifname));

      //---------------------------------------------------------------------
      // time shift
      //---------------------------------------------------------------------
      if (T_shift)
      {
        Bridge::vout.general("@@@@@ do time shift\n");
        rcnp::shift(U->ptr(0), rcnp::Tdir, T_shift, 2 * 3 * 3, 4); // shift in the time direction
      }
      // rcnp::shift(U->ptr(0), rcnp::Tdir, T_shift*ishift, 2 * 3 * 3, 4); //shift in the time direction

      //---------------------------------------------------------------------
      // output plaquette
      //---------------------------------------------------------------------
      Bridge::vout.general("@@@@@ output plaquette\n");
      Staple_lex staple;
      double plaq = staple.plaquette(*U);
      Bridge::vout.general("PLAQ = %18.14f\n\n", plaq);

      //---------------------------------------------------------------------
      // propagator and anti propagator
      //---------------------------------------------------------------------
      Bridge::vout.general("@@@@@ saving propagator\n");
      double *prop = new double[2 * 3 * 4 * 3 * 4 * rcnp::XYZTnodeSites];
      double *prop_anti = new double[2 * 3 * 4 * 3 * 4 * rcnp::XYZTnodeSites];

      // rcnp::invoke_solvers(U.get(), prop, params_fopr, params_solver, params_source);
      rcnp::invoke_solvers_eo(U.get(), prop, params_fopr, params_solver, params_source);
      rcnp::anti_quark_propagator((double *)prop, (double *)prop_anti, rcnp::XYZTnodeSites);

      if (ofbase_prop)
      {
        char ofname[2048];
        char tmp[2048];
        strncpy(tmp, ifname, 2048);
        sprintf(ofname, "%s.%s", ofbase_prop, basename(tmp));

        Bridge::vout.general("generating %s\n", ofname);
        MPI_File fh;
        MPI_File_open(MPI_COMM_WORLD,
                      ofname,
                      MPI_MODE_WRONLY | MPI_MODE_CREATE,
                      MPI_INFO_NULL,
                      &fh);
        rcnp::mpiwrite_XYZT(2 * 3 * 4 * 3 * 4, 1, prop, fh, 0);
        MPI_File_close(&fh);
      }

      MPI_Barrier(MPI_COMM_WORLD);

      // gamma
      rcnp::gamma GI(0, 1, 2, 3, 1.0, 1.0, 1.0, 1.0);
      rcnp::gamma v_gamma[] = {rcnp::gamma::G[1],
                               rcnp::gamma::G[2],
                               rcnp::gamma::G[3]};
      rcnp::gamma t_gamma[] = {rcnp::gamma::G[1] * rcnp::gamma::G[2],
                               rcnp::gamma::G[2] * rcnp::gamma::G[3],
                               rcnp::gamma::G[3] * rcnp::gamma::G[1]};
      rcnp::gamma av_gamma[] = {rcnp::gamma::G[1] * rcnp::gamma::G5,
                                rcnp::gamma::G[2] * rcnp::gamma::G5,
                                rcnp::gamma::G[3] * rcnp::gamma::G5};

      //---------------------------------------------------------------------
      // 2-pt correlator
      //---------------------------------------------------------------------
      Bridge::vout.general("@@@@@ start 2-pt calculation\n");
      if (ofbase_2pt)
      {
        Complex tmp[rcnp::Tsites];
        Complex avr_tmp[rcnp::Tsites];
        Complex data_R[rcnp::Tsites];

        //---**************************---
        //             scalar
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < rcnp::Tsites; i++)
        {
          tmp[i] = (0, 0);
          avr_tmp[i] = (0, 0);
          data_R[i] = (0, 0);
        }

        corr_twopoint_qqbar((double *)&tmp[rcnp::TnodeCoor * rcnp::TnodeSites],
                            (double *)prop,
                            (double *)prop_anti,
                            GI,
                            GI,
                            rcnp::XYZnodeSites,
                            rcnp::TnodeSites);

        MPI_Reduce((double *)tmp, (double *)data_R, 2 * rcnp::Tsites, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rcnp::XYZTnodeCoor == 0)
        {
          data_save_2pt(ifname, ofbase_2pt, "s", rcnp::Tsites, T_shift * ishift, data_R);
        }

        //---**************************---
        //             vector
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < rcnp::Tsites; i++)
        {
          data_R[i] = (0, 0);
        }

        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < rcnp::Tsites; j++)
          {
            tmp[j] = (0, 0);
            avr_tmp[j] = (0, 0);
          }
          corr_twopoint_qqbar((double *)&tmp[rcnp::TnodeCoor * rcnp::TnodeSites],
                              (double *)prop,
                              (double *)prop_anti,
                              v_gamma[i],
                              v_gamma[i],
                              rcnp::XYZnodeSites,
                              rcnp::TnodeSites);

          MPI_Reduce((double *)tmp, (double *)avr_tmp, 2 * rcnp::Tsites, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

          if (rcnp::XYZTnodeCoor == 0)
          {
            for (int vi = 0; vi < rcnp::Tsites; vi++)
            {
              data_R[vi] += 1.0 / 3.0 * avr_tmp[vi];
            }
          }
        }
        if (rcnp::XYZTnodeCoor == 0)
        {
          data_save_2pt(ifname, ofbase_2pt, "v", rcnp::Tsites, T_shift * ishift, data_R);
        }

        //---**************************---
        //             tensor
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < rcnp::Tsites; i++)
        {
          data_R[i] = (0, 0);
        }

        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < rcnp::Tsites; j++)
          {
            tmp[j] = (0, 0);
            avr_tmp[j] = (0, 0);
          }

          corr_twopoint_qqbar((double *)&tmp[rcnp::TnodeCoor * rcnp::TnodeSites],
                              (double *)prop,
                              (double *)prop_anti,
                              t_gamma[i],
                              t_gamma[i],
                              rcnp::XYZnodeSites,
                              rcnp::TnodeSites);

          MPI_Reduce((double *)tmp, (double *)avr_tmp, 2 * rcnp::Tsites, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

          if (rcnp::XYZTnodeCoor == 0)
          {
            for (int vi = 0; vi < rcnp::Tsites; vi++)
            {
              data_R[vi] += 1.0 / 3.0 * avr_tmp[vi];
            }
          }
        }
        if (rcnp::XYZTnodeCoor == 0)
        {
          data_save_2pt(ifname, ofbase_2pt, "t", rcnp::Tsites, T_shift * ishift, data_R);
        }

        //---**************************---
        //           axialvector
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < rcnp::Tsites; i++)
        {
          data_R[i] = (0, 0);
        }

        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < rcnp::Tsites; j++)
          {
            tmp[j] = (0, 0);
            avr_tmp[j] = (0, 0);
          }

          corr_twopoint_qqbar((double *)&tmp[rcnp::TnodeCoor * rcnp::TnodeSites],
                              (double *)prop,
                              (double *)prop_anti,
                              av_gamma[i],
                              av_gamma[i],
                              rcnp::XYZnodeSites,
                              rcnp::TnodeSites);

          MPI_Reduce((double *)tmp, (double *)avr_tmp, 2 * rcnp::Tsites, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

          if (rcnp::XYZTnodeCoor == 0)
          {
            for (int vi = 0; vi < rcnp::Tsites; vi++)
            {
              data_R[vi] += 1.0 / 3.0 * avr_tmp[vi];
            }
          }
        }
        if (rcnp::XYZTnodeCoor == 0)
        {
          data_save_2pt(ifname, ofbase_2pt, "av", rcnp::Tsites, T_shift * ishift, data_R);
        }

        //---**************************---
        //           psuedoscalar
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < rcnp::Tsites; i++)
        {
          data_R[i] = (0, 0);
          tmp[i] = (0, 0);
          avr_tmp[i] = (0, 0);
        }

        corr_twopoint_qqbar((double *)&tmp[rcnp::TnodeCoor * rcnp::TnodeSites],
                            (double *)prop,
                            (double *)prop_anti,
                            rcnp::gamma::G5,
                            rcnp::gamma::G5,
                            rcnp::XYZnodeSites,
                            rcnp::TnodeSites);

        MPI_Reduce((double *)tmp, (double *)data_R, 2 * rcnp::Tsites, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rcnp::XYZTnodeCoor == 0)
        {
          data_save_2pt(ifname, ofbase_2pt, "ps", rcnp::Tsites, T_shift * ishift, data_R);
        }
      }

      //---------------------------------------------------------------------
      // 4-pt correlator
      //---------------------------------------------------------------------
      Bridge::vout.general("@@@@@ start 4-pt calculation\n");
      if (ofbase_4pt)
      {
#define FourPoint(ptr, ixyz, it) ((Complex *)ptr)[ixyz + rcnp::XYZnodeSites * (it)]

        double *avr_temp = new double[2 * rcnp::XYZnodeSites * rcnp::TnodeSites];
        double *result = new double[2 * rcnp::XYZnodeSites * rcnp::TnodeSites];
        for (int i = 0; i < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; i++)
        {
          avr_temp[i] = 0.0;
          result[i] = 0.0;
        }

        // FFT
        run((double *)prop, 3 * 4 * 3 * 4, rcnp::TnodeSites, fft3d::Backward);
        run((double *)prop_anti, 3 * 4 * 3 * 4, rcnp::TnodeSites, fft3d::Forward);

        //---**************************---
        //             scalar
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; i++)
        {
          avr_temp[i] = 0.0;
          result[i] = 0.0;
        }

        corr_fourpoint_qqbar((double *)result,
                             (double *)prop,
                             (double *)prop_anti,
                             GI,
                             GI,
                             rcnp::XYZnodeSites,
                             rcnp::TnodeSites);

        run(result, 1, rcnp::TnodeSites, fft3d::Backward);
        for (int i = 0; i < 2 * rcnp::XYZTnodeSites; i++)
        {
          result[i] /= double(rcnp::XYZsites * rcnp::XYZsites);
        }
        data_save_4pt(ifname, ofbase_4pt, "s", rcnp::TnodeSites, rcnp::TnodeCoor, rcnp::XYZnodeSites, T_shift * ishift, rcnp::comm_XYZ, result);

        //---**************************---
        //             vector
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; i++)
        {
          result[i] = 0.0;
        }

        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; j++)
          {
            avr_temp[j] = 0.0;
          }
          corr_fourpoint_qqbar((double *)avr_temp,
                               (double *)prop,
                               (double *)prop_anti,
                               v_gamma[i],
                               v_gamma[i],
                               rcnp::XYZnodeSites,
                               rcnp::TnodeSites);

          run(avr_temp, 1, rcnp::TnodeSites, fft3d::Backward);
          for (int vi = 0; vi < 2 * rcnp::XYZTnodeSites; vi++)
          {
            avr_temp[vi] /= double(rcnp::XYZsites * rcnp::XYZsites);
            result[vi] += 1.0 / 3.0 * avr_temp[vi];
          }
        }

        data_save_4pt(ifname, ofbase_4pt, "v", rcnp::TnodeSites, rcnp::TnodeCoor, rcnp::XYZnodeSites, T_shift * ishift, rcnp::comm_XYZ, result);

        //---**************************---
        //             tensor
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; i++)
        {
          result[i] = 0.0;
        }

        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; j++)
          {
            avr_temp[j] = 0.0;
          }
          corr_fourpoint_qqbar((double *)avr_temp,
                               (double *)prop,
                               (double *)prop_anti,
                               t_gamma[i],
                               t_gamma[i],
                               rcnp::XYZnodeSites,
                               rcnp::TnodeSites);

          run(avr_temp, 1, rcnp::TnodeSites, fft3d::Backward);
          for (int vi = 0; vi < 2 * rcnp::XYZTnodeSites; vi++)
          {
            avr_temp[vi] /= double(rcnp::XYZsites * rcnp::XYZsites);
            result[vi] += 1.0 / 3.0 * avr_temp[vi];
          }
        }

        data_save_4pt(ifname, ofbase_4pt, "t", rcnp::TnodeSites, rcnp::TnodeCoor, rcnp::XYZnodeSites, T_shift * ishift, rcnp::comm_XYZ, result);

        //---**************************---
        //          axialvector
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; i++)
        {
          result[i] = 0.0;
        }

        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; j++)
          {
            avr_temp[j] = 0.0;
          }
          corr_fourpoint_qqbar((double *)avr_temp,
                               (double *)prop,
                               (double *)prop_anti,
                               av_gamma[i],
                               av_gamma[i],
                               rcnp::XYZnodeSites,
                               rcnp::TnodeSites);

          run(avr_temp, 1, rcnp::TnodeSites, fft3d::Backward);
          for (int vi = 0; vi < 2 * rcnp::XYZTnodeSites; vi++)
          {
            avr_temp[vi] /= double(rcnp::XYZsites * rcnp::XYZsites);
            result[vi] += 1.0 / 3.0 * avr_temp[vi];
          }
        }

        data_save_4pt(ifname, ofbase_4pt, "av", rcnp::TnodeSites, rcnp::TnodeCoor, rcnp::XYZnodeSites, T_shift * ishift, rcnp::comm_XYZ, result);

        //---**************************---
        //         pseudoscalar
        //---**************************---
        MPI_Barrier(MPI_COMM_WORLD);

        for (int i = 0; i < 2 * rcnp::XYZnodeSites * rcnp::TnodeSites; i++)
        {
          avr_temp[i] = 0.0;
          result[i] = 0.0;
        }

        corr_fourpoint_qqbar((double *)result,
                             (double *)prop,
                             (double *)prop_anti,
                             rcnp::gamma::G5,
                             rcnp::gamma::G5,
                             rcnp::XYZnodeSites,
                             rcnp::TnodeSites);

        run(result, 1, rcnp::TnodeSites, fft3d::Backward);
        for (int i = 0; i < 2 * rcnp::XYZTnodeSites; i++)
        {
          result[i] /= double(rcnp::XYZsites * rcnp::XYZsites);
        }
        data_save_4pt(ifname, ofbase_4pt, "ps", rcnp::TnodeSites, rcnp::TnodeCoor, rcnp::XYZnodeSites, T_shift * ishift, rcnp::comm_XYZ, result);

        delete[] result;
        delete[] avr_temp;
#undef FourPoint
      }
      delete[] prop;
      delete[] prop_anti;
    } // for(int ishift = 0; ...) (loop over time shifts)
  }   // (int iarg = 0; ...) (loop over gauge configurations)

  //---------------------------------------------------------------------
  // finalizations
  //---------------------------------------------------------------------
  rcnp::finalize();
  RandomNumberManager::finalize();
  parameters::finalize();
  rcnp::gamma::finalize();
  fft3d::finalize();

  MPI_Barrier(MPI_COMM_WORLD);
  Bridge::vout.general("@@@ finalizations done\n");
  Bridge::vout.general("@@@ shigoowa\n");
  MPI_Barrier(MPI_COMM_WORLD);

  return EXIT_SUCCESS;
}

//====================================================================
int main(int argc, char *argv[])
{
  bridge_initialize(&argc, &argv);

  if (argc < 2)
  {
    usage(argv[0]);

    bridge_finalize();
    return EXIT_FAILURE;
  }

  std::string parameter_file = std::string(argv[1]);

  Parameters params = ParameterManager::read(parameter_file);
  bridge_setup(params.lookup("Main"));

  Timer timer("Main");
  timer.start();

  users_main(params, argc, argv);

  timer.stop();
  timer.report();

  bridge_finalize();

  return EXIT_SUCCESS;
}
