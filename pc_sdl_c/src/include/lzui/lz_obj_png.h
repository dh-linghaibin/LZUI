//
// Created by LHB on 2020/8/3.
//

#ifndef LZUI_LZ_OBJ_PNG_H
#define LZUI_LZ_OBJ_PNG_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

typedef enum {
    LIT_JPG = 1,
    LIT_PNG = 2,
    LIT_GIF = 3,
} lz_icon_type;

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

typedef struct _lz_icon_t {
    struct png_t png;
    uint8_t mesh;
} lz_icon_t;

lz_obj_t *lz_create_png(int x, int y);
void lz_icon_set_val(lz_obj_t * obj, struct png_t val);

#endif //LZUI_LZ_OBJ_PNG_H
