//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_color.h"

uint16_t lui_color_888_to_565(uint32_t rgb) {
	register uint8_t  r, g, b;
	b = ( rgb >> (0+3) ) & 0x1f;
	g = ( rgb >> (8+2) ) & 0x3f;
	r = ( rgb >> (16+3)) & 0x1f;
	return( (r<<11) + (g<<5) + (b<<0) );
}

uint16_t lui_color_rgb_to_565(uint8_t r, uint8_t g, uint8_t b) {
	return (uint16_t)( (( r >> 3 ) << 11 ) | (( g >> 2 ) << 5  ) | ( b  >> 3 ));
}

uint16_t lui_color_alpha_blend(uint16_t fr_c,uint16_t bk_c,uint8_t alpha) {
	register lui_color565_u color, color_fr_c,color_bk_c ;
	color_fr_c.rgb565 = fr_c;
	color_bk_c.rgb565 = bk_c;
	color.rgb.r = ( color_fr_c.rgb.r * alpha + color_bk_c.rgb.r * (0xff-alpha) ) >> 8;
	color.rgb.g = ( color_fr_c.rgb.g * alpha + color_bk_c.rgb.g * (0xff-alpha) ) >> 8;
	color.rgb.b = ( color_fr_c.rgb.b * alpha + color_bk_c.rgb.b * (0xff-alpha) ) >> 8;
	return color.rgb565;
}

