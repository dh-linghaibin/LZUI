//
// Created by LHB on 2020/8/3.
//

#ifndef LZUI_LZ_OBJ_ICON_H
#define LZUI_LZ_OBJ_ICON_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef enum {
    LIT_JPG = 1,
    LIT_PNG = 2,
    LIT_GIF = 3,
} lui_icon_type;

typedef enum {
    LIP_INTERNAL,
    LIP_EXTERNAL,
} lui_icon_path;
  
struct png_t {
    const unsigned char *data;
    uint16_t lenght;
    uint16_t width;
    uint32_t size;
};

typedef struct _lui_icon_t {
    struct png_t png;
} lui_icon_t;

lui_obj_t *lui_create_icon(int x, int y);
void lui_icon_set_val(lui_obj_t * obj, struct png_t val);

#endif //LZUI_LZ_OBJ_ICON_H
