//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_obj_button.h"
#include "lzui/lz_draw.h"

static void lui_button_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_button_event(lui_touch_val_t *val);

lui_obj_t * lui_create_button(int x,int y) {
	lui_button_t * but = lui_malloc(sizeof(lui_button_t));
	but->color.color.rgb565 = lui_color_888_to_565(0x07DBFF);
	but->color.alpha = 0;
	but->on_click = NULL;
	but->on_click_down = NULL;
	lui_obj_t * obj = lui_create_obj(x,y,50,25,but,lui_button_design);
	lui_obj_set_event(obj,lui_button_event);

	return obj;
}

void lui_button_set_text(lui_obj_t * obj, char * tex) {

}

void lui_button_set_color(lui_obj_t * obj, lui_color5658_t color) {
	lui_button_t * but = obj->val;
	but->color = color;
}

void lui_button_text_set_color(lui_obj_t * obj, uint16_t color) {

}

void lui_button_set_size(lui_obj_t * obj, int width,int length) {
	lui_obj_set_width(obj,width);
	lui_obj_set_length(obj,length);
}

void lui_button_setonclicklistener(lui_obj_t * obj, void (*on_click)(lui_obj_t * obj)) {
	lui_button_t * but = obj->val;
	but->on_click = on_click;
}

void lui_button_setonclicklistener_down(lui_obj_t * obj, void (*on_click_down)(lui_obj_t * obj)) {
	lui_button_t * but = obj->val;
	but->on_click_down = on_click_down;
}

static void lui_button_design (struct _lui_obj_t * obj, lui_point_t *point) {
	lui_button_t * but = obj->val;
	lui_draw_frame(point->x,
	               point->y,
	               obj->layout.size.width,
	               obj->layout.size.length,
	               but->color);
    lui_draw_line(20,20,120,20, 0x00);
}

static void lui_button_event(lui_touch_val_t *val) {
	lui_button_t * but = val->obj->val;
	if(val->falg == 2) {
		but->color.alpha = 200;
		if(but->on_click_down != NULL) {
			but->on_click_down(val->obj);
		}
	}
	if(val->falg == 0) {
		but->color.alpha = 0;
		if(but->on_click != NULL) {
			but->on_click(val->obj);
		}
	}
}
