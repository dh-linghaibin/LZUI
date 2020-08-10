//
// Created by LHB on 2020/8/10.
//

#include "lzui/lz_obj_mouse.h"
#include "lzui/lz_draw.h"
#include "lzui/lz_obj_png.h"

extern const struct png_t usb_in_png_v;

static void lz_design (lz_obj_t * obj, lz_point_t *point);

lz_obj_t *lz_create_mouse(int x, int y) {
    lz_mouse_t * mouse = lz_malloc(sizeof(lz_mouse_t));
    lz_obj_t * obj = lz_create_obj(x,y,0,0,mouse,lz_design);
    lz_obj_set_event(obj,NULL);
    return obj;
}

void lz_mouse_x_set(lz_obj_t * obj, uint8_t state) {
    lz_mouse_t * mouse = obj->val;
    if( state == 0 ) {
        mouse->x--;
    } else {
        mouse->x++;
    }
}

void lz_mouse_y_set(lz_obj_t * obj, uint8_t state) {
    lz_mouse_t * mouse = obj->val;
    if( state == 0 ) {
        mouse->y--;
    } else {
        mouse->y++;
    }
}

static void lz_design(lz_obj_t * obj, lz_point_t *point) {
    lz_mouse_t * mouse = obj->val;
    lz_draw_png(mouse->x, mouse->y, usb_in_png_v.width, usb_in_png_v.lenght, usb_in_png_v.data);
}

