//
// Created by LHB on 2020/8/7.
//

#include "lzui/lz_easing.h"
#include <math.h>
#include <lzui/lz_config.h>
#include "lzui/lz_types.h"

static void tick_event(lz_tick_t * tick);

//leasing_v.start_val = 50;
//leasing_v.end_val = 200;
//leasing_v.total_time = 100;
//leasing_v.func = lz_bounce_in;

lz_leasing_t * lz_easing_create( void ) {
    lz_leasing_t * leasing = lz_malloc(sizeof(lz_leasing_t));
    leasing->tick = lz_tick_create(leasing, tick_event, 5);
    return leasing;
}

static void tick_event(lz_tick_t * tick) {
    lz_leasing_t * leasing = tick->obj;
    if( ( leasing->func != NULL ) && ( leasing->func_c != NULL ) ) {
        static uint16_t xx = 0;
        leasing->func_c( leasing->func( leasing, xx ) );
        xx++;
    }
}

void lz_easing_delete( lz_leasing_t * leasing ) {
    lz_free( leasing );
}

double lz_linear(lz_leasing_t * e, double t)
{
    return e->end_val * t / e->total_time + e->start_val;
}

double lz_sine_in(lz_leasing_t * e, double t)
{
    return -e->end_val * cos(t / e->total_time * M_PI_2) + e->end_val + e->start_val;
}

double lz_sine_out(lz_leasing_t * e, double t)
{
    return e->end_val * sin(t / e->total_time * M_PI_2) + e->start_val;
}

double lz_sine_in_out(lz_leasing_t * e, double t)
{
    return -e->end_val / 2 * (cos(M_PI_1 * t / e->total_time) - 1) + e->start_val;
}

double lz_quad_in(lz_leasing_t * e, double t)
{
    t = t / e->total_time;
    return e->end_val * pow(t, 2) + e->start_val;
}

double lz_quad_out(lz_leasing_t * e, double t)
{
    t = t / e->total_time;
    return -e->end_val * t * (t - 2) + e->start_val;
}

double lz_quad_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->total_time * 2;
    if(t < 1)
        r = e->end_val / 2 * pow(t, 2) + e->start_val;
    else
        r = -e->end_val / 2 * ((t - 1) * (t - 3) - 1) + e->start_val;
    return r;
}

double lz_cubic_in(lz_leasing_t * e, double t)
{
    t = t / e->total_time;
    return e->end_val * pow(t, 3) + e->start_val;
}

double lz_cubic_out(lz_leasing_t * e, double t)
{
    t = t / e->total_time - 1;
    return e->end_val * (pow(t, 3) + 1) + e->start_val;
}

double lz_cubic_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->total_time * 2;
    if(t < 1)
    {
        r =  e->end_val / 2 * pow(t, 3) + e->start_val;
    }
    else
    {
        t = t - 2;
        r = e->end_val / 2 * (pow(t, 3) + 2) + e->start_val;
    }
    return r;
}

double lz_quart_in(lz_leasing_t * e, double t)
{
    t = t / e->total_time;
    return e->end_val * pow(t, 4) + e->start_val;
}

double lz_quart_out(lz_leasing_t * e, double t)
{
    t = t / e->total_time - 1;
    return -e->end_val * (pow(t, 4) - 1) + e->start_val;
}

double lz_quart_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->total_time * 2;
    if(t < 1)
    {
        r =  e->end_val / 2 * pow(t, 4) + e->start_val;
    }
    else
    {
        t = t - 2;
        r = -e->end_val / 2 * (pow(t, 4) - 2) + e->start_val;
    }
    return r;
}

double lz_quint_in(lz_leasing_t * e, double t)
{
    t = t / e->total_time;
    return e->end_val * pow(t, 5) + e->start_val;
}

double lz_quint_out(lz_leasing_t * e, double t)
{
    t = t / e->total_time - 1;
    return e->end_val * (pow(t, 5) + 1) + e->start_val;
}

double lz_quint_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->total_time * 2;
    if(t < 1)
    {
        r = e->end_val / 2 * pow(t, 5) + e->start_val;
    }
    else
    {
        t = t - 2;
        r = e->end_val / 2 * (pow(t, 5) + 2) + e->start_val;
    }
    return r;
}

double lz_expo_in(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
        r = e->start_val;
    else
        r = e->end_val * pow(2, 10 * (t / e->total_time - 1)) + e->start_val - e->end_val * 0.001;
    return r;
}

double lz_expo_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == e->total_time)
        r = e->start_val + e->end_val;
    else
        r = e->end_val * 1.001 * (-pow(2, -10 * t / e->total_time) + 1) + e->start_val;
    return r;
}

double lz_expo_in_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->start_val;
    }
    else if(t == e->total_time)
    {
        r = e->start_val + e->end_val;
    }
    else
    {
        t = t / e->total_time * 2;
        if(t < 1)
        {
            r = e->end_val / 2 * pow(2, 10 * (t - 1)) + e->start_val - e->end_val * 0.0005;
        }
        else
        {
            t = t - 1;
            r = e->end_val / 2 * 1.0005 * (-pow(2, -10 * t) + 2) + e->start_val;
        }
    }
    return r;
}

double lz_circ_in(lz_leasing_t * e, double t)
{
    t = t / e->total_time;
    return -e->end_val * (sqrt(1 - pow(t, 2)) - 1) + e->start_val;
}

double lz_circ_out(lz_leasing_t * e, double t)
{
    t = t / e->total_time - 1;
    return e->end_val * sqrt(1 - pow(t, 2)) + e->start_val;
}

double lz_circ_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->total_time * 2;
    if(t < 1)
    {
        r = -e->end_val / 2 * (sqrt(1 - t * t) - 1) + e->start_val;
    }
    else
    {
        t = t - 2;
        r = e->end_val / 2 * (sqrt(1 - t * t) + 1) + e->start_val;
    }
    return r;
}

double lz_back_in(lz_leasing_t * e, double t)
{
    double s = 1.70158;
    t = t / e->total_time;
    return e->end_val * t * t * ((s + 1) * t - s) + e->start_val;
}

double lz_back_out(lz_leasing_t * e, double t)
{
    double s = 1.70158;
    t = t / e->total_time - 1;
    return e->end_val * (t * t * ((s + 1) * t + s) + 1) + e->start_val;
}

double lz_back_in_out(lz_leasing_t * e, double t)
{
    double s = 1.70158;
    double r;
    s = s * 1.525;
    t = t / e->total_time * 2;
    if(t < 1)
    {
        r = e->end_val / 2 * (t * t * ((s + 1) * t - s)) + e->start_val;
    }
    else
    {
        t = t - 2;
        r = e->end_val / 2 * (t * t * ((s + 1) * t + s) + 2) + e->start_val;
    }
    return r;
}

double lz_elastic_in(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->start_val;
    }
    else
    {
        t = t / e->total_time;
        if(t == 1)
        {
            r = e->start_val + e->end_val;
        }
        else
        {
            double p = e->total_time * 0.3;
            double s = p / 4;
            double a = e->end_val;
            t = t - 1;
            r = -(a * pow(2, 10 * t) * sin((t * e->total_time - s) * (2 * M_PI_1) / p)) + e->start_val;
        }
    }
    return r;
}

double lz_elastic_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->start_val;
    }
    else
    {
        t = t / e->total_time;
        if(t == 1)
        {
            r = e->start_val + e->end_val;
        }
        else
        {
            double p = e->total_time * 0.3;
            double s = p / 4;
            double a = e->end_val;
            r = a * pow(2, -10 * t) * sin((t * e->total_time - s) * (2 * M_PI_1) / p) + e->end_val + e->start_val;
        }
    }
    return r;
}

double lz_elastic_in_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->start_val;
    }
    else
    {
        t = t / e->total_time * 2;
        if(t == 2)
        {
            r = e->start_val + e->end_val;
        }
        else
        {
            double p = e->total_time * (0.3 * 1.5);
            double a = e->end_val;
            double s = p / 4;
            if(t < 1)
            {
                t = t - 1;
                r = -0.5 * (a * pow(2, 10 * t) * sin((t * e->total_time - s) * (2 * M_PI_1) / p)) + e->start_val;
            }
            else
            {
                t = t - 1;
                r = a * pow(2, -10 * t) * sin((t * e->total_time - s) * (2 * M_PI_1) / p) * 0.5 + e->end_val	+ e->start_val;
            }
        }
    }
    return r;
}

static inline double __bounce_out(double t, double b, double c, double d)
{
    double r;
    t = t / d;
    if(t < 1 / 2.75)
    {
        r = c * (7.5625 * t * t) + b;
    }
    else if(t < 2 / 2.75)
    {
        t = t - (1.5 / 2.75);
        r = c * (7.5625 * t * t + 0.75) + b;
    }
    else if(t < 2.5 / 2.75)
    {
        t = t - (2.25 / 2.75);
        r = c * (7.5625 * t * t + 0.9375) + b;
    }
    else
    {
        t = t - (2.625 / 2.75);
        r = c * (7.5625 * t * t + 0.984375) + b;
    }
    return r;
}

static inline double __bounce_in(double t, double b, double c, double d)
{
    return c - __bounce_out(d - t, 0, c, d) + b;
}

double lz_bounce_in(lz_leasing_t * e, double t)
{
    return __bounce_in(t, e->start_val, e->end_val, e->total_time);
}

double lz_bounce_out(lz_leasing_t * e, double t)
{
    return __bounce_out(t, e->start_val, e->end_val, e->total_time);
}

double lz_bounce_in_out(lz_leasing_t * e, double t)
{
    double r;
    if (t < e->total_time / 2)
        r = __bounce_in(t * 2, 0, e->end_val, e->total_time) * 0.5 + e->start_val;
    else
        r = __bounce_out(t * 2 - e->total_time, 0, e->end_val, e->total_time) * 0.5 + e->end_val * 0.5 + e->start_val;
    return r;
}

static inline double sample_curve_x(lz_leasing_t * e, double t)
{
    return ((e->ax * t + e->bx) * t + e->cx) * t;
}

static inline double sample_curve_y(lz_leasing_t * e, double t)
{
    return ((e->ay * t + e->by) * t + e->cy) * t;
}

static inline double sample_curve_derivative_x(lz_leasing_t * e, double t)
{
    return (3.0 * e->ax * t + 2.0 * e->bx) * t + e->cx;
}

static inline double solve_curve_x(lz_leasing_t * e, double t)
{
    double t0, t1, t2;
    double x2, d2;
    int i;
    for(t2 = t, i = 0; i < 8; i++)
    {
        x2 = sample_curve_x(e, t2) - t;
        if(fabs(x2) < 1e-7)
            return t2;
        d2 = sample_curve_derivative_x(e, t2);
        if(fabs(d2) < 1e-6)
            break;
        t2 = t2 - x2 / d2;
    }
    t0 = 0.0;
    t1 = 1.0;
    t2 = t;
    while(t0 < t1)
    {
        x2 = sample_curve_x(e, t2);
        if(fabs(x2 - t) < 1e-7)
            return t2;
        if(t > x2)
            t0 = t2;
        else
            t1 = t2;
        t2 = (t1 - t0) * 0.5 + t0;
    }
    return t2;
}

double lz_cubic_bezier(lz_leasing_t * e, double t)
{
    double r;
    if(t < 0.0)
        r = 0.0 + e->start * t;
    else if(t > 1.0)
        r = 1.0 + e->end * (t - 1.0);
    else
        r = sample_curve_y(e, solve_curve_x(e, t));
    return e->end_val * r / e->total_time + e->start_val;
}


