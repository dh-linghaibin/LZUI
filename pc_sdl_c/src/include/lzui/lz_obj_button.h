//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_OBJ_BUTTON_H
#define LZUI_LZ_OBJ_BUTTON_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef struct lz_button_t {
	lz_color5658_t color;
	uint8_t state;
	void (*on_event)(lz_obj_t * obj);
} lz_button_t;

lz_obj_t * lz_create_button(int x,int y);
void lui_button_event_set(lz_obj_t * obj, void (*event)(lz_obj_t * obj));

#endif //LZUI_LZ_OBJ_BUTTON_H
