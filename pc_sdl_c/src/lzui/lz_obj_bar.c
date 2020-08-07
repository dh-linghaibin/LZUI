//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_obj_bar.h"
#include "lzui/lz_draw.h"


static void lz_design (lz_obj_t * obj, lz_point_t *point);
static void lz_event(lz_touch_val_t *val);

lz_obj_t * lz_create_bar(int x, int y) {
	lz_bar_t * bar = lz_malloc(sizeof(lz_bar_t));
	bar->val = 20;
	bar->b_color.color.rgb565 = lz_color_888_to_565(0x07DBFF);
	bar->b_color.alpha = 100;
	bar->t_color.color.rgb565 = lz_color_888_to_565(lz_color_red);
	bar->t_color.alpha = lz_color_ALPHA_FALL;
	bar->on_event = NULL;
	lz_obj_t * obj = lz_create_obj(x,y,150,10,bar,lz_design);
	lz_obj_set_event(obj,lz_event);
	return obj;
}

void lui_bar_set_val(lz_obj_t * obj, uint8_t val) {
	lz_bar_t * bar = obj->val;
	bar->val = val;
}

void lz_bar_set_event(lz_obj_t * obj, void (*on_event)(lz_obj_t * obj)) {
	lz_bar_t * bar = obj->val;
	bar->on_event = on_event;
}

static void lz_design (lz_obj_t * obj, lz_point_t *point) {
	lz_bar_t * bar = obj->val;
	lz_draw_frame(point->x,
	               point->y,
	               obj->layout.size.width,
	               obj->layout.size.length,
	               bar->b_color);
	lz_draw_frame(point->x,
	               point->y,
	               bar->val*(obj->layout.size.width*0.01),
	               obj->layout.size.length,
	               bar->t_color);
}

static void lz_event(lz_touch_val_t *val) {
	lz_bar_t * bar = val->obj->val;
	switch(val->falg) {
		case 0: {

		} break;
		case 1: {
			if(val->abs_x > val->obj->layout.size.width) {
				val->abs_x = val->obj->layout.size.width;
			}
			if(val->abs_x < 0) {
				val->abs_x = 0;
			}
			bar->val = val->abs_x/(val->obj->layout.size.width*0.01);
			if(bar->on_event != NULL) {
				bar->on_event(val->obj);
			}
		} break;
		case 2: {
			if(val->abs_x > val->obj->layout.size.width) {
				val->abs_x = val->obj->layout.size.width;
			}
			if(val->abs_x < 0) {
				val->abs_x = 0;
			}
			bar->val = val->abs_x/(val->obj->layout.size.width*0.01);
			if(bar->on_event != NULL) {
				bar->on_event(val->obj);
			}
		} break;
	}
}


