//
// Created by LHB on 2020/8/7.
//

#ifndef LZUI_LZ_EASING_H
#define LZUI_LZ_EASING_H

#include "lzui/lz_obj.h"
#include "lz_tick.h"

/*
 * t = elapsed time
 * b = begin value
 * c = change value (ending - beginning)
 * d = duration (total time)
 * func = easing function will be invoked in '__call' method
 */
typedef struct lz_leasing_t {
    double start_val;
    double end_val;
    double total_time;
    double ax, bx, cx;
    double ay, by, cy;
    double start, end;
    void (*func_c)( double val );
    double (*func)(struct lz_leasing_t * e, double t);
    lz_tick_t * tick;
} lz_leasing_t;

lz_leasing_t * lz_easing_create( void );
void lz_easing_delete( lz_leasing_t * leasing );

double lz_linear(lz_leasing_t * e, double t);
double lz_bounce_in(lz_leasing_t * e, double t);
double lz_sine_in(lz_leasing_t * e, double t);
double lz_sine_out(lz_leasing_t * e, double t);
double lz_sine_in_out(lz_leasing_t * e, double t);
double lz_quad_in(lz_leasing_t * e, double t);
double lz_quad_out(lz_leasing_t * e, double t);
double lz_quad_in_out(lz_leasing_t * e, double t);
double lz_cubic_in(lz_leasing_t * e, double t);
double lz_cubic_out(lz_leasing_t * e, double t);
double lz_cubic_in_out(lz_leasing_t * e, double t);
double lz_quart_in(lz_leasing_t * e, double t);
double lz_quart_out(lz_leasing_t * e, double t);
double lz_quart_in_out(lz_leasing_t * e, double t);
double lz_quint_in(lz_leasing_t * e, double t);
double lz_quint_out(lz_leasing_t * e, double t);
double lz_quint_in_out(lz_leasing_t * e, double t);
double lz_expo_in(lz_leasing_t * e, double t);
double lz_expo_out(lz_leasing_t * e, double t);
double lz_expo_in_out(lz_leasing_t * e, double t);
double lz_circ_in(lz_leasing_t * e, double t);
double lz_circ_out(lz_leasing_t * e, double t);
double lz_circ_in_out(lz_leasing_t * e, double t);
double lz_back_in(lz_leasing_t * e, double t);
double lz_back_out(lz_leasing_t * e, double t);
double lz_back_in_out(lz_leasing_t * e, double t);
double lz_elastic_in(lz_leasing_t * e, double t);
double lz_elastic_out(lz_leasing_t * e, double t);
double lz_elastic_in_out(lz_leasing_t * e, double t);
double lz_bounce_in(lz_leasing_t * e, double t);
double lz_bounce_out(lz_leasing_t * e, double t);
double lz_bounce_in_out(lz_leasing_t * e, double t);
double lz_cubic_bezier(lz_leasing_t * e, double t);

#endif //LZUI_LZ_EASING_H
