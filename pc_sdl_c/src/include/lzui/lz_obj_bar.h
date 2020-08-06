//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_OBJ_BAR_H
#define LZUI_LZ_OBJ_BAR_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef struct _lz_bar_t {
	uint8_t val;
	lz_color5658_t b_color;
	lz_color5658_t t_color;
	void (*on_event)(lz_obj_t * obj);
} lz_bar_t;

lz_obj_t * lz_create_bar(int x, int y);
void lz_bar_set_event(lz_obj_t * obj, void (*on_event)(lz_obj_t * obj));

#endif //LZUI_LZ_OBJ_BAR_H
