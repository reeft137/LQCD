/**
 * @file gen_average.cc
 * @author TC (reeft137@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-05
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
  // Throw away the information of the command itself
  argc--;
  argv++;

  const char *gauge_type[] = {"C", "L"};
  const char *bilinear_type[] = {"ps", "v"};

  const int time_sites = 64;
  const int space_sites = 32;
  const int shift_interval = 8;
  const int shift_count = time_sites / shift_interval;
  if (time_sites % shift_interval != 0)
  {
    fprintf(stderr, "wrong shift_interval! \n");
    exit(1);
  }

  const char *datalist[] = {
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001200",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001220",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001240",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001260",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001280",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001300",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001320",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001340",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001360",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001380",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001400",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001420",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001440",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001460",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001480",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001500",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001520",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001540",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001560",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001580",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001600",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001620",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001640",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001660",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001680",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001700",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001720",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001740",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001760",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001780",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001800",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001820",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001840",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001860",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001880",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001900",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001920",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001940",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001960",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-001980",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-002000",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-002020",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-002040",
      "RC32x64_B1900Kud01378100Ks01364000C1715-gM-002060",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001240",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001260",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001280",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001300",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001320",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001340",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001360",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001380",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001400",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001420",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001440",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001460",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001480",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001500",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001520",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001540",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001560",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001580",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001600",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001620",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001640",
      "RC32x64_B1900Kud01378100Ks01364000C1715-hM-001660",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-000870",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-000890",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-000910",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-000930",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-000950",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-000970",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-000990",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001010",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001030",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001050",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001070",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001090",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001110",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001130",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001150",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001170",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001190",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001210",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001230",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001250",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001270",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001290",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001310",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001330",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001350",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001370",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001390",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001410",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001430",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001450",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001470",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001490",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001510",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001530",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001550",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001570",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001590",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001610",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001630",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001650",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001670",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001690",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001710",
      "RC32x64_B1900Kud01378100Ks01364000C1715-iM-001730",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000260",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000280",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000300",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000320",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000340",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000360",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000380",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000400",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000420",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000440",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000460",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000480",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000500",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000520",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000540",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000560",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000580",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000600",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000620",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000640",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000660",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000680",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000700",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000720",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000740",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000760",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000780",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000800",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000820",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000840",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000860",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000880",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000900",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000920",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000940",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000960",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-000980",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-001000",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-001020",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-001040",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-001060",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-001080",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-001100",
      "RC32x64_B1900Kud01378100Ks01364000C1715-jM-001120",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001090",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001110",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001130",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001150",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001170",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001190",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001210",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001230",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001250",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001270",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001290",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001310",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001330",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001350",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001370",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001390",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001410",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001430",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001450",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001470",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001490",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001510",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001530",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001550",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001570",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001590",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001610",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001630",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001650",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001670",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001690",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001710",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001730",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001750",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001770",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001790",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001810",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001830",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001850",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001870",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001890",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001910",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001930",
      "RC32x64_B1900Kud01378100Ks01364000C1715-kM-001950",
  };

  const int datalist_length = sizeof(datalist) / sizeof(datalist[0]);
  const int gauge_length = sizeof(gauge_type) / sizeof(gauge_type[0]);
  const int bilinear_length = sizeof(bilinear_type) / sizeof(bilinear_type[0]);

  int spacetotal = space_sites * space_sites * space_sites;

  for (int igauge = 0; igauge < gauge_length; igauge++)
    for (int ibil = 0; ibil < bilinear_length; ibil++)
    {
      char ofname[2048];
      snprintf(ofname, 2048, "%s.AVE_%s", bilinear_type[ibil], gauge_type[igauge]);

      FILE *fp = NULL;
      fp = fopen(ofname, "w");
      if (fp == NULL)
      {
        perror(ofname);
        exit(1);
      }

      fprintf(fp, "#!/bin/bash\n\n");
      fprintf(fp, "LQCD_BASE_DIR=/octfs/work/G14458/u6b229/bridge-1.5.3_XeonPhi/work/data\n\n");
      fprintf(fp, "cd $1\n\n");

      for (int idata = 0; idata < datalist_length; idata++)
      {
        for (int itime = 0; itime < time_sites; itime++)
        {
          fprintf(fp, "$LQCD_BASE_DIR/pre -maxline %d \\\n", spacetotal);
          fprintf(fp, "-ofname 4pt.%s.%+03ld.gfix_%s.%s \\\n", bilinear_type[ibil], itime, gauge_type[igauge], datalist[idata]);
          for (int ishift = 0; ishift < shift_count; ishift++)
          {
            fprintf(fp, "4pt.%s.%+03ld.sft%+03ld.gfix_%s.%s \\\n", bilinear_type[ibil], itime, int(ishift * shift_interval), gauge_type[igauge], datalist[idata]);
          }
          fprintf(fp, ";\n\n");
        }

        for (int itime = 0; itime < time_sites/2; itime++)
        {
          fprintf(fp, "$LQCD_BASE_DIR/pre -maxline %d \\\n", spacetotal);
          fprintf(fp, "-ofname TR.4pt.%s.%+03ld.gfix_%s.%s \\\n", bilinear_type[ibil], itime, gauge_type[igauge], datalist[idata]);
          fprintf(fp, "4pt.%s.%+03ld.gfix_%s.%s \\\n", bilinear_type[ibil], itime, gauge_type[igauge], datalist[idata]);
          fprintf(fp, "4pt.%s.%+03ld.gfix_%s.%s \\\n", bilinear_type[ibil], int(63 - itime), gauge_type[igauge], datalist[idata]);
          fprintf(fp, ";\n\n");
        }
      }

      if (chmod(ofname, 0000755))
        perror(ofname);
    }

  return 0;
}
