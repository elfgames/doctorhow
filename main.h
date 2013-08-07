//defines if a save data was alredy written or not
#define MAGIC_NUM 42

void init();
void init_graphics();
void randomize_enemy(unsigned char i);
void updatePGMove();
void score_update();
void tardis_moving();
void collisions(unsigned char i);
//high score stored in the battery (sav file of the emulator)

#define N_ENEMIES 6
#define SCORE 5
//to speed up math operations
#define TARDIS_S TARDIS_W*TARDIS_H
extern unsigned char high_score[];

//chara and background variables

#include "charas/charas.h"

#include "background/bg.h"

