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
#define LUI_COLOR_ALPHA_FALL 0xff
#define LUI_COLOR_ALPHA_NULL 0x00

typedef union {
	struct {
		uint16_t r : 5;
		uint16_t g : 6;
		uint16_t b : 5;
	} rgb;
	uint16_t rgb565;
} lui_color565_u;

typedef struct _lui_color5658_t {
	lui_color565_u color;
	uint8_t      alpha;
} lui_color5658_t;

uint16_t lui_color_888_to_565(uint32_t rgb);
uint16_t lui_color_rgb_to_565(uint8_t r, uint8_t g, uint8_t b);
uint16_t lui_color_alpha_blend(uint16_t fr_c,uint16_t bk_c,uint8_t alpha);


#define lui_color_white     0xffffff
#define lui_color_ivory     0xfffff0
#define lui_color_lightyellow     0xffffe0
#define lui_color_yellow     0xffff00
#define lui_color_snow     0xfffafa
#define lui_color_floralwhite     0xfffaf0
#define lui_color_lemonchiffon     0xfffacd
#define lui_color_cornsilk     0xfff8dc
#define lui_color_seaShell     0xfff5ee
#define lui_color_lavenderblush     0xfff0f5
#define lui_color_papayawhip     0xffefd5
#define lui_color_blanchedalmond     0xffebcd
#define lui_color_mistyrose     0xffe4e1
#define lui_color_bisque     0xffe4c4
#define lui_color_moccasin     0xffe4b5
#define lui_color_navajowhite     0xffdead
#define lui_color_peachpuff     0xffdab9
#define lui_color_gold     0xffd700
#define lui_color_pink     0xffc0cb
#define lui_color_lightpink     0xffb6c1
#define lui_color_orange     0xffa500
#define lui_color_lightsalmon     0xffa07a
#define lui_color_darkorange     0xff8c00
#define lui_color_coral     0xff7f50
#define lui_color_hotpink     0xff69b4
#define lui_color_tomato     0xff6347
#define lui_color_orangered     0xff4500
#define lui_color_deeppink     0xff1493
#define lui_color_fuchsia     0xff00ff
#define lui_color_magenta     0xff00ff
#define lui_color_red     0xff0000
#define lui_color_oldlace     0xfdf5e6
#define lui_color_lightgoldenrodyellow     0xfafad2
#define lui_color_linen     0xfaf0e6
#define lui_color_antiquewhite     0xfaebd7
#define lui_color_salmon     0xfa8072
#define lui_color_ghostwhite     0xf8f8ff
#define lui_color_mintcream     0xf5fffa
#define lui_color_whitesmoke     0xf5f5f5
#define lui_color_beige     0xf5f5dc
#define lui_color_wheat     0xf5deb3
#define lui_color_sandybrown     0xf4a460
#define lui_color_azure     0xf0ffff
#define lui_color_honeydew     0xf0fff0
#define lui_color_aliceblue     0xf0f8ff
#define lui_color_khaki     0xf0e68c
#define lui_color_lightcoral     0xf08080
#define lui_color_palegoldenrod     0xeee8aa
#define lui_color_violet     0xee82ee
#define lui_color_darksalmon     0xe9967a
#define lui_color_lavender     0xe6e6fa
#define lui_color_lightcyan     0xe0ffff
#define lui_color_burlywood     0xdeb887
#define lui_color_plum     0xdda0dd
#define lui_color_gainsboro     0xdcdcdc
#define lui_color_crimson     0xdc143c
#define lui_color_palevioletred     0xdb7093
#define lui_color_goldenrod     0xdaa520
#define lui_color_orchid     0xda70d6
#define lui_color_thistle     0xd8bfd8
#define lui_color_lightgray     0xd3d3d3
#define lui_color_lightgrey     0xd3d3d3
#define lui_color_tan     0xd2b48c
#define lui_color_chocolate     0xd2691e
#define lui_color_peru     0xcd853f
#define lui_color_indianred     0xcd5c5c
#define lui_color_mediumvioletred     0xc71585
#define lui_color_silver     0xc0c0c0
#define lui_color_darkkhaki     0xbdb76b
#define lui_color_rosybrown     0xbc8f8f
#define lui_color_mediumorchid     0xba55d3
#define lui_color_darkgoldenrod     0xb8860b
#define lui_color_firebrick     0xb22222
#define lui_color_powderblue     0xb0e0e6
#define lui_color_lightsteelblue     0xb0c4de
#define lui_color_paleturquoise     0xafeeee
#define lui_color_greenyellow     0xadff2f
#define lui_color_lightblue     0xadd8e6
#define lui_color_darkgray     0xa9a9a9
#define lui_color_darkgrey     0xa9a9a9
#define lui_color_brown     0xa52a2a
#define lui_color_sienna     0xa0522d
#define lui_color_darkorchid     0x9932cc
#define lui_color_palegreen     0x98fb98
#define lui_color_darkviolet     0x9400d3
#define lui_color_mediumpurple     0x9370db
#define lui_color_lightgreen     0x90ee90
#define lui_color_darkseagreen     0x8fbc8f
#define lui_color_saddlebrown     0x8b4513
#define lui_color_darkmagenta     0x8b008b
#define lui_color_darkred     0x8b0000
#define lui_color_blueviolet     0x8a2be2
#define lui_color_lightskyblue     0x87cefa
#define lui_color_skyblue     0x87ceeb
#define lui_color_grey     0x808080
#define lui_color_olive     0x808000
#define lui_color_purple     0x800080
#define lui_color_maroon     0x800000
#define lui_color_aquamarine     0x7fffd4
#define lui_color_chartreuse     0x7fff00
#define lui_color_lawngreen     0x7cfc00
#define lui_color_mediumslateblue     0x7b68ee
#define lui_color_lightslategray     0x778899
#define lui_color_lightslategrey     0x778899
#define lui_color_slategray     0x708090
#define lui_color_slategrey     0x708090
#define lui_color_olivedrab     0x6b8e23
#define lui_color_slateblue     0x6a5acd
#define lui_color_dimgrey     0x696969
#define lui_color_mediumaquamarine     0x66cdaa
#define lui_color_cornflowerblue     0x6495ed
#define lui_color_cadetblue     0x5f9ea0
#define lui_color_darkolivegreen     0x556b2f
#define lui_color_indigo     0x4b0082
#define lui_color_mediumturquoise     0x48d1cc
#define lui_color_darkslateblue     0x483d8b
#define lui_color_steelblue     0x4682b4
#define lui_color_royalblue     0x4169e1
#define lui_color_turquoise     0x40e0d0
#define lui_color_mediumseagreen     0x3cb371
#define lui_color_limegreen     0x32cd32
#define lui_color_darkslategray     0x2f4f4f
#define lui_color_darkslategrey     0x2f4f4f
#define lui_color_seagreen     0x2e8b57
#define lui_color_forestgreen     0x228b22
#define lui_color_lightseagreen     0x20b2aa
#define lui_color_dodgerblue     0x1e90ff
#define lui_color_midnightblue     0x191970
#define lui_color_aqua     0x00ffff
#define lui_color_cyan     0x00ffff
#define lui_color_springgreen     0x00ff7f
#define lui_color_lime     0x00ff00
#define lui_color_mediumspringgreen     0x00fa9a
#define lui_color_darkturquoise     0x00ced1
#define lui_color_deepskyblue     0x00bfff
#define lui_color_darkcyan     0x008b8b
#define lui_color_teal     0x008080
#define lui_color_green     0x008000
#define lui_color_darkgreen     0x006400
#define lui_color_blue     0x0000ff
#define lui_color_mediumblue     0x0000cd
#define lui_color_darkblue     0x00008b
#define lui_color_navy     0x000080

#endif //LZUI_LZ_COLOR_H
