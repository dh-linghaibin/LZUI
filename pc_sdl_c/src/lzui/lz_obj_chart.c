//
// Created by LHB on 2020/8/4.
//

#include "lzui/lz_obj_chart.h"
#include "lzui/lz_draw.h"
#include <math.h>
#include <minmax.h>
#include <lzui/lz_tick.h>
#include <time.h>
#include <lzui/lz_calculate.h>
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
//    struct _lz_obj_chart_t  * chart = obj->val;
//    lz_color5658_t color;
//    color.color.rgb565 = lz_color_888_to_565(lz_color_red);
//    color.alpha = 0;
//    lz_draw_frame(point->x,
//                   point->y,
//                   obj->layout.size.width,
//                   obj->layout.size.length,
//                   color);
//    for(int i =0; i < 4; i++) {
//        lz_draw_line(point->x + 2, point->y + 10 + (i * 20), point->x + obj->layout.size.width -2, point->y + 10 + (i * 20), 0x00);
//    }
//
//    for(int i =0; i < 7; i++) {
//        lz_draw_line(point->x + 10 + i*20, point->y + 2, point->x + 10 + i*20, point->y + obj->layout.size.length - 2, 0x00);
//    }

//    for(int i = 1; i < 10; i++) {
//        lz_draw_line(point->x + 14 * (i - 1),point->y + obj->layout.size.length -chart->point[i-1],
//                      point->x + 14 * i, point->y + obj->layout.size.length - chart->point[i],0x0505);
//    }
//    lz_draw_frame(pt.x,
//                  pt.y,
//                  10,
//                  10,
//                  color);

    static lv_area_t coords;
    static lv_area_t mask;
    static lv_style_t style;
    coords.x1 = 0;
    coords.x2 = 50;
    coords.y1 = 0;
    coords.y2 = 50;

    mask.x1 = 0;
    mask.x2 = 320;
    mask.y1 = 0;
    mask.y2 = 240;

//    static lv_style_t style_rel;
    lv_style_init();
    lv_style_copy(&style, &lv_style_pretty);

    style.body.radius = 20;
    style.body.border.width = 1;
//    style.body.border.color.full = lz_color_888_to_565(lz_color_red);
//    style.body.grad_color.full = lz_color_888_to_565(lz_color_red);
//    style.body.main_color.full = lz_color_888_to_565(lz_color_red);
//    style.body.border.part = LV_BORDER_FULL;
//    style.body.opa = 255;
//    style.body.border.opa = 76;

//    style.body.border.opa = 50;
//    style.body.opa = 50;
//    style.body.empty = 0;
//    lv_draw_rect(&coords, &mask, &style,50 );
    lv_draw_rect( &coords, &mask, &style,255 );

    style.line.width = 1;
    style.line.color.full = lz_color_888_to_565(lz_color_red);
//    style.line.opa = 255;
    style.line.rounded = 0;
    static lv_point_t point1;
    static lv_point_t point2;
    point1.x = 20;
    point1.y = 20;
    point2.x = 100;
    point2.y = 100;
    lv_draw_line(&point1, &point2, &mask, &style, 255);

}
