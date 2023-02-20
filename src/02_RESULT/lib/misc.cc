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

#include "misc.h"

void gen_print_name(const char *ifname, char *ofname)
{
  char stmp[1024], dir[1024], base[1024];
  // dirname() and basename() will change the original string,
  // so we need to copy from ifname twice.
  strncpy(stmp, ifname, 1023);
  strncpy(dir, dirname(stmp), 1023);
  strncpy(stmp, ifname, 1023);
  strncpy(base, basename(stmp), 1023);
  snprintf(ofname, 2048, "%s/%s", dir, base);
}

void add_prefix(const char *ifname, const char *prefix, char *ofname)
{
  char stmp[1024], dir[1024], base[1024];
  strncpy(stmp, ifname, 1023);
  strncpy(dir, dirname(stmp), 1023);
  strncpy(stmp, ifname, 1023);
  strncpy(base, basename(stmp), 1023);
  snprintf(ofname, 4096, "%s/%s.%s", dir, prefix, base);
}

inline void mk755dir(const char *dir)
{
  if (mkdir(dir, 0000755))
    perror(dir);
}

inline void rm(const char *dir)
{
  if (remove(dir))
    perror(dir);
}
