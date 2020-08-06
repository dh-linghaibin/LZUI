//
// Created by LHB on 2020/8/4.
//

#ifndef LZUI_LZ_TICK_H
#define LZUI_LZ_TICK_H

#include "lzui/lz_types.h"
#include "lzui/lz_obj.h"

struct _lz_tick_t {
    uint32_t over_time;
    uint32_t repeat;
    lz_obj_t * obj;
    void (*event)(struct _lz_tick_t * tick);
    struct _lz_tick_t * nxet;
};

void lz_tick_inc(uint32_t tick_period);
struct _lz_tick_t * lz_tick_create(lz_obj_t * obj, void (*event)(struct _lz_tick_t * tick), uint32_t time);
void lz_tick_delet(struct _lz_tick_t * tick);
void lz_tick_loop(void);
void lz_tick_time_set(struct _lz_tick_t * tick, uint32_t time);

#endif //LZUI_LZ_TICK_H
