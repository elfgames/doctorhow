//to setup a sprite
static UBYTE last_memory_id = 0;
static UBYTE last_sprite_id = 0;
static UBYTE _cu_id, _cu_x, _cu_y, _cu_i, _cu_mode, _cu_size, _cu_size2;

/*
UINT8 loadSpriteGraphics(const UBYTE * chara, UINT8 size) {
	cu_id = last_memory_id;
	set_sprite_data(_cu_id, size, chara);
	_cu_id = last_memory_id + size;
}
*/

#define loadSpriteGraphics(_chara_,_size_) last_memory_id; set_sprite_data(last_memory_id, _size_, _chara_); last_memory_id += _size_
#define changeSprGraphics(_id_,_chara_,_size_) set_sprite_data(_id_, _size_, _chara_)

UINT8 setupSprite(UINT8 memoryTileId, UINT8 width, UINT8 height) {
	
	//UINT8 _cu_id, _cu_x, _cu_y, _cu_i, _cu_mode, _cu_size, _cu_id2, _cu_size2;
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
		//move_sprite(id2+i, _x*8 + 8, _y*8*mode + 16); next line is equivalent to this one
		move_sprite(_cu_id+_cu_i, 8 + (_cu_x<<3), 16 + ((_cu_y<<3)<<(_cu_mode-1)));
		_cu_x++;
	}
	last_sprite_id = last_sprite_id + _cu_size;
	return _cu_id;
}

void resetSprite(UINT8 memoryTileId, UINT8 spriteId, UINT8 width, UINT8 height) {
	
	//UINT8 _cu_id, _cu_x, _cu_y, _cu_i, _cu_mode, _cu_size, _cu_id2, _cu_size2;
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
		//move_sprite(id2+i, _x*8 + 8, _y*8*mode + 16); next line is equivalent to this one
	}
}

//special move-sprite for enemies, because the other one is so slow for little charas
#define fastMoveSprite(_id_,_x_,_y_) move_sprite(_id_, _x_, _y_+16);move_sprite(_id_+1, _x_, _y_+24)
/*void fastMoveSprite(UBYTE id, UBYTE x, UBYTE y) {
	move_sprite(id, x, y+16);
	move_sprite(id+1, x, y+24);
}*/

//move sprites to a specified position
static UBYTE _cu_qy;
void moveSprite(UBYTE id, UBYTE x, UBYTE y, UBYTE width, UBYTE height) {
	_cu_qy=8;
	_cu_x = 0;
	_cu_y = 0;
	_cu_size = width * height;
	if (LCDC_REG & 0x04U) {
		//_cu_mode = 2;
		id = id>>1;
		_cu_size = _cu_size>>1;
		_cu_qy = 4;
	}
	x += 8;
	y += 16;
	for (_cu_i=0;_cu_i!=_cu_size;++_cu_i) {
		if (_cu_x==(width<<3)) {
			_cu_x = 0;
			_cu_y+=_cu_qy;
		}
		//move_sprite(id2+i, _x*8 + (x) + 8, _y*8*mode + (y) + 16); //next line is equivalent to this one
		move_sprite(id+_cu_i, _cu_x + x, _cu_y + y);
		_cu_x+=8;
	}
}
