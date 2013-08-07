#include <gb/gb.h>
#include <gb/drawing.h>
#include <rand.h>
//music library by Lemon
#include "utils/music.h"
//generical functions to simply display and move charas
#include "gbdk-lib-extension/rle_lib.h"
#include "gbdk-lib-extension/chara_utils.h"
//functions for resetting graphics and so on
#include "utils/reset_graphics.c"


#include "title_screen/title_screen.h"

#define fastMoveSprite(_id_,_x_,_y_) move_sprite(_id_, _x_, _y_+16);move_sprite(_id_+1, _x_, _y_+24)


//definition of gameplay functions
#include "main.h"

//keys variabiles, life, immortality after-hit delay, music clock and global timer
UBYTE keys, life, immortal, _timer, clock01, i, j;

//generical chara structure: id of chara, position, graphics and if can shots or not
struct PG {
	UBYTE id;
	UBYTE x;
	UBYTE y;
	UBYTE graphic;
	UBYTE canshot;
};
//enemies +1 (the shot of daleks)
struct PG enemies[N_ENEMIES+1];
struct PG player;
//actual score
unsigned char score_array[SCORE];
struct PG * shot;

UBYTE sprTardis, sprDalek, sprMeteor, sprShot;

void main() {
	UBYTE i=0, enemy_count=0;
	//various initializations
	init();
	//display title screen
	title_screen();
	//reset all graphics
	DISPLAY_OFF;
	disable_APA();
	disable_interrupts();
	SPRITES_8x8;
	HIDE_SPRITES;
	HIDE_BKG;
	HIDE_WIN;
	init_graphics();
	//set up screen
	SHOW_BKG;
	SHOW_WIN;
	SHOW_SPRITES;
	DISPLAY_ON;
	enable_interrupts();
	//graphic loop start
	while(1) {
		//timing system: sycronize with vertical blank to draw graphics and set _timers
		wait_vbl_done();
		keys = joypad();
		_timer++;
		//run music
		if (clock01==3){clock01=0;}      // set the speed of your music
		if (!clock01) music();
		clock01++;
		//scrolling and moving tardis
		scroll_bkg(2, 0);
		tardis_moving();
		if ((_timer & 15) == 0) {
			SWITCH_ROM_MBC1(3);
			score_update();
			if (enemy_count != N_ENEMIES) enemy_count++;
		}
		SWITCH_ROM_MBC1(2);
		for (i=0; i!=enemy_count; ++i) {
			collisions(i);
			--enemies[i].x;
			fastMoveSprite(enemies[i].id, enemies[i].x, enemies[i].y);
			if (enemies[i].x == 255) {
				randomize_enemy(i);
				//resetSprite();
				resetSprite(enemies[i].graphic, enemies[i].id, ENEMY_W, ENEMY_H);
				//changeSprGraphics(enemies[i].id, enemies[i].graphic, 2);
			}
		}
		if (shot->canshot == 0) {
			collisions(N_ENEMIES);
			shot->x--;
			collisions(N_ENEMIES);
			shot->x--;
			if (shot->x == 0 || shot->x == 255) {
				shot->x = 0;
				shot->canshot = 1;
			}
			fastMoveSprite(shot->id, shot->x, shot->y);
		}
		//rotating tardis
		updatePGMove();
	}
}
void init_graphics() NONBANKED {
	UBYTE i=0;
	//background
	//swap to third bank memory
	SWITCH_ROM_MBC1(3);
	//start background loading
	set_bkg_data_rle(0, (SPAZIO_W*SPAZIO_H)-1, spazio_tiledata_rle);
	//set_bkg_data(0, (SPAZIO_W*SPAZIO_H)-1, spazio_tiledata);
	set_bkg_tiles(0, 0, 32, 17, spazio_tilemap);
	//hud
	set_win_data(SPAZIO_W*SPAZIO_H, WINDOW_W, window_tiledata);
	set_win_tiles(0, 0, WINDOW_MAP_W, 1, window_tilemap);
	move_win(7, 136);

	//Charas
	shot = &enemies[N_ENEMIES];
	//switch to second bank memory
	SWITCH_ROM_MBC1(2);
	//start tardis loading
	sprTardis = loadSpriteGraphics(tardis1_tiledata, TARDIS_W*TARDIS_H);
	player.id = setupSprite(sprTardis, TARDIS_W, TARDIS_H);
	player.x = 10;
	player.y = 72;
	moveSprite(player.id, player.x, player.y, TARDIS_W, TARDIS_H);
	//load enemies
	sprDalek = loadSpriteGraphics(dalek_tiledata, ENEMY_W*ENEMY_H);
	sprMeteor = loadSpriteGraphics(meteora_tiledata, ENEMY_W*ENEMY_H);
	for (i=0; i!=N_ENEMIES; ++i) {
		randomize_enemy(i);
		enemies[i].id = setupSprite(enemies[i].graphic, ENEMY_W, ENEMY_H);
		moveSprite(enemies[i].id, enemies[i].x, enemies[i].y, ENEMY_W, ENEMY_H);
	}
	//setup dalek shot
	sprShot = loadSpriteGraphics(sparo_tiledata, ENEMY_W*ENEMY_H);
	shot->id = setupSprite(sprShot, ENEMY_W, ENEMY_H);
	shot->canshot=1;
	fastMoveSprite(shot->id, 255, 255);
}
void init() NONBANKED {
	//random seed
	UWORD seed;
	seed = DIV_REG;
	wait_vbl_done(); //wait a clock timer
	seed |= (UWORD)DIV_REG << 8;
	initrand(seed);
	_timer = 0;
	//music variables and functions
	clock01=0;
	stopmusic();
	patern_definition();
	//reset scores, life and counters
	for (i=0;i!=5;++i) score_array[i] = 0;
	life = 3;
	immortal = 0;
}
void collisions(UBYTE i) NONBANKED {
	UBYTE ok=0;
	if (immortal) return;
	if (enemies[i].y>player.y && enemies[i].y<player.y+32) {
		if (player.x+16 == enemies[i].x) {
			if (life) --life;
			else {
				ENABLE_RAM_MBC1;
				//SWITCH_RAM_MBC1(0);
				if (high_score[0]==0xFF) ok=1;
				for (j=0;j!=SCORE;++j) {
					if(high_score[j]>score_array[j] && ok==0) break;
					if (high_score[j]<score_array[j] || ok) {
						high_score[5]=MAGIC_NUM;
						high_score[j] = score_array[j];
						ok = 1;
					}
				}
				stopmusic();
				DISPLAY_OFF;
				disable_APA();
				gotogxy(5, 5);
				gprintf("GAME OVER");
				gotogxy(7, 7);
				DISPLAY_ON;
				gprintf("%u%u%u%u%u", score_array[0],score_array[1],score_array[2],score_array[3], score_array[4]);
				waitpad(J_A | J_B | J_START | J_SELECT);
				waitpadup();
				DISPLAY_OFF;
				DISABLE_RAM_MBC1;
				reset();
			}
			immortal = 100;
			SWITCH_ROM_MBC1(3);
			set_win_tiles(19-life, 0, 1, 1, &const_num[10]);
			return;
		}
		if (i == N_ENEMIES) return;
		if (enemies[N_ENEMIES].canshot && enemies[i].canshot) {
			enemies[N_ENEMIES].x = enemies[i].x;
			enemies[N_ENEMIES].y = enemies[i].y;
			//enemies[i].canshot = 0;
			enemies[N_ENEMIES].canshot = 0;
		}
	}
}
void randomize_enemy(UBYTE i) {
	UINT8 r, r2;
	//init random seed
	r = rand() & 1; // equivalente a % 2
	r2 = (rand() & 7) + (rand() & 7);
	enemies[i].x=180U;
	enemies[i].y=8+(r2<<3);
	if (r == 0) {
		enemies[i].graphic = sprMeteor;
		enemies[i].canshot = 0;
	} else {
		enemies[i].graphic = sprDalek;
		enemies[i].canshot = 1;
	}
}
void updatePGMove() {
	static UBYTE _timer2 = 0;
	_timer2++;
	if(_timer2==61) _timer2=0;
	if(_timer2==0) {
		changeSprGraphics(0, tardis1_tiledata, TARDIS_S);
	}
	else if(_timer2==10) {
		changeSprGraphics(0, tardis2_tiledata, TARDIS_S);
	}
	else if(_timer2==20) {
		changeSprGraphics(0, tardis3_tiledata, TARDIS_S);
	}
	else if(_timer2==30) {
		changeSprGraphics(0, tardis4_tiledata, TARDIS_S);
	}
	else if(_timer2==40) {
		changeSprGraphics(0, tardis5_tiledata, TARDIS_S);
	}
	else if(_timer2==50) {
		changeSprGraphics(0, tardis6_tiledata, TARDIS_S);
	}
}
void score_update() {
	UBYTE i = 0;
	score_array[4]++;
	if (score_array[4] == 10) {
		score_array[4] = 0;
		score_array[3]++;
		if (score_array[3] == 10) {
			score_array[3] = 0;
			score_array[2]++;
			if (score_array[2] == 10) {
				score_array[2] = 0;
				score_array[1]++;
				if (score_array[1] == 10) {
					score_array[1] = 0;
					score_array[0]++;
				}
			}
		}
	}
	SWITCH_ROM_MBC1(3);
	for (i=0; i!=SCORE; ++i) {
		set_win_tiles(6+i, 0, 1, 1, &const_num[score_array[i]]);
	}
}
void tardis_moving() {
	UBYTE im;
	if (keys & J_DOWN) {
		if (player.y != 104) ++player.y; // 112 = 144 - (altezza_tardis * 8)
	}
	if (keys & J_UP) {
		if (player.y != 0) --player.y;
	}
	if (keys & J_LEFT) {
		if (player.x != 0) --player.x;
	}
	if (keys & J_RIGHT) {
		if (player.x != 136) ++player.x; // 136 = 144 - (larghezza_tardis * 8)
	}
	if (immortal==0) {
		moveSprite(player.id, player.x, player.y, TARDIS_W, TARDIS_H);
		return;
	}
	--immortal;
	im = immortal & 4;
	if (im) moveSprite(player.id, 161, player.y, TARDIS_W, TARDIS_H);
	else moveSprite(player.id, player.x, player.y, TARDIS_W, TARDIS_H);
}
