/**
 * @file misc.h
 * @author TC (reeft137@gmail.com)
 * @brief misc = miscellaneous
 * @version 0.1
 * @date 2023-02-02
 *
 */

#ifndef IS_INCLUDED_MISC_H
#define IS_INCLUDED_MISC_H

// C libraries that are always used
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

// C++ libraries that are always used
#include <string>
/**
 * @brief Generate filename for printing
 *
 * @param nameraw Original character string of the file name: "FILENAME"
 * @param namegen Character string generated for printing: "./FILENAME"
 */
void gen_print_name(const char *nameraw, char *namegen);

/**
 * @brief Add prefix to a file (preserving the original path)
 *
 * @param nameraw Original character string of the file name: "dir/FILENAME"
 * @param prefix The prefix to be added: "PRE"
 * @param namegen Character string generated: "dir/PRE.FILENAME"
 */
void add_prefix(const char *nameraw, const char *prefix, char *namegen);

#endif
