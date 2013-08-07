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

#include "chara_utils.h"

static UBYTE _cu_id, _cu_x, _cu_y, _cu_i, _cu_qy, _cu_mode, _cu_size, _cu_size2;

UINT8 loadSpriteGraphics(const unsigned char * chara , UINT8 size) {
	_cu_id = last_memory_id;
	set_sprite_data(last_memory_id, size, chara);
	last_memory_id += size;
	return _cu_id;
}

#define changeSprGraphics(_id_,_chara_,_size_) set_sprite_data(_id_, _size_, _chara_)

UINT8 setupSprite(UINT8 memoryTileId, UINT8 width, UINT8 height) {
	
	_cu_id = last_sprite_id;
	_cu_x=0;_cu_y=0;
	_cu_size = height*width;
	
	_cu_mode = 1;
	if (LCDC_REG & 0x04U) {
		_cu_mode = 2;
		_cu_size2=_cu_size>>1;
	} else
		_cu_size2=_cu_size;
	
	for (_cu_i=0;_cu_i!=_cu_size2;++_cu_i) {
		if (_cu_x==width) {
			_cu_x = 0;
			_cu_y++;
		}
		if (LCDC_REG & 0x04U)
			set_sprite_tile(_cu_id+_cu_i, memoryTileId+(_cu_i<<1));
		else
			set_sprite_tile(_cu_id+_cu_i, memoryTileId+(_cu_i));
		move_sprite(_cu_id+_cu_i, 8 + (_cu_x<<3), 16 + ((_cu_y<<3)<<(_cu_mode-1)));
		_cu_x++;
	}
	last_sprite_id += _cu_size2;
	return _cu_id;
}

void resetSprite(UINT8 memoryTileId, UINT8 spriteId, UINT8 width, UINT8 height) {
	_cu_size = width*height;
	_cu_mode = 1;
	if (LCDC_REG & 0x04U) {
		_cu_mode = 2;
		_cu_size2=_cu_size>>1;
	} else
		_cu_size2=_cu_size;
	
	for (_cu_i=0;_cu_i!=_cu_size2;++_cu_i) {
		if (LCDC_REG & 0x04U)
			set_sprite_tile(spriteId+_cu_i, memoryTileId+(_cu_i<<1));
		else
			set_sprite_tile(spriteId+_cu_i, memoryTileId+(_cu_i));
	}
}

//special move-sprite for enemies, because the other one is so slow for little charas
//move sprites to a specified position
void moveSprite(UBYTE id, UBYTE x, UBYTE y, UBYTE width, UBYTE height) {
	_cu_qy=8;
	_cu_x = 0;
	_cu_y = 0;
	_cu_size = width * height;
	if (LCDC_REG & 0x04U) {
		_cu_size = _cu_size>>1;
		_cu_qy = 16;
	}
	x += 8;
	y += 16;
	for (_cu_i=0;_cu_i!=_cu_size;++_cu_i) {
		if (_cu_x==(width<<3)) {
			_cu_x = 0;
			_cu_y+=_cu_qy;
		}
		move_sprite(id+_cu_i, _cu_x + x, _cu_y + y);
		_cu_x+=8;
	}
}
