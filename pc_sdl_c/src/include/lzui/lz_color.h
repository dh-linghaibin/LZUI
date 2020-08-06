//
// Created by a6735 on 2020/8/2.
//

#ifndef LZUI_LZ_COLOR_H
#define LZUI_LZ_COLOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#define RGB565(R, G, B) \
    (((uint16_t) ((R) >> 3)) << 11) + (((uint16_t) ((G) >> 2)) << 5) + ((uint16_t) ((B) >> 3))
#define lz_color_ALPHA_FALL 0xff
#define lz_color_ALPHA_NULL 0x00

typedef union {
	struct {
		uint16_t r : 5;
		uint16_t g : 6;
		uint16_t b : 5;
	} rgb;
	uint16_t rgb565;
} lz_color565_u;

typedef struct _lz_color5658_t {
	lz_color565_u color;
	uint8_t      alpha;
} lz_color5658_t;

uint16_t lz_color_888_to_565(uint32_t rgb);
uint16_t lz_color_rgb_to_565(uint8_t r, uint8_t g, uint8_t b);
uint16_t lz_color_alpha_blend(uint16_t fr_c,uint16_t bk_c,uint8_t alpha);


#define lz_color_white     0xffffff
#define lz_color_ivory     0xfffff0
#define lz_color_lightyellow     0xffffe0
#define lz_color_yellow     0xffff00
#define lz_color_snow     0xfffafa
#define lz_color_floralwhite     0xfffaf0
#define lz_color_lemonchiffon     0xfffacd
#define lz_color_cornsilk     0xfff8dc
#define lz_color_seaShell     0xfff5ee
#define lz_color_lavenderblush     0xfff0f5
#define lz_color_papayawhip     0xffefd5
#define lz_color_blanchedalmond     0xffebcd
#define lz_color_mistyrose     0xffe4e1
#define lz_color_bisque     0xffe4c4
#define lz_color_moccasin     0xffe4b5
#define lz_color_navajowhite     0xffdead
#define lz_color_peachpuff     0xffdab9
#define lz_color_gold     0xffd700
#define lz_color_pink     0xffc0cb
#define lz_color_lightpink     0xffb6c1
#define lz_color_orange     0xffa500
#define lz_color_lightsalmon     0xffa07a
#define lz_color_darkorange     0xff8c00
#define lz_color_coral     0xff7f50
#define lz_color_hotpink     0xff69b4
#define lz_color_tomato     0xff6347
#define lz_color_orangered     0xff4500
#define lz_color_deeppink     0xff1493
#define lz_color_fuchsia     0xff00ff
#define lz_color_magenta     0xff00ff
#define lz_color_red     0xff0000
#define lz_color_oldlace     0xfdf5e6
#define lz_color_lightgoldenrodyellow     0xfafad2
#define lz_color_linen     0xfaf0e6
#define lz_color_antiquewhite     0xfaebd7
#define lz_color_salmon     0xfa8072
#define lz_color_ghostwhite     0xf8f8ff
#define lz_color_mintcream     0xf5fffa
#define lz_color_whitesmoke     0xf5f5f5
#define lz_color_beige     0xf5f5dc
#define lz_color_wheat     0xf5deb3
#define lz_color_sandybrown     0xf4a460
#define lz_color_azure     0xf0ffff
#define lz_color_honeydew     0xf0fff0
#define lz_color_aliceblue     0xf0f8ff
#define lz_color_khaki     0xf0e68c
#define lz_color_lightcoral     0xf08080
#define lz_color_palegoldenrod     0xeee8aa
#define lz_color_violet     0xee82ee
#define lz_color_darksalmon     0xe9967a
#define lz_color_lavender     0xe6e6fa
#define lz_color_lightcyan     0xe0ffff
#define lz_color_burlywood     0xdeb887
#define lz_color_plum     0xdda0dd
#define lz_color_gainsboro     0xdcdcdc
#define lz_color_crimson     0xdc143c
#define lz_color_palevioletred     0xdb7093
#define lz_color_goldenrod     0xdaa520
#define lz_color_orchid     0xda70d6
#define lz_color_thistle     0xd8bfd8
#define lz_color_lightgray     0xd3d3d3
#define lz_color_lightgrey     0xd3d3d3
#define lz_color_tan     0xd2b48c
#define lz_color_chocolate     0xd2691e
#define lz_color_peru     0xcd853f
#define lz_color_indianred     0xcd5c5c
#define lz_color_mediumvioletred     0xc71585
#define lz_color_silver     0xc0c0c0
#define lz_color_darkkhaki     0xbdb76b
#define lz_color_rosybrown     0xbc8f8f
#define lz_color_mediumorchid     0xba55d3
#define lz_color_darkgoldenrod     0xb8860b
#define lz_color_firebrick     0xb22222
#define lz_color_powderblue     0xb0e0e6
#define lz_color_lightsteelblue     0xb0c4de
#define lz_color_paleturquoise     0xafeeee
#define lz_color_greenyellow     0xadff2f
#define lz_color_lightblue     0xadd8e6
#define lz_color_darkgray     0xa9a9a9
#define lz_color_darkgrey     0xa9a9a9
#define lz_color_brown     0xa52a2a
#define lz_color_sienna     0xa0522d
#define lz_color_darkorchid     0x9932cc
#define lz_color_palegreen     0x98fb98
#define lz_color_darkviolet     0x9400d3
#define lz_color_mediumpurple     0x9370db
#define lz_color_lightgreen     0x90ee90
#define lz_color_darkseagreen     0x8fbc8f
#define lz_color_saddlebrown     0x8b4513
#define lz_color_darkmagenta     0x8b008b
#define lz_color_darkred     0x8b0000
#define lz_color_blueviolet     0x8a2be2
#define lz_color_lightskyblue     0x87cefa
#define lz_color_skyblue     0x87ceeb
#define lz_color_grey     0x808080
#define lz_color_olive     0x808000
#define lz_color_purple     0x800080
#define lz_color_maroon     0x800000
#define lz_color_aquamarine     0x7fffd4
#define lz_color_chartreuse     0x7fff00
#define lz_color_lawngreen     0x7cfc00
#define lz_color_mediumslateblue     0x7b68ee
#define lz_color_lightslategray     0x778899
#define lz_color_lightslategrey     0x778899
#define lz_color_slategray     0x708090
#define lz_color_slategrey     0x708090
#define lz_color_olivedrab     0x6b8e23
#define lz_color_slateblue     0x6a5acd
#define lz_color_dimgrey     0x696969
#define lz_color_mediumaquamarine     0x66cdaa
#define lz_color_cornflowerblue     0x6495ed
#define lz_color_cadetblue     0x5f9ea0
#define lz_color_darkolivegreen     0x556b2f
#define lz_color_indigo     0x4b0082
#define lz_color_mediumturquoise     0x48d1cc
#define lz_color_darkslateblue     0x483d8b
#define lz_color_steelblue     0x4682b4
#define lz_color_royalblue     0x4169e1
#define lz_color_turquoise     0x40e0d0
#define lz_color_mediumseagreen     0x3cb371
#define lz_color_limegreen     0x32cd32
#define lz_color_darkslategray     0x2f4f4f
#define lz_color_darkslategrey     0x2f4f4f
#define lz_color_seagreen     0x2e8b57
#define lz_color_forestgreen     0x228b22
#define lz_color_lightseagreen     0x20b2aa
#define lz_color_dodgerblue     0x1e90ff
#define lz_color_midnightblue     0x191970
#define lz_color_aqua     0x00ffff
#define lz_color_cyan     0x00ffff
#define lz_color_springgreen     0x00ff7f
#define lz_color_lime     0x00ff00
#define lz_color_mediumspringgreen     0x00fa9a
#define lz_color_darkturquoise     0x00ced1
#define lz_color_deepskyblue     0x00bfff
#define lz_color_darkcyan     0x008b8b
#define lz_color_teal     0x008080
#define lz_color_green     0x008000
#define lz_color_darkgreen     0x006400
#define lz_color_blue     0x0000ff
#define lz_color_mediumblue     0x0000cd
#define lz_color_darkblue     0x00008b
#define lz_color_navy     0x000080

#endif //LZUI_LZ_COLOR_H
