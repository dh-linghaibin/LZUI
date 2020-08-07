//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_OBJ_BUTTON_H
#define LZUI_LZ_OBJ_BUTTON_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef struct _lz_button_t {
	lz_color5658_t color;
	uint8_t state;
	void (*on_click)(lz_obj_t * obj);
	void (*on_click_down)(lz_obj_t * obj);
} lz_button_t;

lz_obj_t * lz_create_button(int x,int y);
void lui_button_set_text(lz_obj_t * obj, char * tex);
void lui_button_set_size(lz_obj_t * obj, int width,int length);
void lui_button_set_color(lz_obj_t * obj, lz_color5658_t color);
void lz_button_text_set_color(lz_obj_t * obj, uint16_t color);
void lui_button_setonclicklistener(lz_obj_t * obj, void (*on_click)(lz_obj_t * obj));
void lui_button_setonclicklistener_down(lz_obj_t * obj, void (*on_click_down)(lz_obj_t * obj));

#endif //LZUI_LZ_OBJ_BUTTON_H
