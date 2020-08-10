//
// Created by LHB on 2020/8/9.
//

#include "lzui/lz_obj_pos.h"
#include "lzui/lz_draw.h"
#include <math.h>
#include <minmax.h>
#include <lzui/lz_tick.h>
#include <time.h>
#include "lzui/lz_color.h"

void pos_init( lz_obj_pos_t * pos );
void pos_calculate( lz_obj_pos_t * pos );
void pos_drawer( lz_obj_pos_t * pos );

static void lui_design (lz_obj_t * obj, lz_point_t *point);
static void tick_event(lz_tick_t * tick);
static void delete_val_cb( lz_obj_t * obj );

lz_obj_t * lz_create_pos(int x,int y) {
    lz_obj_pos_t * pos = lz_malloc(sizeof(lz_obj_pos_t));
    pos_init( pos );
    pos->state = 0;
    lz_obj_t * obj = lz_create_obj(x,y,320,240, pos, lui_design);
    lz_obj_set_delete_val_cb( obj, delete_val_cb );
    pos->tick = lz_tick_create(obj, tick_event, 5);
    return obj;
}

static void delete_val_cb( lz_obj_t * obj ) {
    lz_obj_pos_t * pos = obj->val;
    lz_tick_delet( pos->tick );
}

void lz_post_set_is_mouse_down( lz_obj_t * obj, int is_mouse_down ) {
    lz_obj_pos_t * pos = obj->val;
    pos->is_mouse_down = is_mouse_down;
}

void lz_post_set_mouse_x_y( lz_obj_t * obj, int x, int y ) {
    lz_obj_pos_t * pos = obj->val;
    pos->mouse_x = x;
    pos->mouse_y = y;
}

static void tick_event(lz_tick_t * tick) {
    lz_obj_t * obj = tick->obj;
    lz_obj_pos_t * pos = obj->val;
    pos_calculate(pos);
    pos->state = 1;
}

static void lui_design (lz_obj_t * obj, lz_point_t * point) {
    lz_obj_pos_t * pos = obj->val;
    if(pos->state == 1) {
//        pos->state = 0;
        pos_drawer(pos);
    }
}

void pos_init( lz_obj_pos_t * pos ) {
    // 设置随机种子
    srand((unsigned)time(NULL));//选取种子文件
    // 初始化小球数组
    for (int i = 0; i < NUM_MOVERS; i++) {
        pos->movers[i].color = i * 500;//lz_color_alpha_blend(rand() % 256, rand() % 256, rand() % 256);
        pos->movers[i].x   = rand()%WIDTH;
        pos->movers[i].y   = rand()%HEIGHT;
        pos->movers[i].v_x  = (cos((i))) * (rand() % 34);
        pos->movers[i].v_y  = (sin((i))) * (rand() % 34);
        pos->movers[i].radius  = (rand() % 34)/15.0;
    }
    // 初始化鼠标变量，当前鼠标坐标、上次鼠标坐标都在画布中心
    pos->mouse_x = pos->prev_mouse_x = WIDTH / 2;
    pos->mouse_y = pos->prev_mouse_y = HEIGHT / 2;
    pos->is_mouse_down = 0; // 初始鼠标未按下
}

void pos_calculate( lz_obj_pos_t * pos ) {
    float toDist   = WIDTH * 0.86;  // 吸引距离，小球距离鼠标在此范围内，会受到向内的吸力
    float blowDist = WIDTH * 0.5;   // 打击距离，小球距离鼠标在此范围内，会受到向外的斥力
    float stirDist = WIDTH * 0.125; // 扰动距离，小球距离鼠标在此范围内，会受到鼠标的扰动
    // 前后两次运行间鼠标移动的距离，即为鼠标的速度
    pos->mouse_vx = pos->mouse_x - pos->prev_mouse_x;
    pos->mouse_vy = pos->mouse_y - pos->prev_mouse_y;
    // 更新上次鼠标坐标变量，为记录这次鼠标的坐标
    pos->prev_mouse_x = pos->mouse_x;
    pos->prev_mouse_y = pos->mouse_y;

    for(int i = 0; i < NUM_MOVERS; i++)  // 对所有小球遍历
    {
        float x  = pos->movers[i].x;  // 当前小球坐标
        float y  = pos->movers[i].y;
        float vX = pos->movers[i].v_x;  // 当前小球速度
        float vY = pos->movers[i].v_y;

        float dX = x - pos->mouse_x;    // 计算当前小球位置和鼠标位置的差
        float dY = y - pos->mouse_y;
        float d  = sqrt(dX * dX + dY * dY);    // 当前小球和鼠标位置的距离
        // 下面将dX、dY归一化，仅反映方向，和距离长度无关。
        if (d!=0) {
            dX = dX/d;
            dY = dY/d;
        } else {
            dX = 0;
            dY = 0;
        }
        // 小球距离鼠标 < toDist，在此范围内小球会受到鼠标的吸引
        if (d < toDist) {
            // 吸引力引起的加速度幅度，小球距离鼠标越近，引起的加速度越大。但吸引力的值明显比上面斥力的值要小很多
            float toAcc = (1 - (d / toDist)) * WIDTH * 0.0014f;
            // 由dX、dY归一化方向信息，加速度幅度值toAcc，得到新的小球速度
            vX = vX - dX * toAcc;
            vY = vY - dY * toAcc;
        }

        // 当鼠标左键按下，并且小球距离鼠标 < blowDist（在打击范围内，会受到向外的力）
        if (pos->is_mouse_down && d < blowDist) {
            // 打击力引起的加速度幅度(Acceleration)，这个公式表示小球距离鼠标越近，打击斥力引起的加速度越大
            float blowAcc = (1 - (d / blowDist)) * 10;
            // 由上面得到的dX、dY归一化方向信息，上面的加速度幅度值blowAcc，得到新的小球速度
            //  float(rand()) / RAND_MAX 产生[0,1]之间的随机数
            // 0.5f - float(rand()) / RAND_MAX 产生[-0.5,0.5]之间的随机数，加入一些扰动
            vX = vX + dX * blowAcc + 0.5f - (rand()) / RAND_MAX;
            vY = vY + dY * blowAcc + 0.5f - (rand()) / RAND_MAX;
        }
        // 小球距离鼠标 < stirDist，在此范围内小球会受到鼠标的扰动
        if (d < stirDist) {
            // 扰动力引起的加速度幅度，小球距离鼠标越近，引起的加速度越大。扰动力的值更小
            float mAcc = (1 - (d / stirDist)) * WIDTH * 0.00026f;
            // 鼠标速度越快，引起的扰动力越大
            vX = vX + pos->mouse_vx * mAcc;
            vY = vY + pos->mouse_vy * mAcc;
        }
        // 小球运动有一个阻尼（摩擦力），速度逐渐减少
        vX = vX * FRICTION;
        vY = vY * FRICTION;
        // 速度的绝对值
        float avgVX = fabsf(vX);
        float avgVY = fabsf(vY);
        // 两个方向速度的平均
        float avgV  = (avgVX + avgVY) * 0.5f;
        // 因为有上面阻尼的作用，如果速度过小的话，乘以一个[0,3]的随机数，会以比较大的概率让速度变大
        if (avgVX < 0.1)
            vX = vX * (rand()) / RAND_MAX * 3;
        if (avgVY < 0.1)
            vY = vY * (rand()) / RAND_MAX * 3;
        // 小球的半径在[0.4,3.5]之间，速度越大，半径越大
        float sc = avgV * 0.45f;
        sc = max(min(sc, 3.5f), 0.4f);
        pos->movers[i].radius = sc;
        // 根据位置+速度，更新小球的坐标
        float nextX = x + vX;
        float nextY = y + vY;
        // 小球如果超过上下左右四个边界的话，位置设为边界处，速度反向
        if	(nextX > WIDTH)
        {
            nextX = WIDTH;
            vX = -1*vX;
        } else if (nextX < 0) {
            nextX = 0;
            vX = -1*vX;
        }
        if (nextY > HEIGHT) {
            nextY = HEIGHT;
            vY = -1*vY;
        } else if (nextY < 0) {
            nextY = 0;
            vY = -1*vY;
        }
        // 更新小球位置、速度的结构体数组
        pos->movers[i].v_x = vX;
        pos->movers[i].v_y = vY;
        pos-> movers[i].x  = nextX;
        pos->movers[i].y  = nextY;
    }
}

void pos_drawer( lz_obj_pos_t * pos ) {
    lz_color5658_t color;
    color.alpha = 0;
    for(int i = 0; i < NUM_MOVERS; i++) {
        color.color.rgb565 = pos->movers[i].color;
        lz_draw_frame((pos->movers[i].x + 1), (pos->movers[i].y + 1),
                      (pos->movers[i].radius + 1),(pos->movers[i].radius + 1),
                      color);
    }
}
