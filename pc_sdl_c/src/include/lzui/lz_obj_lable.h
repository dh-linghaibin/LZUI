//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_OBJ_LABLE_H
#define LZUI_LZ_OBJ_LABLE_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

struct font_text_t {
	char * text;
};

struct font_t {
	uint16_t num;
	uint16_t h;
	uint16_t w;
	uint32_t size;
	const struct font_text_t * text_val;
	const unsigned char * base_adr;
};

typedef struct lz_text_t {
	char text[30];
	uint16_t color;
	struct font_t font_val;
} lz_text_t;


lz_obj_t * lz_create_lable(int x,int y);
void lz_lable_text_set( lz_obj_t * obj, char * val );

#endif //LZUI_LZ_OBJ_LABLE_H
