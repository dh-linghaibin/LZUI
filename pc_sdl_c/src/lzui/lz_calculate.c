//
// Created by LHB on 2020/8/18.
//

#include "lzui/lz_calculate.h"
#include "math.h"

int lz_calculate_get_center( int start_v, int end_v ){
    return (int)( (start_v - end_v) * 0.5 + start_v);
}

/*
圆心坐标：(x0,y0)
半径：r
角度：a
则圆上任一点为：（x1,y1）
x1   =   x0   +   r   *   cos( a )
 y1   =   y0   +   r   *   sin( a )
 * */
lz_point_t lz_calculate_arc_point( lz_point_t point, uint16_t r, uint16_t rate ) {
    struct lz_point_t val;
    val.x = point.x + r * cos( rate );
    val.y = point.y + r * sin( rate );
    return val;
}