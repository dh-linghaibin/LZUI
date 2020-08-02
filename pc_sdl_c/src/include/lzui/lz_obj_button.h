//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_OBJ_BUTTON_H
#define LZUI_LZ_OBJ_BUTTON_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef struct _lui_button_t {
	lui_color5658_t color;
	void (*on_click)(lui_obj_t * obj);
	void (*on_click_down)(lui_obj_t * obj);
} lui_button_t;

lui_obj_t * lui_create_button(int x,int y);
void lui_button_set_text(lui_obj_t * obj, char * tex);
void lui_button_set_size(lui_obj_t * obj, int width,int length);
void lui_button_set_color(lui_obj_t * obj, lui_color5658_t color);
void lui_button_text_set_color(lui_obj_t * obj, uint16_t color);
void lui_button_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj));
void lui_button_setonclicklistener_down(lui_obj_t * obj, void (*on_click_down)(lui_obj_t * obj));

#endif //LZUI_LZ_OBJ_BUTTON_H
