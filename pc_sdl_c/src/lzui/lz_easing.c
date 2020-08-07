//
// Created by LHB on 2020/8/7.
//

#include "lzui/lz_easing.h"
#include <math.h>
#include "lzui/lz_types.h"

double linear(lz_leasing_t * e, double t)
{
    return e->c * t / e->d + e->b;
}

double sine_in(lz_leasing_t * e, double t)
{
    return -e->c * cos(t / e->d * M_PI_2) + e->c + e->b;
}

double sine_out(lz_leasing_t * e, double t)
{
    return e->c * sin(t / e->d * M_PI_2) + e->b;
}

double sine_in_out(lz_leasing_t * e, double t)
{
    return -e->c / 2 * (cos(M_PI_1 * t / e->d) - 1) + e->b;
}

double quad_in(lz_leasing_t * e, double t)
{
    t = t / e->d;
    return e->c * pow(t, 2) + e->b;
}

double quad_out(lz_leasing_t * e, double t)
{
    t = t / e->d;
    return -e->c * t * (t - 2) + e->b;
}

double quad_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->d * 2;
    if(t < 1)
        r = e->c / 2 * pow(t, 2) + e->b;
    else
        r = -e->c / 2 * ((t - 1) * (t - 3) - 1) + e->b;
    return r;
}

double cubic_in(lz_leasing_t * e, double t)
{
    t = t / e->d;
    return e->c * pow(t, 3) + e->b;
}

double cubic_out(lz_leasing_t * e, double t)
{
    t = t / e->d - 1;
    return e->c * (pow(t, 3) + 1) + e->b;
}

double cubic_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->d * 2;
    if(t < 1)
    {
        r =  e->c / 2 * pow(t, 3) + e->b;
    }
    else
    {
        t = t - 2;
        r = e->c / 2 * (pow(t, 3) + 2) + e->b;
    }
    return r;
}

double quart_in(lz_leasing_t * e, double t)
{
    t = t / e->d;
    return e->c * pow(t, 4) + e->b;
}

double quart_out(lz_leasing_t * e, double t)
{
    t = t / e->d - 1;
    return -e->c * (pow(t, 4) - 1) + e->b;
}

double quart_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->d * 2;
    if(t < 1)
    {
        r =  e->c / 2 * pow(t, 4) + e->b;
    }
    else
    {
        t = t - 2;
        r = -e->c / 2 * (pow(t, 4) - 2) + e->b;
    }
    return r;
}

double quint_in(lz_leasing_t * e, double t)
{
    t = t / e->d;
    return e->c * pow(t, 5) + e->b;
}

double quint_out(lz_leasing_t * e, double t)
{
    t = t / e->d - 1;
    return e->c * (pow(t, 5) + 1) + e->b;
}

double quint_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->d * 2;
    if(t < 1)
    {
        r = e->c / 2 * pow(t, 5) + e->b;
    }
    else
    {
        t = t - 2;
        r = e->c / 2 * (pow(t, 5) + 2) + e->b;
    }
    return r;
}

double expo_in(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
        r = e->b;
    else
        r = e->c * pow(2, 10 * (t / e->d - 1)) + e->b - e->c * 0.001;
    return r;
}

double expo_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == e->d)
        r = e->b + e->c;
    else
        r = e->c * 1.001 * (-pow(2, -10 * t / e->d) + 1) + e->b;
    return r;
}

double expo_in_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->b;
    }
    else if(t == e->d)
    {
        r = e->b + e->c;
    }
    else
    {
        t = t / e->d * 2;
        if(t < 1)
        {
            r = e->c / 2 * pow(2, 10 * (t - 1)) + e->b - e->c * 0.0005;
        }
        else
        {
            t = t - 1;
            r = e->c / 2 * 1.0005 * (-pow(2, -10 * t) + 2) + e->b;
        }
    }
    return r;
}

double circ_in(lz_leasing_t * e, double t)
{
    t = t / e->d;
    return -e->c * (sqrt(1 - pow(t, 2)) - 1) + e->b;
}

double circ_out(lz_leasing_t * e, double t)
{
    t = t / e->d - 1;
    return e->c * sqrt(1 - pow(t, 2)) + e->b;
}

double circ_in_out(lz_leasing_t * e, double t)
{
    double r;
    t = t / e->d * 2;
    if(t < 1)
    {
        r = -e->c / 2 * (sqrt(1 - t * t) - 1) + e->b;
    }
    else
    {
        t = t - 2;
        r = e->c / 2 * (sqrt(1 - t * t) + 1) + e->b;
    }
    return r;
}

double back_in(lz_leasing_t * e, double t)
{
    double s = 1.70158;
    t = t / e->d;
    return e->c * t * t * ((s + 1) * t - s) + e->b;
}

double back_out(lz_leasing_t * e, double t)
{
    double s = 1.70158;
    t = t / e->d - 1;
    return e->c * (t * t * ((s + 1) * t + s) + 1) + e->b;
}

double back_in_out(lz_leasing_t * e, double t)
{
    double s = 1.70158;
    double r;
    s = s * 1.525;
    t = t / e->d * 2;
    if(t < 1)
    {
        r = e->c / 2 * (t * t * ((s + 1) * t - s)) + e->b;
    }
    else
    {
        t = t - 2;
        r = e->c / 2 * (t * t * ((s + 1) * t + s) + 2) + e->b;
    }
    return r;
}

double elastic_in(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->b;
    }
    else
    {
        t = t / e->d;
        if(t == 1)
        {
            r = e->b + e->c;
        }
        else
        {
            double p = e->d * 0.3;
            double s = p / 4;
            double a = e->c;
            t = t - 1;
            r = -(a * pow(2, 10 * t) * sin((t * e->d - s) * (2 * M_PI_1) / p)) + e->b;
        }
    }
    return r;
}

double elastic_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->b;
    }
    else
    {
        t = t / e->d;
        if(t == 1)
        {
            r = e->b + e->c;
        }
        else
        {
            double p = e->d * 0.3;
            double s = p / 4;
            double a = e->c;
            r = a * pow(2, -10 * t) * sin((t * e->d - s) * (2 * M_PI_1) / p) + e->c + e->b;
        }
    }
    return r;
}

double elastic_in_out(lz_leasing_t * e, double t)
{
    double r;
    if(t == 0)
    {
        r = e->b;
    }
    else
    {
        t = t / e->d * 2;
        if(t == 2)
        {
            r = e->b + e->c;
        }
        else
        {
            double p = e->d * (0.3 * 1.5);
            double a = e->c;
            double s = p / 4;
            if(t < 1)
            {
                t = t - 1;
                r = -0.5 * (a * pow(2, 10 * t) * sin((t * e->d - s) * (2 * M_PI_1) / p)) + e->b;
            }
            else
            {
                t = t - 1;
                r = a * pow(2, -10 * t) * sin((t * e->d - s) * (2 * M_PI_1) / p) * 0.5 + e->c	+ e->b;
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

double bounce_in(lz_leasing_t * e, double t)
{
    return __bounce_in(t, e->b, e->c, e->d);
}

double bounce_out(lz_leasing_t * e, double t)
{
    return __bounce_out(t, e->b, e->c, e->d);
}

double bounce_in_out(lz_leasing_t * e, double t)
{
    double r;
    if (t < e->d / 2)
        r = __bounce_in(t * 2, 0, e->c, e->d) * 0.5 + e->b;
    else
        r = __bounce_out(t * 2 - e->d, 0, e->c, e->d) * 0.5 + e->c * 0.5 + e->b;
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

double cubic_bezier(lz_leasing_t * e, double t)
{
    double r;
    if(t < 0.0)
        r = 0.0 + e->start * t;
    else if(t > 1.0)
        r = 1.0 + e->end * (t - 1.0);
    else
        r = sample_curve_y(e, solve_curve_x(e, t));
    return e->c * r / e->d + e->b;
}


