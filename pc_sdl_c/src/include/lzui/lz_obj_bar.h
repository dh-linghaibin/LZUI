//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_OBJ_BAR_H
#define LZUI_LZ_OBJ_BAR_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef struct _lui_bar_t {
	uint8_t val;
	lui_color5658_t b_color;
	lui_color5658_t t_color;
	void (*on_event)(lui_obj_t * obj);
} lui_bar_t;

lui_obj_t * lui_create_bar(int x, int y);
void lui_bar_set_event(lui_obj_t * obj, void (*on_event)(lui_obj_t * obj));

#endif //LZUI_LZ_OBJ_BAR_H
