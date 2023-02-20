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

/**
 * @brief Generate filename for printing
 * 
 * @param ifname Original character string of the file name: "FILENAME"
 * @param ofname Character string generated for printing: "./FILENAME"
 */
void gen_print_name(const char *ifname, char *ofname);

/**
 * @brief Add prefix to a file (preserving the original path)
 * 
 * @param ifname Original character string of the file name: "dir/FILENAME"
 * @param prefix The prefix to be added: "PRE"
 * @param ofname Character string generated: "dir/PRE.FILENAME"
 */
void add_prefix(const char *ifname, const char *prefix, char *ofname);

/**
 * @brief Make directory with ownership 755
 * 
 * @param dir 
 */
void mk755dir(const char *dir);

/**
 * @brief Remove a Directory
 * 
 * @param dir 
 */
void rm(const char *dir);

#endif
