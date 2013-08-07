#include <gb/gb.h>
//#include "../main.h"
#include <gb/drawing.h>
#pragma bank=5

#define MAGIC_NUM 42
extern unsigned char high_score[];
//title screen image
extern unsigned char title[];

void title_graphics(UBYTE pstart) NONBANKED {
	SWITCH_ROM_MBC1(5);
	draw_image(title);
	if (pstart) {
		gotogxy(5, 11);
		gprintf("Press Start");
	}
	if (high_score[5]==MAGIC_NUM) {
		gotogxy(14, 0);
		gprintf("Score");
		gotogxy(14, 1);
		gprintf("%u%u%u%u%u", high_score[0],high_score[1],high_score[2],high_score[3], high_score[4]);
	}
}
void title_screen() NONBANKED {
	UBYTE counter = 0, pstart = 1, keys = 0;
	wait_vbl_done();
	disable_interrupts();
	DISPLAY_OFF;
	HIDE_SPRITES;
	HIDE_WIN;
	HIDE_BKG;
	ENABLE_RAM_MBC1;
	DISPLAY_ON;
	enable_interrupts();
	while(1) {
		wait_vbl_done();
		keys = joypad();
		if (keys & (J_A|J_START)) {
			break;
		}
		counter++;
		if (counter == 40) {
			title_graphics(pstart);
			pstart = !pstart;
			counter = 0;
		}
	}
	DISABLE_RAM_MBC1;
}
