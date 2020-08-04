//
// Created by LHB on 2020/8/3.
//

#include "lzui/lz_obj_icon.h"
#include "lzui/lz_draw.h"

static void lui_icon_design (struct _lui_obj_t * obj, lui_point_t *point);
static void lui_icon_event(lui_touch_val_t *val);

lui_obj_t *lui_create_icon(int x, int y) {
    lui_icon_t * icon = lui_malloc(sizeof(lui_icon_t));
    lui_obj_t * obj = lui_create_obj(x,y,0,0,icon,lui_icon_design);
    lui_obj_set_event(obj,lui_icon_event);
    return obj;
}

void lui_icon_set_val(lui_obj_t * obj, struct png_t val) {
    if(obj == NULL) return;
    lui_icon_t * icon = obj->val;
    icon->png = val;
    lui_obj_set_width(obj, icon->png.width );
    lui_obj_set_length(obj, icon->png.lenght );
}

static void lui_icon_design (struct _lui_obj_t * obj, lui_point_t *point) {
    lui_icon_t * icon = obj->val;
    lui_draw_png(point->x,point->y,icon->png.width,icon->png.lenght,icon->png.data);
}

static void lui_icon_event(lui_touch_val_t *val) {
//    lui_icon_t * icon = val->obj->val;
//    if(val->falg == 2) {
//        icon->mesh = 1;
//    }
//    if(val->falg == 0) {
//        icon->mesh = 0;
//    }
}

