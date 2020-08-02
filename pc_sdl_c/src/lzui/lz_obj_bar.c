//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_obj_bar.h"
#include "lzui/lz_draw.h"


static void lui_bar_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_bar_event(lui_touch_val_t *val);

lui_obj_t * lui_create_bar(int x, int y) {
	lui_bar_t * bar = lui_malloc(sizeof(lui_bar_t));
	bar->val = 1;
	bar->b_color.color.rgb565 = lui_color_888_to_565(lui_color_white);
	bar->b_color.alpha = 100;
	bar->t_color.color.rgb565 = lui_color_888_to_565(lui_color_white);
	bar->t_color.alpha = LUI_COLOR_ALPHA_FALL;
	bar->on_event = NULL;
	lui_obj_t * obj = lui_create_obj(x,y,150,10,bar,lui_bar_design);
	lui_obj_set_event(obj,lui_bar_event);
	return obj;
}

void lui_bar_set_val(lui_obj_t * obj, uint8_t val) {
	lui_bar_t * bar = obj->val;
	bar->val = val;
}

void lui_bar_set_event(lui_obj_t * obj, void (*on_event)(lui_obj_t * obj)) {
	lui_bar_t * bar = obj->val;
	bar->on_event = on_event;
}

static void lui_bar_design (struct _lui_obj_t * obj, lui_point_t *point) {
	lui_bar_t * bar = obj->val;
	lui_draw_frame(point->x,
	               point->y,
	               obj->layout.size.width,
	               obj->layout.size.length,
	               bar->b_color);
	lui_draw_frame(point->x,
	               point->y,
	               bar->val*(obj->layout.size.width*0.01),
	               obj->layout.size.length,
	               bar->t_color);
}

static void lui_bar_event(lui_touch_val_t *val) {
	lui_bar_t * bar = val->obj->val;
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


