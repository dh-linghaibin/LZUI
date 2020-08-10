//
// Created by LHB on 2020/8/4.
//

#include "lzui/lz_obj_chart.h"
#include "lzui/lz_draw.h"
#include <math.h>
#include <minmax.h>
#include <lzui/lz_tick.h>
#include <time.h>
#include "lzui/lz_color.h"


static void lui_design (lz_obj_t * obj, lz_point_t *point);

lz_obj_t * lz_create_chart(int x,int y) {
    lz_obj_chart_t * chart = lz_malloc(sizeof(lz_obj_chart_t));
    for(int i =0; i < 20; i++) {
        chart->point[i] = i;
    }
    lz_obj_t * obj = lz_create_obj(x,y,150,80,chart,lui_design);
    return obj;
}

static void lui_design (lz_obj_t * obj, lz_point_t *point) {
    struct _lz_obj_chart_t  * chart = obj->val;
    lz_color5658_t color;
    color.color.rgb565 = 0x00;
    color.alpha = 200;
    lz_draw_frame(point->x,
                   point->y,
                   obj->layout.size.width,
                   obj->layout.size.length,
                   color);
    for(int i =0; i < 4; i++) {
        lz_draw_line(point->x + 2, point->y + 10 + (i * 20), point->x + obj->layout.size.width -2, point->y + 10 + (i * 20), 0x00);
    }

    for(int i =0; i < 7; i++) {
        lz_draw_line(point->x + 10 + i*20, point->y + 2, point->x + 10 + i*20, point->y + obj->layout.size.length - 2, 0x00);
    }
//    for(int i = 1; i < 10; i++) {
//        lz_draw_line(point->x + 14 * (i - 1),point->y + obj->layout.size.length -chart->point[i-1],
//                      point->x + 14 * i, point->y + obj->layout.size.length - chart->point[i],0x0505);
//    }
}
