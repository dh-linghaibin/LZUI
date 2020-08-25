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
#define LV_ANTIALIAS       1       /*1: Enable anti-aliasing*/
#endif

typedef int16_t lv_coord_t;

typedef uint8_t lv_anim_enable_t;

/** Type of the animated value*/
typedef lv_coord_t lv_anim_value_t;


typedef union
{
    struct
    {
        uint8_t blue : 1;
        uint8_t green : 1;
        uint8_t red : 1;
    } ch;
    uint8_t full;
} lv_color1_t;

typedef union
{
    struct
    {
        uint8_t blue : 2;
        uint8_t green : 3;
        uint8_t red : 3;
    } ch;
    uint8_t full;
} lv_color8_t;

typedef union
{
    struct
    {
#if LV_COLOR_16_SWAP == 0
        uint16_t blue : 5;
        uint16_t green : 6;
        uint16_t red : 5;
#else
        uint16_t green_h : 3;
        uint16_t red : 5;
        uint16_t blue : 5;
        uint16_t green_l : 3;
#endif
    } ch;
    uint16_t full;
} lv_color16_t;

typedef uint8_t lv_opa_t;

typedef struct
{
    uint16_t h;
    uint8_t s;
    uint8_t v;
} lv_color_hsv_t;


typedef uint16_t lv_color_int_t;
typedef lv_color16_t lv_color_t;

/** Represents an area of the screen. */
typedef struct
{
    lv_coord_t x1;
    lv_coord_t y1;
    lv_coord_t x2;
    lv_coord_t y2;
} lv_area_t;

/*Border types (Use 'OR'ed values)*/
enum {
    LV_BORDER_NONE     = 0x00,
    LV_BORDER_BOTTOM   = 0x01,
    LV_BORDER_TOP      = 0x02,
    LV_BORDER_LEFT     = 0x04,
    LV_BORDER_RIGHT    = 0x08,
    LV_BORDER_FULL     = 0x0F,
    LV_BORDER_INTERNAL = 0x10, /**< FOR matrix-like objects (e.g. Button matrix)*/
};
typedef uint8_t lv_border_part_t;

/*Shadow types*/
enum {
    LV_SHADOW_BOTTOM = 0, /**< Only draw bottom shadow */
    LV_SHADOW_FULL,       /**< Draw shadow on all sides */
};
typedef uint8_t lv_shadow_type_t;

/**
 * Objects in LittlevGL can be assigned a style - which holds information about
 * how the object should be drawn.
 *
 * This allows for easy customization without having to modify the object's design
 * function.
 */
typedef struct
{
    uint8_t glass : 1; /**< 1: Do not inherit this style*/

    /** Object background. */
    struct
    {
        lv_color_t main_color; /**< Object's main background color. */
        lv_color_t grad_color; /**< Second color. If not equal to `main_color` a gradient will be drawn for the background. */
        lv_coord_t radius; /**< Object's corner radius. You can use #LV_RADIUS_CIRCLE if you want to draw a circle. */
        lv_opa_t opa; /**< Object's opacity (0-255). */

        struct
        {
            lv_color_t color; /**< Border color */
            lv_coord_t width; /**< Border width */
            lv_border_part_t part; /**< Which borders to draw */
            lv_opa_t opa; /**< Border opacity. */
        } border;


        struct
        {
            lv_color_t color;
            lv_coord_t width;
            lv_shadow_type_t type; /**< Which parts of the shadow to draw */
        } shadow;

        struct
        {
            lv_coord_t top;
            lv_coord_t bottom;
            lv_coord_t left;
            lv_coord_t right;
            lv_coord_t inner;
        } padding;
    } body;

    /** Style for text drawn by this object. */
    struct
    {
        lv_color_t color; /**< Text color */
        lv_color_t sel_color; /**< Text selection background color. */
//        const lv_font_t * font;
        lv_coord_t letter_space; /**< Space between letters */
        lv_coord_t line_space; /**< Space between lines (vertical) */
        lv_opa_t opa; /**< Text opacity */
    } text;

    /**< Style of images. */
    struct
    {
        lv_color_t color; /**< Color to recolor the image with */
        lv_opa_t intense; /**< Opacity of recoloring (0 means no recoloring) */
        lv_opa_t opa; /**< Opacity of whole image */
    } image;

    /**< Style of lines (not borders). */
    struct
    {
        lv_color_t color;
        lv_coord_t width;
        lv_opa_t opa;
        uint8_t rounded : 1; /**< 1: rounded line endings*/
    } line;

#if LV_USE_DEBUG
    #if LV_USE_ASSERT_STYLE
    uint32_t debug_sentinel; /**<Should `LV_STYLE_DEGUG_SENTINEL_VALUE` to indicate that the style is valid*/
#endif
#endif

} lv_style_t;

/**
 * Opacity percentages.
 */
enum {
    LV_OPA_TRANSP = 0,
    LV_OPA_0      = 0,
    LV_OPA_10     = 25,
    LV_OPA_20     = 51,
    LV_OPA_30     = 76,
    LV_OPA_40     = 102,
    LV_OPA_50     = 127,
    LV_OPA_60     = 153,
    LV_OPA_70     = 178,
    LV_OPA_80     = 204,
    LV_OPA_90     = 229,
    LV_OPA_100    = 255,
    LV_OPA_COVER  = 255,
};



/**
 * Structure for holding display buffer information.
 */
typedef struct
{
    void * buf1; /**< First display buffer. */
    void * buf2; /**< Second display buffer. */

    /*Internal, used by the library*/
    void * buf_act;
    uint32_t size; /*In pixel count*/
    lv_area_t area;
    volatile uint32_t flushing : 1;
} lv_disp_buf_t;

/**
 * Display Driver structure to be registered by HAL
 */
typedef struct _disp_drv_t
{

    lv_coord_t hor_res; /**< Horizontal resolution. */
    lv_coord_t ver_res; /**< Vertical resolution. */

    /** Pointer to a buffer initialized with `lv_disp_buf_init()`.
     * LittlevGL will use this buffer(s) to draw the screens contents */
    lv_disp_buf_t * buffer;

#if LV_ANTIALIAS
    uint32_t antialiasing : 1; /**< 1: antialiasing is enabled on this display. */
#endif
    uint32_t rotated : 1; /**< 1: turn the display by 90 degree. @warning Does not update coordinates for you!*/

#if LV_COLOR_SCREEN_TRANSP
    /**Handle if the the screen doesn't have a solid (opa == LV_OPA_COVER) background.
     * Use only if required because it's slower.*/
    uint32_t screen_transp : 1;
#endif

    /** MANDATORY: Write the internal buffer (VDB) to the display. 'lv_disp_flush_ready()' has to be
     * called when finished */
    void (*flush_cb)(struct _disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

    /** OPTIONAL: Extend the invalidated areas to match with the display drivers requirements
     * E.g. round `y` to, 8, 16 ..) on a monochrome display*/
    void (*rounder_cb)(struct _disp_drv_t * disp_drv, lv_area_t * area);

    /** OPTIONAL: Set a pixel in a buffer according to the special requirements of the display
     * Can be used for color format not supported in LittelvGL. E.g. 2 bit -> 4 gray scales
     * @note Much slower then drawing with supported color formats. */
    void (*set_px_cb)(struct _disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
                      lv_color_t color, lv_opa_t opa);

    /** OPTIONAL: Called after every refresh cycle to tell the rendering and flushing time + the
     * number of flushed pixels */
    void (*monitor_cb)(struct _disp_drv_t * disp_drv, uint32_t time, uint32_t px);

#if LV_USE_GPU
    /** OPTIONAL: Blend two memories using opacity (GPU only)*/
    void (*gpu_blend_cb)(struct _disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length,
                         lv_opa_t opa);

    /** OPTIONAL: Fill a memory with a color (GPU only)*/
    void (*gpu_fill_cb)(struct _disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
                        const lv_area_t * fill_area, lv_color_t color);
#endif

    /** On CHROMA_KEYED images this color will be transparent.
     * `LV_COLOR_TRANSP` by default. (lv_conf.h)*/
    lv_color_t color_chroma_key;

#if LV_USE_USER_DATA
    lv_disp_drv_user_data_t user_data; /**< Custom display driver user data */
#endif

} lv_disp_drv_t;


/**
 * Display structure.
 * ::lv_disp_drv_t is the first member of the structure.
 */
typedef struct _disp_t
{
    /**< Driver to the display*/
    lv_disp_drv_t driver;

    /**< A task which periodically checks the dirty areas and refreshes them*/
//    lv_task_t * refr_task;

    /** Screens of the display*/
//    lv_ll_t scr_ll;
//    struct _lv_obj_t * act_scr; /**< Currently active screen on this display */
//    struct _lv_obj_t * top_layer; /**< @see lv_disp_get_layer_top */
//    struct _lv_obj_t * sys_layer; /**< @see lv_disp_get_layer_sys */

    /** Invalidated (marked to redraw) areas*/
//    lv_area_t inv_areas[LV_INV_BUF_SIZE];
//    uint8_t inv_area_joined[LV_INV_BUF_SIZE];
    uint32_t inv_p : 10;

    /*Miscellaneous data*/
    uint32_t last_activity_time; /**< Last time there was activity on this display */
} lv_disp_t;

/**
 * Represents a point on the screen.
 */
typedef struct
{
    lv_coord_t x;
    lv_coord_t y;
} lv_point_t;

/*********************
 *      DEFINES
 *********************/
/*Circle segment greater then this value will be anti-aliased by a non-linear (cos) opacity
 * mapping*/
#define CIRCLE_AA_NON_LINEAR_OPA_THRESHOLD 1

/*Calculate with 2^x bigger shadow opacity values to avoid rounding errors*/
#define SHADOW_OPA_EXTRA_PRECISION 8

/*Add extra radius with LV_SHADOW_BOTTOM to cover anti-aliased corners*/
#define SHADOW_BOTTOM_AA_EXTRA_RADIUS 3


#define LV_OPA_MIN 16  /*Opacities below this will be transparent*/
#define LV_OPA_MAX 251 /*Opacities above this will fully cover*/

#define LV_MATH_MIN(a, b) ((a) < (b) ? (a) : (b))
#define LV_MATH_MAX(a, b) ((a) > (b) ? (a) : (b))
#define LV_MATH_ABS(x) ((x) > 0 ? (x) : (-(x)))

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

#define LV_COLOR_DEPTH     16

# define LV_COLOR_GET_R16(c) (c).ch.red
# define LV_COLOR_GET_G16(c) (c).ch.green
# define LV_COLOR_GET_G16_SWAP(c) (((c).ch.green_h << 3) + (c).ch.green_l)
# define LV_COLOR_GET_B16(c) (c).ch.blue
# define LV_COLOR_GET_A16(c) 0xFF

# define LV_COLOR_GET_R(c) LV_COLOR_GET_R16(c)
# if LV_COLOR_16_SWAP == 0
#   define LV_COLOR_GET_G(c) LV_COLOR_GET_G16(c)
# else
#   define LV_COLOR_GET_G(c) LV_COLOR_GET_G16_SWAP(c)
# endif
# define LV_COLOR_GET_B(c)   LV_COLOR_GET_B16(c)
# define LV_COLOR_GET_A(c)   LV_COLOR_GET_A16(c)

# define LV_COLOR_SET_R16(c, v) (c).ch.red = (uint8_t)(v) & 0x1FU;
# define LV_COLOR_SET_G16(c, v) (c).ch.green = (uint8_t)(v) & 0x3FU;
# define LV_COLOR_SET_G16_SWAP(c, v) {(c).ch.green_h = (uint8_t)(((v) >> 3) & 0x7); (c).ch.green_l = (uint8_t)((v) & 0x7);}
# define LV_COLOR_SET_B16(c, v) (c).ch.blue = (uint8_t)(v) & 0x1FU;
# define LV_COLOR_SET_A16(c, v) do {} while(0)

# define LV_COLOR_SET_R(c, v) LV_COLOR_SET_R16(c,v)
#   define LV_COLOR_SET_G(c, v) LV_COLOR_SET_G16(c,v)
# define LV_COLOR_SET_B(c, v) LV_COLOR_SET_B16(c,v)
# define LV_COLOR_SET_A(c, v) LV_COLOR_SET_A16(c,v)

static inline lv_color_t lv_color_mix(lv_color_t c1, lv_color_t c2, uint8_t mix)
{
    lv_color_t ret;
#if LV_COLOR_DEPTH != 1
    /*LV_COLOR_DEPTH == 8, 16 or 32*/
    LV_COLOR_SET_R(ret, (uint16_t)((uint16_t) LV_COLOR_GET_R(c1) * mix + LV_COLOR_GET_R(c2) * (255 - mix)) >> 8);
    LV_COLOR_SET_G(ret, (uint16_t)((uint16_t) LV_COLOR_GET_G(c1) * mix + LV_COLOR_GET_G(c2) * (255 - mix)) >> 8);
    LV_COLOR_SET_B(ret, (uint16_t)((uint16_t) LV_COLOR_GET_B(c1) * mix + LV_COLOR_GET_B(c2) * (255 - mix)) >> 8);
    LV_COLOR_SET_A(ret, 0xFF);
#else
    /*LV_COLOR_DEPTH == 1*/
    ret.full = mix > LV_OPA_50 ? c1.full : c2.full;
#endif

    return ret;
}


void lv_draw_rect_border_corner(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style,
                                lv_opa_t opa_scale);

void lv_draw_rect(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale);

/**
 * Draw a line
 * @param point1 first point of the line
 * @param point2 second point of the line
 * @param mask the line will be drawn only on this area
 * @param style pointer to a line's style
 * @param opa_scale scale down all opacities by the factor
 */
void lv_draw_line(const lv_point_t * point1, const lv_point_t * point2, const lv_area_t * mask,
                  const lv_style_t * style, lv_opa_t opa_scale);

extern lv_style_t lv_style_scr;
extern lv_style_t lv_style_transp;
extern lv_style_t lv_style_transp_fit;
extern lv_style_t lv_style_transp_tight;
extern lv_style_t lv_style_plain;
extern lv_style_t lv_style_plain_color;
extern lv_style_t lv_style_pretty;
extern lv_style_t lv_style_pretty_color;
extern lv_style_t lv_style_btn_rel;
extern lv_style_t lv_style_btn_pr;
extern lv_style_t lv_style_btn_tgl_rel;
extern lv_style_t lv_style_btn_tgl_pr;
extern lv_style_t lv_style_btn_ina;

#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_SILVER LV_COLOR_MAKE(0xC0, 0xC0, 0xC0)
#define LV_COLOR_GRAY LV_COLOR_MAKE(0x80, 0x80, 0x80)
#define LV_COLOR_BLACK LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_RED LV_COLOR_MAKE(0xFF, 0x00, 0x00)
#define LV_COLOR_MAROON LV_COLOR_MAKE(0x80, 0x00, 0x00)
#define LV_COLOR_YELLOW LV_COLOR_MAKE(0xFF, 0xFF, 0x00)
#define LV_COLOR_OLIVE LV_COLOR_MAKE(0x80, 0x80, 0x00)
#define LV_COLOR_LIME LV_COLOR_MAKE(0x00, 0xFF, 0x00)
#define LV_COLOR_GREEN LV_COLOR_MAKE(0x00, 0x80, 0x00)
#define LV_COLOR_CYAN LV_COLOR_MAKE(0x00, 0xFF, 0xFF)
#define LV_COLOR_AQUA LV_COLOR_CYAN
#define LV_COLOR_TEAL LV_COLOR_MAKE(0x00, 0x80, 0x80)
#define LV_COLOR_BLUE LV_COLOR_MAKE(0x00, 0x00, 0xFF)
#define LV_COLOR_NAVY LV_COLOR_MAKE(0x00, 0x00, 0x80)
#define LV_COLOR_MAGENTA LV_COLOR_MAKE(0xFF, 0x00, 0xFF)
#define LV_COLOR_PURPLE LV_COLOR_MAKE(0x80, 0x00, 0x80)
#define LV_COLOR_ORANGE LV_COLOR_MAKE(0xFF, 0xA5, 0x00)

#define LV_DPI              100     /*[px]*/

#define LV_COLOR_MAKE(r8, g8, b8) ((lv_color_t){{(uint16_t)((b8 >> 3) & 0x1FU), (uint16_t)((g8 >> 2) & 0x3FU), (uint16_t)((r8 >> 3) & 0x1FU)}})

static inline lv_color_t lv_color_make(uint8_t r, uint8_t g, uint8_t b)
{
    return LV_COLOR_MAKE(r, g, b);
}

void lv_style_init(void);
void lv_style_copy(lv_style_t * dest, const lv_style_t * src);

#endif //LZUI_LZ_DRAW_H
