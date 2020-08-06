//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_color.h"

uint16_t lz_color_888_to_565(uint32_t rgb) {
	register uint8_t  r, g, b;
	b = ( rgb >> (0+3) ) & 0x1f;
	g = ( rgb >> (8+2) ) & 0x3f;
	r = ( rgb >> (16+3)) & 0x1f;
	return( (r<<11) + (g<<5) + (b<<0) );
}

uint16_t lz_color_rgb_to_565(uint8_t r, uint8_t g, uint8_t b) {
	return (uint16_t)( (( r >> 3 ) << 11 ) | (( g >> 2 ) << 5  ) | ( b  >> 3 ));
}

uint16_t lz_color_alpha_blend(uint16_t fr_c,uint16_t bk_c,uint8_t alpha) {
	register lz_color565_u color, color_fr_c,color_bk_c ;
    register uint8_t al;
    al = 0xff-alpha;
	color_fr_c.rgb565 = fr_c;
	color_bk_c.rgb565 = bk_c;
	color.rgb.r = ( ( (color_fr_c.rgb.r * alpha) + (color_bk_c.rgb.r * (al) ) ) >> 8 );
	color.rgb.g = ( ( (color_fr_c.rgb.g * alpha) + (color_bk_c.rgb.g * (al) ) ) >> 8 );
	color.rgb.b = ( ( (color_fr_c.rgb.b * alpha) + (color_bk_c.rgb.b * (al) ) ) >> 8 );
	return color.rgb565;
}


#define RGB565_RED      0xf800
#define RGB565_GREEN    0x07e0
#define RGB565_BLUE     0x001f
void display_write_color_565(uint16_t volor_565) {
    // 获取RGB单色，并填充低位
//    uint8_t color_r = (volor_565 & RGB565_RED)  >> 8;
//    uint8_t color_g = (volor_565 & RGB565_GREEN)  >> 3;
//    uint8_t color_b = (volor_565 & RGB565_BLUE)  << 3;
}


