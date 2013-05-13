#ifndef _CPK_UNCOMPRESS_H_INCLUDED
#define _CPK_UNCOMPRESS_H_INCLUDED

#include <stdio.h>
#include "util.h"

long uncompress(reader_t *infile, long offset, long size, FILE *outfile);

long uncompress2(reader_t *infile, long offset, long input_size, FILE *outfile, long start_offset);

#endif
