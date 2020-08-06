//
// Created by LHB on 2020/8/4.
//

#include "lzui/lz_tick.h"
#include "lzui/lz_config.h"

static uint32_t sys_time    = 0;
static struct _lz_tick_t tick_head = {
        0,
        0,
        NULL,
        NULL,
        NULL,
};

void lz_tick_inc(uint32_t tick_period) {
    sys_time += tick_period;
}

uint32_t lz_tick_get(void) {
    return sys_time;
}

uint32_t lz_tick_elaps(uint32_t prev_tick) {
    uint32_t act_time = lz_tick_get();
    /*If there is no overflow in sys_time simple subtract*/
    if(act_time >= prev_tick) {
        prev_tick = act_time - prev_tick;
    } else {
        prev_tick  = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }
    return prev_tick;
}

struct _lz_tick_t * lz_tick_create(lz_obj_t * obj, void (*event)(struct _lz_tick_t * tick), uint32_t time) {
    if(obj == NULL) return NULL;
    struct _lz_tick_t * node;
    node = &tick_head;
    while(node->nxet != NULL) {
        node = node->nxet;
    }
    struct _lz_tick_t * new_node = lz_malloc(sizeof(struct _lz_tick_t));
    if(new_node == NULL) {
        return NULL;
    }
    node->nxet      = new_node;

    new_node->nxet  = NULL;
    new_node->obj   = obj;
    new_node->event = event;
    new_node->over_time = time;
    new_node->repeat    = lz_tick_get();
    return new_node;
}

void lz_tick_time_set(struct _lz_tick_t * tick, uint32_t time) {
    if(tick == NULL) return;
    tick->over_time = time;
}

void lz_tick_delet(struct _lz_tick_t * tick) {
    if(tick == NULL) return;
    struct _lz_tick_t * node;
    node = &tick_head;
    while(node != NULL) {
        if(node->nxet == tick) {
            node->nxet = node->nxet->nxet;
            lz_free(tick);
            return;
        }
        node = node->nxet;
    }
}

void lz_tick_loop(void) {
    struct _lz_tick_t * node; /* 遍历时间链表 */
    node = tick_head.nxet;
    while(node != NULL) {
        if(node->event != NULL) {
            if(lz_tick_elaps(node->repeat) >= node->over_time) {
                node->repeat = lz_tick_get();
                node->event(node);
            }
        }
        node = node->nxet;
    }
}


