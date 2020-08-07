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
typedef struct _lz_leasing_t {
    double b;
    double c;
    double d;
    double ax, bx, cx;
    double ay, by, cy;
    double start, end;
    double (*func)(struct _lz_leasing_t * e, double t);
} lz_leasing_t;

double sine_in_out(lz_leasing_t * e, double t);
double bounce_in(lz_leasing_t * e, double t);

#endif //LZUI_LZ_EASING_H
