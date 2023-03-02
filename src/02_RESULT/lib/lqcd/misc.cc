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

void gen_print_name(const char *nameraw, char *namegen)
{
  char stmp[1024], dir[1024], base[1024];
  // dirname() and basename() will change the original string,
  // so we need to copy from nameraw twice.
  strncpy(stmp, nameraw, 1023);
  strncpy(dir, dirname(stmp), 1023);
  strncpy(stmp, nameraw, 1023);
  strncpy(base, basename(stmp), 1023);
  snprintf(namegen, 2048, "%s/%s", dir, base);
}

void add_prefix(const char *nameraw, const char *prefix, char *namegen)
{
  char stmp[1024], dir[1024], base[1024];
  strncpy(stmp, nameraw, 1023);
  strncpy(dir, dirname(stmp), 1023);
  strncpy(stmp, nameraw, 1023);
  strncpy(base, basename(stmp), 1023);
  snprintf(namegen, 4096, "%s/%s.%s", dir, prefix, base);
}
