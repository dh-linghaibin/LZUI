//
// Created by LHB on 2020/8/4.
//

#ifndef LZUI_LZ_OBJ_CHART_H
#define LZUI_LZ_OBJ_CHART_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"

#define WIDTH		320		// 屏幕宽
#define HEIGHT		240			// 屏幕高
#define NUM_MOVERS	2000			// 小球数量
#define	FRICTION	0.96f		// 摩擦力/阻尼系数

// 定义小球结构
typedef struct _mover_t {
    uint16_t color;			// 颜色
    float x,y;			// 坐标
    float v_x,v_y;			// 速度
    float radius;         // 半径
} mover_t;

typedef struct _pos_t {
    mover_t movers[NUM_MOVERS];// 小球数组
    int mouse_x, mouse_y;			// 当前鼠标坐标
    int prev_mouse_x, prev_mouse_y;		// 上次鼠标坐标
    int mouse_vx, mouse_vy;		// 鼠标速度
    int is_mouse_down;				// 鼠标左键是否按下
} pos_t;

struct _lz_obj_chart_t {
    uint16_t point[20];
};

lz_obj_t * lz_create_chart(int x,int y);

#endif //LZUI_LZ_OBJ_CHART_H
