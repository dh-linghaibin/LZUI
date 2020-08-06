//
// Created by LHB on 2020/8/4.
//

#ifndef LZUI_LZ_OBJ_CHART_H
#define LZUI_LZ_OBJ_CHART_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

struct _lz_obj_chart_t {
    uint16_t point[20];
};

lz_obj_t * lz_create_chart(int x,int y);

#endif //LZUI_LZ_OBJ_CHART_H
