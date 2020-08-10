//
// Created by LHB on 2020/8/9.
//

#ifndef LZUI_LZ_OBJ_POS_H
#define LZUI_LZ_OBJ_POS_H

#include "lzui/lz_obj.h"
#include "lzui/lz_color.h"
#include "lz_tick.h"

#define WIDTH		320		// 屏幕宽
#define HEIGHT		240			// 屏幕高
#define NUM_MOVERS	200			// 小球数量
#define	FRICTION	0.96f		// 摩擦力/阻尼系数

// 定义小球结构
typedef struct mover_t {
    uint16_t color;			// 颜色
    float x,y;			// 坐标
    float v_x,v_y;			// 速度
    float radius;         // 半径
} mover_t;

typedef struct lz_obj_pos_t {
    mover_t movers[NUM_MOVERS];// 小球数组
    int mouse_x, mouse_y;			// 当前鼠标坐标
    int prev_mouse_x, prev_mouse_y;		// 上次鼠标坐标
    int mouse_vx, mouse_vy;		// 鼠标速度
    int is_mouse_down;				// 鼠标左键是否按下
    uint8_t state;
    lz_tick_t * tick;
} lz_obj_pos_t;

lz_obj_t * lz_create_pos(int x,int y);
void lz_post_set_is_mouse_down( lz_obj_t * obj, int is_mouse_down );
void lz_post_set_mouse_x_y( lz_obj_t * obj, int x, int y );

#endif //LZUI_LZ_OBJ_POS_H
