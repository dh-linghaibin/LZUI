//
// Created by LHB on 2020/8/18.
//

#ifndef LZUI_LZ_CALCULATE_H
#define LZUI_LZ_CALCULATE_H

#include "lzui/lz_obj.h"

int lz_calculate_get_center( int start_v, int end_v );
lz_point_t lz_calculate_arc_point( lz_point_t point, uint16_t r, uint16_t rate );

#endif //LZUI_LZ_CALCULATE_H
