/*
*	RLE Unpack library:
*	RLE compressed equivalents for GBDK standard functions
*
*/
#ifndef __RLE_LIB__
#define __RLE_LIB__
#include <gb/gb.h>

void set_bkg_data_rle(UINT8 first_tile, UINT8 nb_tiles, unsigned char *data) NONBANKED;

#endif