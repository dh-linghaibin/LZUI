//
// Created by LHB on 2020/8/3.
//

#include "lzui/lz_obj_png.h"
#include "lzui/lz_draw.h"

static void lz_design (struct _lz_obj_t * obj, lz_point_t *point);
static void lz_event(lz_touch_val_t *val);

lz_obj_t *lz_create_png(int x, int y) {
    lz_icon_t * icon = lz_malloc(sizeof(lz_icon_t));
    lz_obj_t * obj = lz_create_obj(x,y,0,0,icon,lz_design);
    lz_obj_set_event(obj,lz_event);
    return obj;
}

void lz_icon_set_val(lz_obj_t * obj, struct png_t val) {
    if(obj == NULL) return;
    lz_icon_t * icon = obj->val;
    icon->png = val;
    lz_obj_set_width(obj, icon->png.width );
    lz_obj_set_length(obj, icon->png.lenght );
}

static void lz_design(struct _lz_obj_t * obj, lz_point_t *point) {
    lz_icon_t * png = obj->val;
    lz_draw_png(point->x,point->y,png->png.width,png->png.lenght,png->png.data);

}

static void lz_event(lz_touch_val_t *val) {
//    lz_icon_t * icon = val->obj->val;
//    if(val->falg == 2) {
//        icon->mesh = 1;
//    }
//    if(val->falg == 0) {
//        icon->mesh = 0;
//    }
}

