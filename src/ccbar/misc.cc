/**
 * @file misc.cc
 * @author TC (reeft137@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

void gen_print_name(const char *ifname, char *ofname)
{
  char stmp[2048], dir[2048], base[2048];
  // dirname() and basename() will change the original string, 
  // so we need to copy from ifname twice.
  strncpy(stmp, ifname, 2047);
  strncpy(dir, dirname(stmp), 2047);
  strncpy(stmp, ifname, 2047);
  strncpy(base, basename(stmp), 2047);
  snprintf(ofname, 2047, "%s/%s", dir, base);
}

void add_prefix(const char *ifname, const char *prefix, char *ofname)
{
  char stmp[2048], dir[2048], base[2048];
  strncpy(stmp, ifname, 2047);
  strncpy(dir, dirname(stmp), 2047);
  strncpy(stmp, ifname, 2047);
  strncpy(base, basename(stmp), 2047);
  snprintf(ofname, 2047, "%s/%s.%s", dir, prefix, base);
}

inline void mkchdir(const char *destination)
{
  if (mkdir(destination, 0777))
    perror(destination);
  if (chmod(destination, 0000755))
    perror(destination);
}