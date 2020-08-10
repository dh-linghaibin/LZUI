//
// Created by LHB on 2020/8/10.
//

#ifndef LZUI_LZ_OBJ_MOUSE_H
#define LZUI_LZ_OBJ_MOUSE_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef struct lz_mouse_t {
    int x;
    int y;
    uint8_t state;
} lz_mouse_t;

lz_obj_t *lz_create_mouse(int x, int y);
void lz_mouse_x_set(lz_obj_t * obj, uint8_t state);
void lz_mouse_y_set(lz_obj_t * obj, uint8_t state);

#endif //LZUI_LZ_OBJ_MOUSE_H
