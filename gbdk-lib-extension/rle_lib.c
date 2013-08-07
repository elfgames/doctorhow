/*
*	RLE Unpack library:
*	RLE compressed equivalents for GBDK standard functions
*
*/

#include "rle_lib.h"

void set_bkg_data_rle(UINT8 first_tile, UINT8 nb_tiles, unsigned char *data) NONBANKED {
	static UBYTE i, j, w = 255;
	static UWORD k = 0;
	static UBYTE temp_tile[16];
	for (i = 0; i != nb_tiles; ++i) {
		for (j = 0; j != 16; j+=2) {
			temp_tile[j] = data[k];
			temp_tile[j+1] = data[k+1];
			if (data[k] != data[k+2] || data[k+1] != data[k+3])
				k+=2;
			else {
				if (w == 0) {
					k+=5;
					w = 255;
					continue;
				}
				if (w == 255)
					w = data[k+4]-1;
				
				--w;
			}
		}
		
		set_bkg_data(first_tile, 1, temp_tile);
		++first_tile;
	}
}
