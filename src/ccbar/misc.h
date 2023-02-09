/**
 * @file misc.h
 * @author TC (reeft137@gmail.com)
 * @brief Functions deal with the strings and more!
 * @version 0.1
 * @date 2023-02-02
 * 
 */

#ifndef IS_INCLUDED_MISC_H
#define IS_INCLUDED_MISC_H

// C libraries
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

// Generate filename for printing
void gen_print_name(const char *ifname, char *ofname);

// Add prefix to a file path ("dir/prefix.base")
void add_prefix(const char *ifname, const char *prefix, char *ofname);

// Make directory and change the ownership
void mkchdir(const char *destination);
#endif
