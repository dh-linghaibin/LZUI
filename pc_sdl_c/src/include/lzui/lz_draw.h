//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_DRAW_H
#define LZUI_LZ_DRAW_H

#include "lzui/lz_color.h"
#include "lzui/lz_obj.h"
#include "lzui/lz_config.h"

void lz_draw_set_updata( void (*up)(int x, int y, int width, int length, uint16_t * color) );
void lz_draw_frame(int x, int y, int width, int length, lz_color5658_t color);
void lz_draw_line(int s_x, int s_y, int e_x, int e_y, uint16_t color);
void lz_draw_empty_frame( int x1, int y1, int x2, int y2, uint16_t color );
void lz_draw_arc( int x0, int y0, int r, int s, uint16_t color );
void lz_draw_circle( int x0, int y0, int r, uint16_t c );
void lz_draw_fill_circle( int x0, int y0, int r, uint16_t c );
void lz_draw_round_frame( int x1, int y1, int x2, int y2, int r, uint16_t c );
void lz_draw_fill_round_frame( int x1, int y1, int x2, int y2, int r, lz_color5658_t color );
void lz_draw_mesh( int x1, int y1, int x2, int y2, lz_color5658_t color );
void lz_draw_png(int x, int y, int width, int length, const unsigned char * material);
void lz_draw_jpg(int x, int y, int width, int length, uint8_t * material);
void lz_draw_font(int x, int y, uint8_t wighth, uint8_t length, uint16_t color, uint8_t * mate);
void lz_draw_screen(int x, int y, int width, int length, uint32_t * material);

void lz_drawcache_size_set(int x, int y, int width, int length);
void lz_draw_cache_to_lcd(int x, int y, int width, int length);


#ifndef LV_ANTIALIAS
#define LV_ANTIALIAS        1       /*1: Enable anti-aliasing*/
#endif

#define CIRCLE_AA_NON_LINEAR_OPA_THRESHOLD  5   /*Circle segment greater then this value will be anti-aliased by a non-linear (cos) opacity mapping*/

#define SHADOW_OPA_EXTRA_PRECISION      8       /*Calculate with 2^x bigger shadow opacity values to avoid rounding errors*/
#define SHADOW_BOTTOM_AA_EXTRA_RADIUS   3       /*Add extra radius with LV_SHADOW_BOTTOM to cover anti-aliased corners*/

#define LV_CIRC_OCT1_X(p) (p.x)
#define LV_CIRC_OCT1_Y(p) (p.y)
#define LV_CIRC_OCT2_X(p) (p.y)
#define LV_CIRC_OCT2_Y(p) (p.x)
#define LV_CIRC_OCT3_X(p) (-p.y)
#define LV_CIRC_OCT3_Y(p) (p.x)
#define LV_CIRC_OCT4_X(p) (-p.x)
#define LV_CIRC_OCT4_Y(p) (p.y)
#define LV_CIRC_OCT5_X(p) (-p.x)
#define LV_CIRC_OCT5_Y(p) (-p.y)
#define LV_CIRC_OCT6_X(p) (-p.y)
#define LV_CIRC_OCT6_Y(p) (-p.x)
#define LV_CIRC_OCT7_X(p) (p.y)
#define LV_CIRC_OCT7_Y(p) (-p.x)
#define LV_CIRC_OCT8_X(p) (p.x)
#define LV_CIRC_OCT8_Y(p) (-p.y)

#define LV_MATH_MIN(a,b) ((a) < (b) ? (a) : (b))
#define LV_MATH_MAX(a,b) ((a) > (b) ? (a) : (b))
#define LV_MATH_ABS(x) ((x) > 0 ? (x) : (-(x)))

typedef int16_t lv_coord_t;
typedef uint8_t lv_opa_t;
typedef uint8_t lv_border_part_t;
typedef uint8_t lv_shadow_type_t;

#define LV_OPA_MIN      16      /*Opacities below this will be transparent*/
#define LV_OPA_MAX      251     /*Opacities above this will fully cover*/

#ifndef LV_HOR_RES
#define LV_HOR_RES          (320)
#endif
#ifndef LV_VER_RES
#define LV_VER_RES          (240)
#endif

typedef union
{
    uint8_t blue  :1;
    uint8_t green :1;
    uint8_t red   :1;
    uint8_t full  :1;
} lv_color1_t;

typedef union
{
    struct
    {
        uint8_t blue  :2;
        uint8_t green :3;
        uint8_t red   :3;
    };
    uint8_t full;
} lv_color8_t;

typedef union
{
    struct
    {
#if LV_COLOR_16_SWAP == 0
        uint16_t blue  :5;
        uint16_t green :6;
        uint16_t red   :5;
#else
        uint16_t green_h :3;
        uint16_t red   :5;
        uint16_t blue  :5;
        uint16_t green_l :3;
#endif
    };
    uint16_t full;
} lv_color16_t;

typedef uint16_t lv_color_int_t;
typedef lv_color16_t lv_color_t;

enum {
    LV_OPA_TRANSP =  0,
    LV_OPA_0      =  0,
    LV_OPA_10     =  25,
    LV_OPA_20     =  51,
    LV_OPA_30     =  76,
    LV_OPA_40     =  102,
    LV_OPA_50     =  127,
    LV_OPA_60     =  153,
    LV_OPA_70     =  178,
    LV_OPA_80     =  204,
    LV_OPA_90     =  229,
    LV_OPA_100    =  255,
    LV_OPA_COVER  =  255,
};

/*Border types (Use 'OR'ed values)*/
enum
{
    LV_BORDER_NONE =     0x00,
    LV_BORDER_BOTTOM =   0x01,
    LV_BORDER_TOP =      0x02,
    LV_BORDER_LEFT =     0x04,
    LV_BORDER_RIGHT =    0x08,
    LV_BORDER_FULL =     0x0F,
    LV_BORDER_INTERNAL = 0x10,    /*FOR matrix-like objects (e.g. Button matrix)*/
};
typedef uint8_t lv_border_part_t;

typedef struct
{
    lv_coord_t x;
    lv_coord_t y;
} lv_point_t;

typedef struct
{
    lv_coord_t x1;
    lv_coord_t y1;
    lv_coord_t x2;
    lv_coord_t y2;
} lv_area_t;

typedef struct
{
    uint8_t glass :1;           /*1: Do not inherit this style*/

    struct {
        lv_color_t main_color;
        lv_color_t grad_color;      /*`grad_color` will be removed in v6.0, use `aux_color` instead*/
        lv_coord_t radius;
        lv_opa_t opa;

        struct {
            lv_color_t color;
            lv_coord_t width;
            lv_border_part_t part;
            lv_opa_t opa;
        } border;

        struct {
            lv_color_t color;
            lv_coord_t width;
            lv_shadow_type_t type;
        } shadow;

        struct {
            lv_coord_t ver;
            lv_coord_t hor;
            lv_coord_t inner;
        } padding;

        uint8_t empty :1;   /*Transparent background (border still drawn)*/
    } body;


    struct {
        lv_color_t color;
//        const lv_font_t * font;
        lv_coord_t letter_space;
        lv_coord_t line_space;
        lv_opa_t opa;
    } text;

    struct {
        lv_color_t color;
        lv_opa_t intense;
        lv_opa_t opa;
    } image;

    struct {
        lv_color_t color;
        lv_coord_t width;
        lv_opa_t opa;
        uint8_t rounded :1;     /*1: rounded line endings*/
    } line;
} lv_style_t;

static inline lv_coord_t lv_area_get_width(const lv_area_t * area_p)
{
    return area_p->x2 - area_p->x1 + 1;
}

/**
 * Get the height of an area
 * @param area_p pointer to an area
 * @return the height of the area (if y1 == y2 -> height = 1)
 */
static inline lv_coord_t lv_area_get_height(const lv_area_t * area_p)
{
    return area_p->y2 - area_p->y1 + 1;
}

static inline lv_color_t lv_color_mix(lv_color_t c1, lv_color_t c2, uint8_t mix)
{
    lv_color_t ret;
#if LV_COLOR_DEPTH != 1
    /*LV_COLOR_DEPTH == 8, 16 or 32*/
    ret.red =   (uint16_t)((uint16_t) c1.red * mix + (c2.red * (255 - mix))) >> 8;
#  if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP
    /*If swapped Green is in 2 parts*/
    uint16_t g_1 = (c1.green_h << 3) + c1.green_l;
    uint16_t g_2 = (c2.green_h << 3) + c2.green_l;
    uint16_t g_out = (uint16_t)((uint16_t) g_1 * mix + (g_2 * (255 - mix))) >> 8;
    ret.green_h = g_out >> 3;
    ret.green_l = g_out & 0x7;
#  else
    ret.green = (uint16_t)((uint16_t) c1.green * mix + (c2.green * (255 - mix))) >> 8;
#  endif
    ret.blue =  (uint16_t)((uint16_t) c1.blue * mix + (c2.blue * (255 - mix))) >> 8;
# if LV_COLOR_DEPTH == 32
    ret.alpha = 0xFF;
# endif
#else
    /*LV_COLOR_DEPTH == 1*/
    ret.full = mix > LV_OPA_50 ? c1.full : c2.full;
#endif

    return ret;
}

void lv_draw_rect_main_corner(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale);
void lv_draw_rect_border_corner(const lv_area_t * coords, const lv_area_t * mask, const  lv_style_t * style, lv_opa_t opa_scale);

void lv_draw_rect(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale);

#endif //LZUI_LZ_DRAW_H
