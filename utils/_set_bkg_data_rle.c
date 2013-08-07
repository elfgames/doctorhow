UBYTE i, j, w;
UWORD k;
static UBYTE temp_tile[16];

void set_bkg_data_rle(UINT8 first_tile, UINT8 nb_tiles, unsigned char *data) NONBANKED {
	k = 0, w = 255;
	//itero finché non popolo tutti i tile
	for (i = 0; i != nb_tiles; ++i) {
		// partendo da k = 0 itero il data generando i 16 byte da cui è comporto un tile
		for (j = 0; j != 16; j+=2) {
			temp_tile[j] = data[k];
			temp_tile[j+1] = data[k+1];
			//se il precedente e il successivo sono diversi procedo shallamente
			if (data[k] != data[k+2] || data[k+1] != data[k+3])
				k+=2;
			else {
			//altrimenti: se w è 0 vuol dire che ho già pigliato i tile w volte e ho finito, quindi posso procedere. Se procedo w andrà a 255 e j aumenterà
				if (w == 0) {
					k+=5;
					w = 255;
					continue;
				}
				// se sono all'inizio piglio il data
				if (w == 255)
					w = data[k+4]-1;
				
				--w;
			}
		}
		
		set_bkg_data(first_tile, 1, temp_tile);
		++first_tile;
	}
}
