/**
 * @file valarray_io.h
 * @author Tianchen (zhangtc@rcnp.osaka-u.ac.jp)
 * @brief head file for valarray_io.cc
 *        read and write valarrays from and to files
 * @version 0.1
 * @date 2023-01-17
 *
 */

#ifndef IS_INCLUDED_VALARRAY_IO_H
#define IS_INCLUDED_VALARRAY_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <valarray>

extern std::valarray<double> read_corr_double(const char *ifname, int maxrow);

extern std::valarray<double _Complex> read_corr_complex(const char *ifname, int maxrow);

extern void write_corr_double(const char *ifname, const char *prefix, int maxrow, const std::valarray<double> &valdata);

extern void write_corr_complex(const char *ifname, const char *prefix, int maxrow, const std::valarray<double _Complex> &valdata);

extern void write_result_to_txt(const char *ofname, int maxrow, const std::valarray<double _Complex> &valdata, bool judge_4pt);

#endif
