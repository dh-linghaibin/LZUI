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

#endif //LZUI_LZ_DRAW_H
