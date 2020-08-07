//
// Created by LHB on 2020/8/7.
//

#ifndef LZUI_LZ_EASING_H
#define LZUI_LZ_EASING_H

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
    double (*func)(struct lz_leasing_t * e, double t);
} lz_leasing_t;

double lz_sine_in_out(lz_leasing_t * e, double t);
double lz_bounce_in(lz_leasing_t * e, double t);

#endif //LZUI_LZ_EASING_H
