//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_obj_button.h"
#include "lzui/lz_draw.h"

static void lui_button_design (lz_obj_t * obj, lz_point_t *point);
static void lui_button_event(lz_touch_val_t *val);

lz_obj_t * lz_create_button(int x,int y) {
	lz_button_t * but = lz_malloc(sizeof(lz_button_t));
	but->color.color.rgb565 = lz_color_888_to_565(0x07DBFF);
	but->color.alpha = 0;
	but->state = 0;
	but->on_click = NULL;
	but->on_click_down = NULL;
	lz_obj_t * obj = lz_create_obj(x,y,50,25,but,lui_button_design);
	lz_obj_set_event(obj,lui_button_event);

	return obj;
}

void lui_button_set_text(lz_obj_t * obj, char * tex) {

}

void lui_button_set_color(lz_obj_t * obj, lz_color5658_t color) {
	lz_button_t * but = obj->val;
	but->color = color;
}

void lz_button_text_set_color(lz_obj_t * obj, uint16_t color) {

}

void lui_button_set_size(lz_obj_t * obj, int width,int length) {
	lz_obj_set_width(obj,width);
	lz_obj_set_length(obj,length);
}

void lui_button_setonclicklistener(lz_obj_t * obj, void (*on_click)(lz_obj_t * obj)) {
	lz_button_t * but = obj->val;
	but->on_click = on_click;
}

void lui_button_setonclicklistener_down(lz_obj_t * obj, void (*on_click_down)(lz_obj_t * obj)) {
	lz_button_t * but = obj->val;
	but->on_click_down = on_click_down;
}

static void lui_button_design (lz_obj_t * obj, lz_point_t *point) {
	lz_button_t * but = obj->val;
	if(but->state == 0) {
		lz_draw_frame(point->x,
					  point->y,
					  obj->layout.size.width,
					  obj->layout.size.length,
					  but->color);
	} else {
		lz_draw_frame(point->x+2,
					  point->y+2,
					  obj->layout.size.width-4,
					  obj->layout.size.length-4,
					  but->color);
	}
}

static void lui_button_event(lz_touch_val_t *val) {
	lz_button_t * but = val->obj->val;
	if(val->falg == 2) {
		but->state = 1;
		if(but->on_click_down != NULL) {
			but->on_click_down(val->obj);
		}
	}
	if(val->falg == 0) {
		but->state = 0;
		if(but->on_click != NULL) {
			but->on_click(val->obj);
		}
	}
}
