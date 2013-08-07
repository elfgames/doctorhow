/*
*
*	Metasprite Library:
*	
*	-loadSpriteGraphics: loads in the VRAM the sprite data and returns the address of the first memory block used
*
*	-changeSprGraphics: exchange sprite data given the id (given by the previews function)
*
*	-setupSprite: sets up a metasprite given size and memory position of its data (loaded by the first function) and returns the id of the first sprite used (0-39)
*
*	-resetSprite: swaps references between a sprite data and it's metasprite
*	
*	-moveSprite: moves a metasprite
*
*/
#ifndef __CHARA_UTILS__
#define __CHARA_UTILS__

#include <gb/gb.h>

static UBYTE last_memory_id = 0;
static UBYTE last_sprite_id = 0;

UINT8 loadSpriteGraphics(const unsigned char * chara , UINT8 size);

#define changeSprGraphics(_id_,_chara_,_size_) set_sprite_data(_id_, _size_, _chara_)

UINT8 setupSprite(UINT8 memoryTileId, UINT8 width, UINT8 height);

void resetSprite(UINT8 memoryTileId, UINT8 spriteId, UINT8 width, UINT8 height);

void moveSprite(UBYTE id, UBYTE x, UBYTE y, UBYTE width, UBYTE height);

#endif