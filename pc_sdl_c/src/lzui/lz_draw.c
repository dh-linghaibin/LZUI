//
// Created by a6735 on 2020/8/2.
//

#include <stdbool.h>
#include "lzui/lz_draw.h"


typedef struct _lcache {
	uint16_t array[CACHE_SIZE];
	lz_layout_t coordinate;
} lui_cache;

static void (*updata)(int x, int y, int width, int length, uint16_t * color) = NULL;
static lui_cache cache;
lz_layout_t f_layout;

static void l_point(int s_x, int s_y ,int m_x, int m_y, uint16_t color);
static void lui_draw_point_clor5658(int s_x, int s_y ,int m_x, int m_y, lz_color5658_t color);

void lz_draw_set_updata( void (*up)(int x, int y, int width, int length, uint16_t * color) ) {
	updata = up;
}

/* 函数功能: 判断点(x, y)是否在有ploy_sides个顶点的多边形内 */
/* 参数: poly_sides	测试多边形的顶点数
**		 poly_x	测试多边形的各个顶点的X轴坐标
**		 poly_y	测试多边形的各个顶点的Y轴坐标
**		 x	测试点的X轴坐标
**		 Y 	测试点的Y轴坐标 */
/* 返回值: 返回0 表示不在多边形内部，返回1 表示在多边形内部 */
/* 说明: 在多边形各边上的点默认不在多边形内部 */
int in_or_not(int poly_sides, int * poly_x, int * poly_y, int x, int y) {
    int i, j;
    j = poly_sides - 1;
    int res = 0;
    for (i = 0; i < poly_sides; i++) {
        if (( (poly_y[i] < y && poly_y[j] >= y) || (poly_y[j] < y && poly_y[i] >= y) ) &&
            (poly_x[i] <= x || poly_x[j] <= x)) {
            res ^= ((poly_x[i] + (y - poly_y[i]) / (poly_y[j] - poly_y[i]) * (poly_x[j] - poly_x[i])) < x);
        }
        j = i;
    }
    return res;
}

void lz_draw_frame(int x, int y, int width, int length, lz_color5658_t color) {
	if(color.alpha < lz_color_ALPHA_FALL) {
		int maxX1 = x + width;
		int maxY1 = y + length;
		int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
		int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

		int l_x = f_layout.point.x + f_layout.size.width;
		int l_y = f_layout.point.y + f_layout.size.length;

		if (!(maxX1 < f_layout.point.x || x > l_x
		      || maxY1 < f_layout.point.y || y > l_y )) {
			if (!(maxX1 < cache.coordinate.point.x || x > maxX2
			      || maxY1 < cache.coordinate.point.y || y > maxY2)) {
				int x1 = 0; int y1 = 0;
				int x2 = cache.coordinate.size.width;
				int y2 = cache.coordinate.size.length;
				if(f_layout.point.x > x) {
					x = f_layout.point.x;
				}
				if(f_layout.point.y > y) {
					y = f_layout.point.y;
				}
				if(maxX1 > l_x) {
					maxX1 = l_x;
				}
				if(maxY1 > l_y) {
					maxY1 = l_y;
				}
				if(x >= cache.coordinate.point.x) {
					x1 = x-cache.coordinate.point.x;
				}
				if(maxX1 < maxX2) {
					x2 = cache.coordinate.size.width-(maxX2-maxX1);
				}
				if(y >= cache.coordinate.point.y) {
					y1 = y-cache.coordinate.point.y;
				}
				if(maxY1 < maxY2) {
					y2 = cache.coordinate.size.length-(maxY2-maxY1);
				}
				uint16_t y_max = y2*cache.coordinate.size.width;
				for(int y_i = y1*cache.coordinate.size.width; y_i < y_max; y_i += cache.coordinate.size.width) {
					for(int x_j = x1; x_j < x2; x_j++) {
						if(color.alpha == lz_color_ALPHA_NULL) {
							cache.array[y_i+x_j] = color.color.rgb565;
						} else {
							cache.array[y_i+x_j] = lz_color_alpha_blend(cache.array[y_i+x_j],color.color.rgb565,color.alpha);
						}
					}
				}
			}
		}
	}
}

void lz_draw_line(int s_x, int s_y, int e_x, int e_y, uint16_t color) {
	int maxx2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxy2 = cache.coordinate.point.y + cache.coordinate.size.length;
	if (!((s_x + e_x) < cache.coordinate.point.x || s_x > maxx2 || (s_y + e_y) < cache.coordinate.point.y || s_y > maxy2)) {
		int dx,dy,dx2,dy2,x_inc,y_inc,error,index;
		dx = e_x-s_x;
		dy = e_y-s_y;
		if (dx>=0) {
			x_inc = 1;
		} else {
			x_inc = -1;
			dx = -dx;
		}
		if (dy>=0) {
			y_inc = 1;
		} else {
			y_inc = -1;
			dy    = -dy;
		}
		dx2 = dx << 1;
		dy2 = dy << 1;
		if (dx > dy) {
			error = dy2 - dx;
			for (index=0; index <= dx; index++) {
				l_point(s_x,s_y,maxx2,maxy2,color);
				if (error >= 0) {
					error-=dx2;
					s_y+=y_inc;
				}
				error+=dy2;
				s_x+=x_inc;
			}
		} else {
			error = dx2 - dy;
			for (index=0; index <= dy; index++) {
				l_point(s_x,s_y,maxx2,maxy2,color);
				if (error >= 0) {
					error-=dy2;
					s_x+=x_inc;
				}
				error+=dx2;
				s_y+=y_inc;
			}
		}
	}
}

void lz_draw_empty_frame( int x1, int y1, int x2, int y2, uint16_t color ) {
	lz_draw_line(x1,y1,x2,y1,color);
	lz_draw_line(x1,y2,x2,y2,color);
	lz_draw_line(x1,y1,x1,y2,color);
	lz_draw_line(x2,y1,x2,y2,color);
}

void lz_draw_arc( int x0, int y0, int r, int s, uint16_t color ) {
	int maxx2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxy2 = cache.coordinate.point.y + cache.coordinate.size.length;
	int x,y,xd,yd,e;
	if ( x0<0 ) return;
	if ( y0<0 ) return;
	if ( r<=0 ) return;
	xd = 1 - (r << 1); yd = 0; e = 0; x = r; y = 0;
	while ( x >= y ) {
		// Q1
		if ( s & 0x01 ) {
			l_point(x0 + x,y0 - y,maxx2,maxy2,color);
		}
		if ( s & 0x02 ) {
			l_point(x0 + y,y0 - x,maxx2,maxy2,color);
		}
		// Q2
		if ( s & 0x04 ) {
			l_point(x0 - y,y0 - x,maxx2,maxy2,color);
		}
		if ( s & 0x08 ) {
			l_point(x0 - x,y0 - y,maxx2,maxy2,color);
		}
		// Q3
		if ( s & 0x10 ) {
			l_point(x0 - x,y0 + y,maxx2,maxy2,color);
		}
		if ( s & 0x20 ) {
			l_point(x0 - y,y0 + x,maxx2,maxy2,color);
		}

		// Q4
		if ( s & 0x40 ) {
			l_point(x0 + y,y0 + x,maxx2,maxy2,color);
		}
		if ( s & 0x80 ) {
			l_point(x0 + x,y0 + y,maxx2,maxy2,color);
		}
		y++;
		e += yd;
		yd += 2;
		if ( ((e << 1) + xd) > 0 ) {
			x--;
			e += xd;
			xd += 2;
		}
	}
}

void lz_draw_circle( int x0, int y0, int r, uint16_t c ) {
	int maxx = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxy = cache.coordinate.point.y + cache.coordinate.size.length;
	int x,y,xd,yd,e;
	if ( x0<0 ) return;
	if ( y0<0 ) return;
	if ( r<=0 ) return;
	xd = 1 - (r << 1); yd = 0; e = 0; x = r; y = 0;
	while ( x >= y ) {
		l_point(x0 - x, y0 + y, maxx, maxy, c);
		l_point(x0 - x, y0 - y, maxx, maxy, c);
		l_point(x0 + x, y0 + y, maxx, maxy, c);
		l_point(x0 + x, y0 - y, maxx, maxy, c);
		l_point(x0 - y, y0 + x, maxx, maxy, c);
		l_point(x0 - y, y0 - x, maxx, maxy, c);
		l_point(x0 + y, y0 + x, maxx, maxy, c);
		l_point(x0 + y, y0 - x, maxx, maxy, c);
		y++;
		e += yd;
		yd += 2;
		if ( ((e << 1) + xd) > 0 ) {
			x--;
			e += xd;
			xd += 2;
		}
	}
}

void lz_draw_fill_circle( int x0, int y0, int r, uint16_t c ) {
	int x,y,xd;
	if ( x0<0 ) return;
	if ( y0<0 ) return;
	if ( r<=0 ) return;
	xd = 3 - (r << 1);
	x = 0;
	y = r;
	while ( x <= y ) {
		if( y > 0 ) {
			lz_draw_line(x0 - x, y0 - y,x0 - x, y0 + y, c);
			lz_draw_line(x0 + x, y0 - y,x0 + x, y0 + y, c);
		}
		if( x > 0 ) {
			lz_draw_line(x0 - y, y0 - x,x0 - y, y0 + x, c);
			lz_draw_line(x0 + y, y0 - x,x0 + y, y0 + x, c);
		}
		if ( xd < 0 ) {
			xd += (x << 2) + 6;
		} else {
			xd += ((x - y) << 2) + 10;
			y--;
		}
		x++;
	}
	lz_draw_circle(x0, y0, r,c);
}

void lz_draw_round_frame( int x1, int y1, int x2, int y2, int r, uint16_t c ) {
	int n;
	if ( x2 < x1 ) {
		n = x2;
		x2 = x1;
		x1 = n;
	}
	if ( y2 < y1 ) {
		n = y2;
		y2 = y1;
		y1 = n;
	}
	if ( r > x2 ) return;
	if ( r > y2 ) return;
	lz_draw_line(x1+r, y1, x2-r, y1, c);
	lz_draw_line(x1+r, y2, x2-r, y2, c);
	lz_draw_line(x1, y1+r, x1, y2-r, c);
	lz_draw_line(x2, y1+r, x2, y2-r, c);
	lz_draw_arc(x1+r, y1+r, r, 0x0C, c);
	lz_draw_arc(x2-r, y1+r, r, 0x03, c);
	lz_draw_arc(x1+r, y2-r, r, 0x30, c);
	lz_draw_arc(x2-r, y2-r, r, 0xC0, c);
}

void lz_draw_fill_round_frame( int x1, int y1, int x2, int y2, int r, lz_color5658_t color ) {
	int  x,y,xd;
	x2 += x1;
	y2 += y1;
	if ( x2 < x1 ) {
		x = x2;
		x2 = x1;
		x1 = x;
	}
	if ( y2 < y1 ) {
		y = y2;
		y2 = y1;
		y1 = y;
	}
	if ( r<=0 ) return;
	xd = 3 - (r << 1); x = 0; y = r;
	lz_draw_frame(x1 + r, y1, x2 - r - x1 - r, (y2-y1), color);
	while ( x <= y ) {
		if( y > 0 ) {
			lz_draw_line(x2 + x - r, y1 - y + r, x2+ x - r, y + y2 - r, color.color.rgb565);
			lz_draw_line(x1 - x + r, y1 - y + r, x1- x + r, y + y2 - r, color.color.rgb565);
		}
		if( x > 0 ) {
			lz_draw_line(x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r, color.color.rgb565);
			lz_draw_line(x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r, color.color.rgb565);
		}
		if ( xd < 0 ) {
			xd += (x << 2) + 6;
		} else {
			xd += ((x - y) << 2) + 10;
			y--;
		}
		x++;
	}
}

void lz_draw_mesh( int x1, int y1, int x2, int y2, lz_color5658_t color ) {
	int maxx2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxy2 = cache.coordinate.point.y + cache.coordinate.size.length;
	if (!((x1 + x2) < cache.coordinate.point.x || x1 > maxx2 || (y1 + y2) < cache.coordinate.point.y || y1 > maxy2)) {
		int n,m;
		if ( x2 < x1 ) {
			n = x2;
			x2 = x1;
			x1 = n;
		}
		if ( y2 < y1 ) {
			n = y2;
			y2 = y1;
			y1 = n;
		}
		for( m=y1; m<=y2; m+=2 )
		{
			for( n=x1; n<=x2; n+=2 )
			{
				lui_draw_point_clor5658(n,m,maxx2,maxy2,color);
			}
		}
	}
}

void lz_draw_font(int x, int y, uint8_t wighth, uint8_t length, uint16_t color, uint8_t * mate) {
	uint8_t font_w = wighth;
	uint8_t font_l = length;

	int maxX1 = x + font_w;
	int maxY1 = y + font_l;
	int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

	int l_x = f_layout.point.x + f_layout.size.width;
	int l_y = f_layout.point.y + f_layout.size.length;

	if (!(maxX1 < f_layout.point.x || x > l_x
	      || maxY1 < f_layout.point.y || y > l_y )) {
		if (!(maxX1 < cache.coordinate.point.x || x > maxX2 || maxY1 < cache.coordinate.point.y || y > maxY2)) {
			int x1 = 0; int y1 = 0; int x2 = cache.coordinate.size.width; int y2 = cache.coordinate.size.length;

			uint16_t deviation_y = 0;
			uint32_t ptr = 0;
			if(f_layout.point.x > x) {
				ptr += (f_layout.point.x-x);
				deviation_y = ptr;
				x = f_layout.point.x;
			}

			if(f_layout.point.y > y) {
				ptr += (f_layout.point.y-y)*font_w;
				y = f_layout.point.y;
			}

			if(maxY1 > l_y) {
				maxY1 = l_y;
			}

			if(x >= cache.coordinate.point.x) {
				x1 = x-cache.coordinate.point.x;
			}
			if(maxX1 < maxX2) {
				x2 = cache.coordinate.size.width-(maxX2-maxX1);
			}
			if(y >= cache.coordinate.point.y) {
				y1 = y-cache.coordinate.point.y;
			}
			if(maxY1 < maxY2) {
				y2 = cache.coordinate.size.length-(maxY2-maxY1);
			}
			if(y < cache.coordinate.point.y) {
				ptr += (cache.coordinate.point.y-y)*font_w;
			}
			if(x < cache.coordinate.point.x) {
				ptr += (cache.coordinate.point.x-x);
			}
			for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
				for(int x_j = x1; x_j < x2; x_j++) {
					if(mate[ptr] != 0) {
					    uint16_t color_rel = 0;
						if(mate[ptr] == 0xff) {
//							cache.array[y_i+x_j] = color;
                            color_rel = color;
						} else {
//							cache.array[y_i+x_j] = lz_color_alpha_blend(color,cache.array[y_i+x_j],mate[(ptr)]);
                            color_rel = lz_color_alpha_blend(color,cache.array[y_i+x_j],mate[(ptr)]);
						}
                        cache.array[y_i+x_j] = lz_color_alpha_blend(color_rel,cache.array[y_i+x_j],255);
					}
					ptr++;
				}
				ptr += deviation_y;
				if((x+font_w) > maxX2) {
					ptr += ( (x+font_w) - maxX2);
				}
				if(x < cache.coordinate.point.x) {
					ptr += (cache.coordinate.point.x-x);
				}
			}
		}
	}
}

void lz_draw_jpg(int x, int y, int width, int length, uint8_t * material) {
	int maxX1 = x + width;
	int maxY1 = y + length;
	int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

	int l_x = f_layout.point.x + f_layout.size.width;
	int l_y = f_layout.point.y + f_layout.size.length;

	if (!(maxX1 < f_layout.point.x || x > l_x
	      || maxY1 < f_layout.point.y || y > l_y )) {
		if (!(maxX1 < cache.coordinate.point.x || x > maxX2 || maxY1 < cache.coordinate.point.y || y > maxY2)) {
			int x1 = 0; int y1 = 0; int x2 = cache.coordinate.size.width; int y2 = cache.coordinate.size.length;

			uint16_t deviation_y = 0;
			uint32_t ptr = 0;
			if(f_layout.point.x > x) {
				ptr += (f_layout.point.x-x);
				deviation_y = ptr;
				x = f_layout.point.x;
			}

			if(f_layout.point.y > y) {
				ptr += (f_layout.point.y-y)*width;
				y = f_layout.point.y;
			}
			if(maxY1 > l_y) {
				maxY1 = l_y;
			}

			if(x >= cache.coordinate.point.x) {
				x1 = x-cache.coordinate.point.x;
			}
			if(maxX1 < maxX2) {
				x2 = cache.coordinate.size.width-(maxX2-maxX1);
			}
			if(y >= cache.coordinate.point.y) {
				y1 = y-cache.coordinate.point.y;
			}
			if(maxY1 < maxY2) {
				y2 = cache.coordinate.size.length-(maxY2-maxY1);
			}


			if(y < cache.coordinate.point.y) {
				ptr += (cache.coordinate.point.y-y)*width;
			}
			if(x < cache.coordinate.point.x) {
				ptr += (cache.coordinate.point.x-x);
			}
			ptr *= 2;
			for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
				for(int x_j = x1; x_j < x2; x_j++) {
					cache.array[y_i+x_j] = (uint16_t)( material[ptr+1]<<8)+material[ptr];
					ptr += 2;
				}
				ptr += deviation_y*2;
				if((x+width) > maxX2) {
					ptr += ( (x+width) - maxX2) * 2;
				}
				if(x < cache.coordinate.point.x) {
					ptr += (cache.coordinate.point.x-x) * 2;
				}
			}
		}
	}
}

void lz_draw_png(int x, int y, int width, int length, const unsigned char * material) {
	int maxX1 = x + width;
	int maxY1 = y + length;
	int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

	int l_x = f_layout.point.x + f_layout.size.width;
	int l_y = f_layout.point.y + f_layout.size.length;

	if (!(maxX1 < f_layout.point.x || x > l_x
	      || maxY1 < f_layout.point.y || y > l_y )) {
		if (!(maxX1 < cache.coordinate.point.x || x > maxX2 || maxY1 < cache.coordinate.point.y || y > maxY2)) {
			int x1 = 0; int y1 = 0; int x2 = cache.coordinate.size.width; int y2 = cache.coordinate.size.length;

			uint16_t deviation_y = 0;
			uint16_t ptr = 0;
			if(f_layout.point.x > x) {
				ptr += (f_layout.point.x-x);
				deviation_y = ptr;
				x = f_layout.point.x;
			}

			if(f_layout.point.y > y) {
				ptr += (f_layout.point.y-y)*width;
				y = f_layout.point.y;
			}

			if(maxY1 > l_y) {
				maxY1 = l_y;
			}

			if(x >= cache.coordinate.point.x) {
				x1 = x-cache.coordinate.point.x;
			}
			if(maxX1 < maxX2) {
				x2 = cache.coordinate.size.width-(maxX2-maxX1);
			}
			if(y >= cache.coordinate.point.y) {
				y1 = y-cache.coordinate.point.y;
			}
			if(maxY1 < maxY2) {
				y2 = cache.coordinate.size.length-(maxY2-maxY1);
			}
			if(y < cache.coordinate.point.y) {
				ptr += (cache.coordinate.point.y-y)*width;
			}
			if(x < cache.coordinate.point.x) {
				ptr += (cache.coordinate.point.x-x);
			}
			ptr *= 3;
			for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
				for(int x_j = x1; x_j < x2; x_j++) {
					if(material[ptr+2] != 0) {
						uint16_t color = (uint16_t)( material[ptr+1]<<8)+material[ptr];
						cache.array[y_i+x_j] = lz_color_alpha_blend(cache.array[y_i+x_j],color, 0xff-material[ptr+2]);
					}
					ptr += 3;
				}
				ptr += deviation_y*3;
				if((x+width) > maxX2) {
					ptr += ( (x+width) - maxX2) * 3;
				}
				if(x < cache.coordinate.point.x) {
					ptr += (cache.coordinate.point.x-x) * 3;
				}
			}
		}
	}
}

void lz_draw_screen(int x, int y, int width, int length, uint32_t * material) {
	int maxX1 = x + width;
	int maxY1 = y + length;
	int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

	int l_x = f_layout.point.x + f_layout.size.width;
	int l_y = f_layout.point.y + f_layout.size.length;

	if (!(maxX1 < f_layout.point.x || x > l_x
	      || maxY1 < f_layout.point.y || y > l_y )) {
		if (!(maxX1 < cache.coordinate.point.x || x > maxX2 || maxY1 < cache.coordinate.point.y || y > maxY2)) {
			int x1 = 0; int y1 = 0; int x2 = cache.coordinate.size.width; int y2 = cache.coordinate.size.length;

			uint16_t deviation_y = 0;
			uint32_t ptr = 0;
			if(f_layout.point.x > x) {
				ptr += (f_layout.point.x-x);
				deviation_y = ptr;
				x = f_layout.point.x;
			}

			if(f_layout.point.y > y) {
				ptr += (f_layout.point.y-y)*width;
				y = f_layout.point.y;
			}
			if(maxY1 > l_y) {
				maxY1 = l_y;
			}

			if(x >= cache.coordinate.point.x) {
				x1 = x-cache.coordinate.point.x;
			}
			if(maxX1 < maxX2) {
				x2 = cache.coordinate.size.width-(maxX2-maxX1);
			}
			if(y >= cache.coordinate.point.y) {
				y1 = y-cache.coordinate.point.y;
			}
			if(maxY1 < maxY2) {
				y2 = cache.coordinate.size.length-(maxY2-maxY1);
			}
			if(y < cache.coordinate.point.y) {
				ptr += (cache.coordinate.point.y-y)*width;
			}
			if(x < cache.coordinate.point.x) {
				ptr += (cache.coordinate.point.x-x);
			}
			for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
				for(int x_j = x1; x_j < x2; x_j++) {
					cache.array[y_i+x_j] = (uint16_t)( material[ptr] );
					ptr++;
				}
				ptr += deviation_y;
				if((x+width) > maxX2) {
					ptr += ( (x+width) - maxX2);
				}
				if(x < cache.coordinate.point.x) {
					ptr += (cache.coordinate.point.x-x);
				}
			}
		}
	}
}

static void l_point(int s_x, int s_y ,int m_x, int m_y, uint16_t color) {
	if( ( s_x  >= cache.coordinate.point.x && s_x  < m_x ) &&
	    (  s_y >= cache.coordinate.point.y && s_y < m_y ) ) {
		int xx = (s_x-cache.coordinate.point.x);
		int yy = (s_y-cache.coordinate.point.y);
		cache.array[yy*cache.coordinate.size.width+xx] = color;
	}
}

static void lui_draw_point_clor5658(int s_x, int s_y ,int m_x, int m_y, lz_color5658_t color) {
	if(color.alpha < lz_color_ALPHA_FALL) {
		if( ( s_x  >= cache.coordinate.point.x && s_x  < m_x ) &&
		    (  s_y >= cache.coordinate.point.y && s_y < m_y ) ) {
			uint32_t pos = (s_y-cache.coordinate.point.y)*cache.coordinate.size.width+(s_x-cache.coordinate.point.x);
			if(color.alpha == lz_color_ALPHA_NULL) {
				cache.array[pos] = color.color.rgb565;
			} else {
				cache.array[pos] = lz_color_alpha_blend(cache.array[pos],color.color.rgb565,color.alpha);
			}
		}
	}
}

uint8_t lui_draw_check_layout(int x, int y, int width, int length) {
	int maxX1 = x + width;
	int maxY1 = y + length;
	int maxX2 = cache.coordinate.point.x + cache.coordinate.size.width;
	int maxY2 = cache.coordinate.point.y + cache.coordinate.size.length;

	int l_x = f_layout.point.x + f_layout.size.width;
	int l_y = f_layout.point.y + f_layout.size.length;

	if (!(maxX1 < f_layout.point.x || x > l_x
	      || maxY1 < f_layout.point.y || y > l_y )) {

		if (!(maxX1 < cache.coordinate.point.x || x > maxX2
		      || maxY1 < cache.coordinate.point.y || y > maxY2)) {
//			int cx1 = 0; int cy1 = 0;
//			int cx2 = cache.coordinate.size.width;
//			int cy2 = cache.coordinate.size.length;

			if(f_layout.point.x > x) {
				x = f_layout.point.x;
			}

			if(f_layout.point.y > y) {
				y = f_layout.point.y;
			}

			if(maxX1 > l_x) {
				maxX1 = l_x;
			}

			if(maxY1 > l_y) {
				maxY1 = l_y;
			}
			return 1;
		}
	}
	return 0;
}

void lz_drawcache_size_set(int x, int y, int width, int length) {
	for(int i = 0; i < CACHE_SIZE; i++) {
		cache.array[i] = 0xffff;
	}
	cache.coordinate.point.x = x;
	cache.coordinate.point.y = y;
	cache.coordinate.size.width = width;
	cache.coordinate.size.length = length;
}

lz_layout_t * lui_draw_cache_size(void) {
	return &cache.coordinate;
}

void lz_draw_cache_to_lcd(int x, int y, int width, int length) {
	if(updata != NULL) {
		updata(x,y,width,length,cache.array);
	}
}










static uint16_t lv_draw_cont_radius_corr(uint16_t r, lv_coord_t w, lv_coord_t h)
{
    if(r >= (w >> 1)) {
        r = (w >> 1);
        if(r != 0) r--;
    }
    if(r >= (h >> 1)) {
        r = (h >> 1);
        if(r != 0) r--;
    }

    if(r > 0) r -= LV_ANTIALIAS;

    return r;
}

void lv_circ_init(lv_point_t * c, lv_coord_t * tmp, lv_coord_t radius)
{
    c->x = radius;
    c->y = 0;
    *tmp = 1 - radius;
}

/**
 * Approximate the opacity for anti-aliasing.
 * Used  the first segment of a circle which is the longest and have the most non-linearity (cos)
 * @param seg length of the line segment
 * @param px_id index of pixel on the line segment
 * @param line_opa opacity of the lien (it will be the max opacity)
 * @return the desired opacity of the pixel
 */
static lv_opa_t antialias_get_opa_circ(lv_coord_t seg, lv_coord_t px_id, lv_opa_t opa)
{
    static const  lv_opa_t opa_map[8] = {250, 242, 221, 196, 163, 122, 74, 18};
    if(seg == 0) return LV_OPA_TRANSP;
    else if(seg == 1) return LV_OPA_80;
    else {
        uint8_t id = (uint32_t)((uint32_t)px_id * (sizeof(opa_map) - 1)) / (seg - 1);
        return (uint32_t)((uint32_t) opa_map[id] * opa) >> 8;
    }
}

/**
 * Get the opacity of a pixel based it's position in a line segment
 * @param seg segment length
 * @param px_id position of  of a pixel which opacity should be get [0..seg-1]
 * @param base_opa the base opacity
 * @return the opacity of the given pixel
 */
lv_opa_t lv_draw_aa_get_opa(lv_coord_t seg, lv_coord_t px_id, lv_opa_t base_opa)
{
    /* How to calculate the opacity of pixels on the edges which makes the anti-aliasing?
     * For example we have a line like this (y = -0.5 * x):
     *
     *  | _ _
     *    * * |
     *
     * Anti-aliased pixels come to the '*' characters
     * Calculate what percentage of the pixels should be covered if real line (not rasterized) would be drawn:
     * 1. A real line should start on (0;0) and end on (2;1)
     * 2. So the line intersection coordinates on the first pixel: (0;0) (1;0.5) -> 25% covered pixel in average
     * 3. For the second pixel: (1;0.5) (2;1) -> 75% covered pixel in average
     * 4. The equation: (px_id * 2 + 1) / (segment_width * 2)
     *                   segment_width: the line segment which is being anti-aliased (was 2 in the example)
     *                   px_id: pixel ID from 0 to  (segment_width - 1)
     *                   result: [0..1] coverage of the pixel
     */

    /*Accelerate the common segment sizes to avoid division*/
    static const  lv_opa_t seg1[1] = {128};
    static const  lv_opa_t seg2[2] = {64, 192};
    static const  lv_opa_t seg3[3] = {42, 128, 212};
    static const  lv_opa_t seg4[4] = {32, 96, 159, 223};
    static const  lv_opa_t seg5[5] = {26,  76, 128, 178, 230};
    static const  lv_opa_t seg6[6] = {21,  64, 106, 148, 191, 234};
    static const  lv_opa_t seg7[7] = {18,  55, 91, 128, 164, 200, 237};
    static const  lv_opa_t seg8[8] = {16,  48, 80, 112, 143, 175, 207, 239};

    static const lv_opa_t * seg_map[] = {seg1, seg2, seg3, seg4,
                                         seg5, seg6, seg7, seg8
    };

    if(seg == 0) return LV_OPA_TRANSP;
    else if(seg < 8) return (uint32_t)((uint32_t)seg_map[seg - 1][px_id] * base_opa) >> 8;
    else {
        return ((px_id * 2 + 1) * base_opa) / (2 * seg);
    }

}

/**
 * Test the circle drawing is ready or not
 * @param c same as in circ_init
 * @return true if the circle is not ready yet
 */
bool lv_circ_cont(lv_point_t * c)
{
    return c->y <= c->x ? true : false;
}

/**
 * Get the next point from the circle
 * @param c same as in circ_init. The next point stored here.
 * @param tmp same as in circ_init.
 */
void lv_circ_next(lv_point_t * c, lv_coord_t * tmp)
{
    c->y++;

    if(*tmp <= 0) {
        (*tmp) += 2 * c->y + 1;   // Change in decision criterion for y -> y+1
    } else {
        c->x--;
        (*tmp) += 2 * (c->y - c->x) + 1;   // Change for y -> y+1, x -> x-1
    }
}

/**
 * Initialize an area
 * @param area_p pointer to an area
 * @param x1 left coordinate of the area
 * @param y1 top coordinate of the area
 * @param x2 right coordinate of the area
 * @param y2 bottom coordinate of the area
 */
void lv_area_set(lv_area_t * area_p, lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2)
{
    area_p->x1 = x1;
    area_p->y1 = y1;
    area_p->x2 = x2;
    area_p->y2 = y2;
}

/**
 * Get the common parts of two areas
 * @param res_p pointer to an area, the result will be stored here
 * @param a1_p pointer to the first area
 * @param a2_p pointer to the second area
 * @return false: the two area has NO common parts, res_p is invalid
 */
bool lv_area_intersect(lv_area_t * res_p, const lv_area_t * a1_p, const lv_area_t * a2_p)
{
    /* Get the smaller area from 'a1_p' and 'a2_p' */
    res_p->x1 = LV_MATH_MAX(a1_p->x1, a2_p->x1);
    res_p->y1 = LV_MATH_MAX(a1_p->y1, a2_p->y1);
    res_p->x2 = LV_MATH_MIN(a1_p->x2, a2_p->x2);
    res_p->y2 = LV_MATH_MIN(a1_p->y2, a2_p->y2);

    /*If x1 or y1 greater then x2 or y2 then the areas union is empty*/
    bool union_ok = true;
    if((res_p->x1 > res_p->x2) ||
       (res_p->y1 > res_p->y2)) {
        union_ok = false;
    }

    return union_ok;
}

void fill_fp(const lv_area_t * coords, const lv_area_t * mask, lv_color_t color, lv_opa_t opa);
/**
 * Put a pixel to the display
 * @param x x coordinate of the pixel
 * @param y y coordinate of the pixel
 * @param mask_p the pixel will be drawn on this area
 * @param color color of the pixel
 * @param opa opacity (ignored, only for compatibility with lv_vpx)
 */
void px_fp(lv_coord_t x, lv_coord_t y, const lv_area_t * mask, lv_color_t color, lv_opa_t opa)  {
//    printf("%d,", opa);
    lz_color5658_t colorx;
    colorx.alpha = opa;
    colorx.color.rgb565 = color.full;
    printf("%d,", color.full);
    lz_draw_frame(x, y, 1, 1, colorx);

//    (void)opa;      /*Opa is used only for compatibility with lv_vpx*/
//
//    lv_area_t area;
//    area.x1 = x;
//    area.y1 = y;
//    area.x2 = x;
//    area.y2 = y;
//
//    fill_fp(&area, mask, color, LV_OPA_COVER);
}

/**
 * Fill an area on the display
 * @param cords_p coordinates of the area to fill
 * @param mask_p fill only o this mask
 * @param color fill color
 * @param opa opacity (ignored, only for compatibility with lv_vfill)
 */
void fill_fp(const lv_area_t * coords, const lv_area_t * mask, lv_color_t color, lv_opa_t opa) {
    lz_color5658_t colorx;
    colorx.alpha = opa;
    colorx.color.rgb565 = color.full;
    lz_draw_frame(coords->x1, coords->y1, coords->x2 - coords->x1 + 1, coords->y2 - coords->y1 + 1, colorx);

//    (void)opa;      /*Opa is used only for compatibility with lv_vfill*/
//
//    lv_area_t masked_area;
//    bool union_ok = true;
//
//    if(mask != NULL) {
//        union_ok = lv_area_intersect(&masked_area, coords, mask);
//    } else {
//        lv_area_t scr_area;
//        lv_area_set(&scr_area, 0, 0, LV_HOR_RES - 1, LV_VER_RES - 1);
//        union_ok = lv_area_intersect(&masked_area, coords, &scr_area);
//    }
//
//    if(union_ok != false) {
//        lz_color5658_t colorx;
//        colorx.alpha = opa;
//        colorx.color.rgb565 = color.full;
//        lz_draw_frame(masked_area.x1, masked_area.y1, masked_area.x2 - masked_area.x1, masked_area.y2 - masked_area.y1, colorx);
////        lv_disp_fill(masked_area.x1, masked_area.y1, masked_area.x2, masked_area.y2, color);
//    }
}

/**
 * Draw the top and bottom parts (corners) of a rectangle
 * @param coords the coordinates of the original rectangle
 * @param mask the rectangle will be drawn only  on this area
 * @param rects_p pointer to a rectangle style
 * @param opa_scale scale down all opacities by the factor
 */
void lv_draw_rect_main_corner(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale)
{
    uint16_t radius = style->body.radius;

    lv_color_t mcolor = style->body.main_color;
    lv_color_t gcolor = style->body.grad_color;
    lv_color_t act_color;
    lv_opa_t opa = opa_scale == LV_OPA_COVER ? style->body.opa : (uint16_t)((uint16_t) style->body.opa * opa_scale) >> 8;
    uint8_t mix;
    lv_coord_t height = lv_area_get_height(coords);
    lv_coord_t width = lv_area_get_width(coords);

    radius = lv_draw_cont_radius_corr(radius, width, height);

    lv_point_t lt_origo;   /*Left  Top    origo*/
    lv_point_t lb_origo;   /*Left  Bottom origo*/
    lv_point_t rt_origo;   /*Right Top    origo*/
    lv_point_t rb_origo;   /*Left  Bottom origo*/

    lt_origo.x = coords->x1 + radius + LV_ANTIALIAS;
    lt_origo.y = coords->y1 + radius + LV_ANTIALIAS;

    lb_origo.x = coords->x1 + radius + LV_ANTIALIAS;
    lb_origo.y = coords->y2 - radius - LV_ANTIALIAS;

    rt_origo.x = coords->x2 - radius - LV_ANTIALIAS;
    rt_origo.y = coords->y1 + radius + LV_ANTIALIAS;

    rb_origo.x = coords->x2 - radius - LV_ANTIALIAS;
    rb_origo.y = coords->y2 - radius - LV_ANTIALIAS;

    lv_area_t edge_top_area;
    lv_area_t mid_top_area;
    lv_area_t mid_bot_area;
    lv_area_t edge_bot_area;

    lv_point_t cir;
    lv_coord_t cir_tmp;
    lv_circ_init(&cir, &cir_tmp, radius);

    /*Init the areas*/
    lv_area_set(&mid_bot_area,  lb_origo.x + LV_CIRC_OCT4_X(cir),
                lb_origo.y + LV_CIRC_OCT4_Y(cir),
                rb_origo.x + LV_CIRC_OCT1_X(cir),
                rb_origo.y + LV_CIRC_OCT1_Y(cir));

    lv_area_set(&edge_bot_area, lb_origo.x + LV_CIRC_OCT3_X(cir),
                lb_origo.y + LV_CIRC_OCT3_Y(cir),
                rb_origo.x + LV_CIRC_OCT2_X(cir),
                rb_origo.y + LV_CIRC_OCT2_Y(cir));

    lv_area_set(&mid_top_area,  lt_origo.x + LV_CIRC_OCT5_X(cir),
                lt_origo.y + LV_CIRC_OCT5_Y(cir),
                rt_origo.x + LV_CIRC_OCT8_X(cir),
                rt_origo.y + LV_CIRC_OCT8_Y(cir));

    lv_area_set(&edge_top_area, lt_origo.x + LV_CIRC_OCT6_X(cir),
                lt_origo.y + LV_CIRC_OCT6_Y(cir),
                rt_origo.x + LV_CIRC_OCT7_X(cir),
                rt_origo.y + LV_CIRC_OCT7_Y(cir));
#if LV_ANTIALIAS
    /*Store some internal states for anti-aliasing*/
    lv_coord_t out_y_seg_start = 0;
    lv_coord_t out_y_seg_end = 0;
    lv_coord_t out_x_last = radius;

    lv_color_t aa_color_hor_top;
    lv_color_t aa_color_hor_bottom;
    lv_color_t aa_color_ver;
#endif

    while(lv_circ_cont(&cir)) {
#if LV_ANTIALIAS != 0
        /*New step in y on the outter circle*/
        if(out_x_last != cir.x) {
            out_y_seg_end = cir.y;
            lv_coord_t seg_size = out_y_seg_end - out_y_seg_start;
            lv_point_t aa_p;

            aa_p.x = out_x_last;
            aa_p.y = out_y_seg_start;

            mix = (uint32_t)((uint32_t)(radius - out_x_last) * 255) / height;
            aa_color_hor_top = lv_color_mix(gcolor, mcolor, mix);
            aa_color_hor_bottom = lv_color_mix(mcolor, gcolor, mix);

            lv_coord_t i;
            for(i = 0; i  < seg_size; i++) {
                lv_opa_t aa_opa;
                if(seg_size > CIRCLE_AA_NON_LINEAR_OPA_THRESHOLD) {    /*Use non-linear opa mapping on the first segment*/
                    aa_opa = antialias_get_opa_circ(seg_size, i, opa);
                } else {
                    aa_opa = opa - lv_draw_aa_get_opa(seg_size, i, opa);
                }

                px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p) + i, rb_origo.y + LV_CIRC_OCT2_Y(aa_p) + 1, mask, aa_color_hor_bottom, aa_opa);
                px_fp(lb_origo.x + LV_CIRC_OCT3_X(aa_p) - i, lb_origo.y + LV_CIRC_OCT3_Y(aa_p) + 1, mask, aa_color_hor_bottom, aa_opa);
                px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p) - i, lt_origo.y + LV_CIRC_OCT6_Y(aa_p) - 1, mask, aa_color_hor_top, aa_opa);
                px_fp(rt_origo.x + LV_CIRC_OCT7_X(aa_p) + i, rt_origo.y + LV_CIRC_OCT7_Y(aa_p) - 1, mask, aa_color_hor_top, aa_opa);

                mix = (uint32_t)((uint32_t)(radius - out_y_seg_start + i) * 255) / height;
                aa_color_ver = lv_color_mix(mcolor, gcolor, mix);
                px_fp(rb_origo.x + LV_CIRC_OCT1_X(aa_p) + 1, rb_origo.y + LV_CIRC_OCT1_Y(aa_p) + i, mask, aa_color_ver, aa_opa);
                px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p) - 1, lb_origo.y + LV_CIRC_OCT4_Y(aa_p) + i, mask, aa_color_ver, aa_opa);

                aa_color_ver = lv_color_mix(gcolor, mcolor, mix);
                px_fp(lt_origo.x + LV_CIRC_OCT5_X(aa_p) - 1, lt_origo.y + LV_CIRC_OCT5_Y(aa_p) - i, mask, aa_color_ver, aa_opa);
                px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p) + 1, rt_origo.y + LV_CIRC_OCT8_Y(aa_p) - i, mask, aa_color_ver, aa_opa);
            }

            out_x_last = cir.x;
            out_y_seg_start = out_y_seg_end;
        }
#endif
        uint8_t edge_top_refr = 0;
        uint8_t mid_top_refr = 0;
        uint8_t mid_bot_refr = 0;
        uint8_t edge_bot_refr = 0;

        /* If a new row coming draw the previous
         * The y coordinate can remain the same so wait for a new*/
        if(mid_bot_area.y1 != LV_CIRC_OCT4_Y(cir) + lb_origo.y) mid_bot_refr = 1;

        if(edge_bot_area.y1 != LV_CIRC_OCT2_Y(cir) + lb_origo.y) edge_bot_refr = 1;

        if(mid_top_area.y1 != LV_CIRC_OCT8_Y(cir) + lt_origo.y) mid_top_refr = 1;

        if(edge_top_area.y1 != LV_CIRC_OCT7_Y(cir) + lt_origo.y) edge_top_refr = 1;

        /*Draw the areas which are not disabled*/
        if(edge_top_refr != 0) {
            if(mcolor.full == gcolor.full) act_color = mcolor;
            else {
                mix = (uint32_t)((uint32_t)(coords->y2 - edge_top_area.y1)  * 255) / height;
                act_color = lv_color_mix(mcolor, gcolor, mix);
            }
            fill_fp(&edge_top_area, mask, act_color, opa);
        }

        if(mid_top_refr != 0) {
            if(mcolor.full == gcolor.full) act_color = mcolor;
            else {
                mix = (uint32_t)((uint32_t)(coords->y2 - mid_top_area.y1) * 255) / height;
                act_color = lv_color_mix(mcolor, gcolor, mix);
            }
            fill_fp(&mid_top_area, mask, act_color, opa);
        }

        if(mid_bot_refr != 0) {
            if(mcolor.full == gcolor.full) act_color = mcolor;
            else {
                mix = (uint32_t)((uint32_t)(coords->y2 - mid_bot_area.y1) * 255) / height;
                act_color = lv_color_mix(mcolor, gcolor, mix);
            }
            fill_fp(&mid_bot_area, mask, act_color, opa);
        }

        if(edge_bot_refr != 0) {

            if(mcolor.full == gcolor.full) act_color = mcolor;
            else {
                mix = (uint32_t)((uint32_t)(coords->y2 - edge_bot_area.y1) * 255) / height;
                act_color = lv_color_mix(mcolor, gcolor, mix);
            }
            fill_fp(&edge_bot_area, mask, act_color, opa);
        }

        /*Save the current coordinates*/
        lv_area_set(&mid_bot_area,  lb_origo.x + LV_CIRC_OCT4_X(cir),
                    lb_origo.y + LV_CIRC_OCT4_Y(cir),
                    rb_origo.x + LV_CIRC_OCT1_X(cir),
                    rb_origo.y + LV_CIRC_OCT1_Y(cir));

        lv_area_set(&edge_bot_area, lb_origo.x + LV_CIRC_OCT3_X(cir),
                    lb_origo.y + LV_CIRC_OCT3_Y(cir),
                    rb_origo.x + LV_CIRC_OCT2_X(cir),
                    rb_origo.y + LV_CIRC_OCT2_Y(cir));

        lv_area_set(&mid_top_area,  lt_origo.x + LV_CIRC_OCT5_X(cir),
                    lt_origo.y + LV_CIRC_OCT5_Y(cir),
                    rt_origo.x + LV_CIRC_OCT8_X(cir),
                    rt_origo.y + LV_CIRC_OCT8_Y(cir));

        lv_area_set(&edge_top_area, lt_origo.x + LV_CIRC_OCT6_X(cir),
                    lt_origo.y + LV_CIRC_OCT6_Y(cir),
                    rt_origo.x + LV_CIRC_OCT7_X(cir),
                    rt_origo.y + LV_CIRC_OCT7_Y(cir));

        lv_circ_next(&cir, &cir_tmp);
    }

    if(mcolor.full == gcolor.full) act_color = mcolor;
    else {
        mix = (uint32_t)((uint32_t)(coords->y2 - edge_top_area.y1)  * 255) / height;
        act_color = lv_color_mix(mcolor, gcolor, mix);
    }
    fill_fp(&edge_top_area, mask, act_color, opa);

    if(edge_top_area.y1 != mid_top_area.y1) {

        if(mcolor.full == gcolor.full) act_color = mcolor;
        else {
            mix = (uint32_t)((uint32_t)(coords->y2 - mid_top_area.y1) * 255) / height;
            act_color = lv_color_mix(mcolor, gcolor, mix);
        }
        fill_fp(&mid_top_area, mask, act_color, opa);
    }

    if(mcolor.full == gcolor.full) act_color = mcolor;
    else {
        mix = (uint32_t)((uint32_t)(coords->y2 - mid_bot_area.y1) * 255) / height;
        act_color = lv_color_mix(mcolor, gcolor, mix);
    }
    fill_fp(&mid_bot_area, mask, act_color, opa);

    if(edge_bot_area.y1 != mid_bot_area.y1) {

        if(mcolor.full == gcolor.full) act_color = mcolor;
        else {
            mix = (uint32_t)((uint32_t)(coords->y2 - edge_bot_area.y1) * 255) / height;
            act_color = lv_color_mix(mcolor, gcolor, mix);
        }
        fill_fp(&edge_bot_area, mask, act_color, opa);
    }


#if LV_ANTIALIAS
    /*The first and the last line is not drawn*/
    edge_top_area.x1 = coords->x1 + radius + 2;
    edge_top_area.x2 = coords->x2 - radius - 2;
    edge_top_area.y1 = coords->y1;
    edge_top_area.y2 = coords->y1;
    fill_fp(&edge_top_area, mask, style->body.main_color, opa);

    edge_top_area.y1 = coords->y2;
    edge_top_area.y2 = coords->y2;
    fill_fp(&edge_top_area, mask, style->body.grad_color, opa);

    /*Last parts of the anti-alias*/
    out_y_seg_end = cir.y;
    lv_coord_t seg_size = out_y_seg_end - out_y_seg_start;
    lv_point_t aa_p;

    aa_p.x = out_x_last;
    aa_p.y = out_y_seg_start;

    mix = (uint32_t)((uint32_t)(radius - out_x_last) * 255) / height;
    aa_color_hor_bottom = lv_color_mix(gcolor, mcolor, mix);
    aa_color_hor_top = lv_color_mix(mcolor, gcolor, mix);

    lv_coord_t i;
    for(i = 0; i  < seg_size; i++) {
        lv_opa_t aa_opa = opa - lv_draw_aa_get_opa(seg_size, i, opa);
        px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p) + i, rb_origo.y + LV_CIRC_OCT2_Y(aa_p) + 1, mask, aa_color_hor_top, aa_opa);
        px_fp(lb_origo.x + LV_CIRC_OCT3_X(aa_p) - i, lb_origo.y + LV_CIRC_OCT3_Y(aa_p) + 1, mask, aa_color_hor_top, aa_opa);
        px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p) - i, lt_origo.y + LV_CIRC_OCT6_Y(aa_p) - 1, mask, aa_color_hor_bottom, aa_opa);
        px_fp(rt_origo.x + LV_CIRC_OCT7_X(aa_p) + i, rt_origo.y + LV_CIRC_OCT7_Y(aa_p) - 1, mask, aa_color_hor_bottom, aa_opa);

        mix = (uint32_t)((uint32_t)(radius - out_y_seg_start + i) * 255) / height;
        aa_color_ver = lv_color_mix(mcolor, gcolor, mix);
        px_fp(rb_origo.x + LV_CIRC_OCT1_X(aa_p) + 1, rb_origo.y + LV_CIRC_OCT1_Y(aa_p) + i, mask, aa_color_ver, aa_opa);
        px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p) - 1, lb_origo.y + LV_CIRC_OCT4_Y(aa_p) + i, mask, aa_color_ver, aa_opa);

        aa_color_ver = lv_color_mix(gcolor, mcolor, mix);
        px_fp(lt_origo.x + LV_CIRC_OCT5_X(aa_p) - 1, lt_origo.y + LV_CIRC_OCT5_Y(aa_p) - i, mask, aa_color_ver, aa_opa);
        px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p) + 1, rt_origo.y + LV_CIRC_OCT8_Y(aa_p) - i, mask, aa_color_ver, aa_opa);
    }

    /*In some cases the last pixel is not drawn*/
    if(LV_MATH_ABS(aa_p.x - aa_p.y) == seg_size) {
        aa_p.x = out_x_last;
        aa_p.y = out_x_last;

        mix = (uint32_t)((uint32_t)(out_x_last) * 255) / height;
        aa_color_hor_top = lv_color_mix(gcolor, mcolor, mix);
        aa_color_hor_bottom = lv_color_mix(mcolor, gcolor, mix);

        lv_opa_t aa_opa = opa >> 1;
        px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p), rb_origo.y + LV_CIRC_OCT2_Y(aa_p), mask, aa_color_hor_bottom, aa_opa);
        px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p), lb_origo.y + LV_CIRC_OCT4_Y(aa_p), mask, aa_color_hor_bottom, aa_opa);
        px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p), lt_origo.y + LV_CIRC_OCT6_Y(aa_p), mask, aa_color_hor_top, aa_opa);
        px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p), rt_origo.y + LV_CIRC_OCT8_Y(aa_p), mask, aa_color_hor_top, aa_opa);
    }
#endif
}

/**
 * Draw the corners of a rectangle border
 * @param coords the coordinates of the original rectangle
 * @param mask the rectangle will be drawn only  on this area
 * @param style pointer to a style
 * @param opa_scale scale down all opacities by the factor
 */
void lv_draw_rect_border_corner(const lv_area_t * coords, const lv_area_t * mask, const  lv_style_t * style, lv_opa_t opa_scale)
{
    uint16_t radius = style->body.radius ;
    uint16_t bwidth = style->body.border.width;
    lv_color_t color = style->body.border.color;
    lv_border_part_t part = style->body.border.part;
    lv_opa_t opa = opa_scale == LV_OPA_COVER ? style->body.border.opa : (uint16_t)((uint16_t) style->body.border.opa * opa_scale) >> 8;
    /*0 px border width drawn as 1 px, so decrement the bwidth*/
    bwidth--;

#if LV_ANTIALIAS
    bwidth--;    /*Because of anti-aliasing the border seems one pixel ticker*/
#endif

    lv_coord_t width = lv_area_get_width(coords);
    lv_coord_t height = lv_area_get_height(coords);

    radius = lv_draw_cont_radius_corr(radius, width, height);

    lv_point_t lt_origo;   /*Left  Top    origo*/
    lv_point_t lb_origo;   /*Left  Bottom origo*/
    lv_point_t rt_origo;   /*Right Top    origo*/
    lv_point_t rb_origo;   /*Left  Bottom origo*/

    lt_origo.x = coords->x1 + radius + LV_ANTIALIAS;
    lt_origo.y = coords->y1 + radius + LV_ANTIALIAS;

    lb_origo.x = coords->x1 + radius + LV_ANTIALIAS;
    lb_origo.y = coords->y2 - radius - LV_ANTIALIAS;

    rt_origo.x = coords->x2 - radius - LV_ANTIALIAS;
    rt_origo.y = coords->y1 + radius + LV_ANTIALIAS;

    rb_origo.x = coords->x2 - radius - LV_ANTIALIAS;
    rb_origo.y = coords->y2 - radius - LV_ANTIALIAS;

    lv_point_t cir_out;
    lv_coord_t tmp_out;
    lv_circ_init(&cir_out, &tmp_out, radius);

    lv_point_t cir_in;
    lv_coord_t tmp_in;
    lv_coord_t radius_in = radius - bwidth;

    if(radius_in < 0) {
        radius_in = 0;
    }

    lv_circ_init(&cir_in, &tmp_in, radius_in);

    lv_area_t circ_area;
    lv_coord_t act_w1;
    lv_coord_t act_w2;

#if LV_ANTIALIAS
    /*Store some internal states for anti-aliasing*/
    lv_coord_t out_y_seg_start = 0;
    lv_coord_t out_y_seg_end = 0;
    lv_coord_t out_x_last = radius;


    lv_coord_t in_y_seg_start = 0;
    lv_coord_t in_y_seg_end = 0;
    lv_coord_t in_x_last = radius - bwidth;
#endif

    while(cir_out.y <= cir_out.x) {

        /*Calculate the actual width to avoid overwriting pixels*/
        if(cir_in.y < cir_in.x) {
            act_w1 = cir_out.x - cir_in.x;
            act_w2 = act_w1;
        } else {
            act_w1 = cir_out.x - cir_out.y;
            act_w2 = act_w1 - 1;
        }

#if LV_ANTIALIAS != 0
        /*New step in y on the outter circle*/
        if(out_x_last != cir_out.x) {
            out_y_seg_end = cir_out.y;
            lv_coord_t seg_size = out_y_seg_end - out_y_seg_start;
            lv_point_t aa_p;

            aa_p.x = out_x_last;
            aa_p.y = out_y_seg_start;

            lv_coord_t i;
            for(i = 0; i  < seg_size; i++) {
                lv_opa_t aa_opa;

                if(seg_size > CIRCLE_AA_NON_LINEAR_OPA_THRESHOLD) {    /*Use non-linear opa mapping on the first segment*/
                    aa_opa = antialias_get_opa_circ(seg_size, i, opa);
                } else {
                    aa_opa = opa - lv_draw_aa_get_opa(seg_size, i, opa);
                }

                if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
                    px_fp(rb_origo.x + LV_CIRC_OCT1_X(aa_p) + 1, rb_origo.y + LV_CIRC_OCT1_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
                    px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p) + i, rb_origo.y + LV_CIRC_OCT2_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
                }

                if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
                    px_fp(lb_origo.x + LV_CIRC_OCT3_X(aa_p) - i, lb_origo.y + LV_CIRC_OCT3_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
                    px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p) - 1, lb_origo.y + LV_CIRC_OCT4_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
                }


                if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
                    px_fp(lt_origo.x + LV_CIRC_OCT5_X(aa_p) - 1, lt_origo.y + LV_CIRC_OCT5_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
                    px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p) - i, lt_origo.y + LV_CIRC_OCT6_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
                }

                if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
                    px_fp(rt_origo.x + LV_CIRC_OCT7_X(aa_p) + i, rt_origo.y + LV_CIRC_OCT7_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
                    px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p) + 1, rt_origo.y + LV_CIRC_OCT8_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
                }
            }

            out_x_last = cir_out.x;
            out_y_seg_start = out_y_seg_end;
        }

        /*New step in y on the inner circle*/
        if(in_x_last != cir_in.x) {
            in_y_seg_end = cir_out.y;
            lv_coord_t seg_size = in_y_seg_end - in_y_seg_start;
            lv_point_t aa_p;

            aa_p.x = in_x_last;
            aa_p.y = in_y_seg_start;

            lv_coord_t i;
            for(i = 0; i  < seg_size; i++) {
                lv_opa_t aa_opa;

                if(seg_size > CIRCLE_AA_NON_LINEAR_OPA_THRESHOLD) {    /*Use non-linear opa mapping on the first segment*/
                    aa_opa = opa - antialias_get_opa_circ(seg_size, i, opa);
                } else {
                    aa_opa = lv_draw_aa_get_opa(seg_size, i, opa);
                }

                if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
                    px_fp(rb_origo.x + LV_CIRC_OCT1_X(aa_p) - 1, rb_origo.y + LV_CIRC_OCT1_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
                }

                if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
                    px_fp(lb_origo.x + LV_CIRC_OCT3_X(aa_p) - i, lb_origo.y + LV_CIRC_OCT3_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
                }

                if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
                    px_fp(lt_origo.x + LV_CIRC_OCT5_X(aa_p) + 1, lt_origo.y + LV_CIRC_OCT5_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
                }

                if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
                    px_fp(rt_origo.x + LV_CIRC_OCT7_X(aa_p) + i, rt_origo.y + LV_CIRC_OCT7_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
                }

                /*Be sure the pixels on the middle are not drawn twice*/
                if(LV_CIRC_OCT1_X(aa_p) - 1 != LV_CIRC_OCT2_X(aa_p) + i) {
                    if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
                        px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p) + i, rb_origo.y + LV_CIRC_OCT2_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
                    }

                    if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
                        px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p) + 1, lb_origo.y + LV_CIRC_OCT4_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
                    }

                    if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
                        px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p) - i, lt_origo.y + LV_CIRC_OCT6_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
                    }

                    if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
                        px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p) - 1, rt_origo.y + LV_CIRC_OCT8_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
                    }
                }

            }

            in_x_last = cir_in.x;
            in_y_seg_start = in_y_seg_end;

        }

#endif


        /*Draw the octets to the right bottom corner*/
        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
            circ_area.x1 = rb_origo.x + LV_CIRC_OCT1_X(cir_out) - act_w2;
            circ_area.x2 = rb_origo.x + LV_CIRC_OCT1_X(cir_out);
            circ_area.y1 = rb_origo.y + LV_CIRC_OCT1_Y(cir_out);
            circ_area.y2 = rb_origo.y + LV_CIRC_OCT1_Y(cir_out);
            fill_fp(&circ_area, mask, color, opa);

            circ_area.x1 = rb_origo.x + LV_CIRC_OCT2_X(cir_out);
            circ_area.x2 = rb_origo.x + LV_CIRC_OCT2_X(cir_out);
            circ_area.y1 = rb_origo.y + LV_CIRC_OCT2_Y(cir_out) - act_w1;
            circ_area.y2 = rb_origo.y + LV_CIRC_OCT2_Y(cir_out);
            fill_fp(&circ_area, mask, color, opa);
        }

        /*Draw the octets to the left bottom corner*/
        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
            circ_area.x1 = lb_origo.x + LV_CIRC_OCT3_X(cir_out);
            circ_area.x2 = lb_origo.x + LV_CIRC_OCT3_X(cir_out);
            circ_area.y1 = lb_origo.y + LV_CIRC_OCT3_Y(cir_out) - act_w2;
            circ_area.y2 = lb_origo.y + LV_CIRC_OCT3_Y(cir_out);
            fill_fp(&circ_area, mask, color, opa);

            circ_area.x1 = lb_origo.x + LV_CIRC_OCT4_X(cir_out);
            circ_area.x2 = lb_origo.x + LV_CIRC_OCT4_X(cir_out) + act_w1;
            circ_area.y1 = lb_origo.y + LV_CIRC_OCT4_Y(cir_out);
            circ_area.y2 = lb_origo.y + LV_CIRC_OCT4_Y(cir_out);
            fill_fp(&circ_area, mask, color, opa);
        }

        /*Draw the octets to the left top corner*/
        if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
            if(lb_origo.y + LV_CIRC_OCT4_Y(cir_out) > lt_origo.y + LV_CIRC_OCT5_Y(cir_out)) {
                /*Don't draw if the lines are common in the middle*/
                circ_area.x1 = lt_origo.x + LV_CIRC_OCT5_X(cir_out);
                circ_area.x2 = lt_origo.x + LV_CIRC_OCT5_X(cir_out) + act_w2;
                circ_area.y1 = lt_origo.y + LV_CIRC_OCT5_Y(cir_out);
                circ_area.y2 = lt_origo.y + LV_CIRC_OCT5_Y(cir_out);
                fill_fp(&circ_area, mask, color, opa);
            }

            circ_area.x1 = lt_origo.x + LV_CIRC_OCT6_X(cir_out);
            circ_area.x2 = lt_origo.x + LV_CIRC_OCT6_X(cir_out);
            circ_area.y1 = lt_origo.y + LV_CIRC_OCT6_Y(cir_out);
            circ_area.y2 = lt_origo.y + LV_CIRC_OCT6_Y(cir_out) + act_w1;
            fill_fp(&circ_area, mask, color, opa);
        }

        /*Draw the octets to the right top corner*/
        if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
            circ_area.x1 = rt_origo.x + LV_CIRC_OCT7_X(cir_out);
            circ_area.x2 = rt_origo.x + LV_CIRC_OCT7_X(cir_out);
            circ_area.y1 = rt_origo.y + LV_CIRC_OCT7_Y(cir_out);
            circ_area.y2 = rt_origo.y + LV_CIRC_OCT7_Y(cir_out) + act_w2;
            fill_fp(&circ_area, mask, color, opa);

            /*Don't draw if the lines are common in the middle*/
            if(rb_origo.y + LV_CIRC_OCT1_Y(cir_out) > rt_origo.y + LV_CIRC_OCT8_Y(cir_out)) {
                circ_area.x1 = rt_origo.x + LV_CIRC_OCT8_X(cir_out) - act_w1;
                circ_area.x2 = rt_origo.x + LV_CIRC_OCT8_X(cir_out);
                circ_area.y1 = rt_origo.y + LV_CIRC_OCT8_Y(cir_out);
                circ_area.y2 = rt_origo.y + LV_CIRC_OCT8_Y(cir_out);
                fill_fp(&circ_area, mask, color, opa);
            }
        }
        lv_circ_next(&cir_out, &tmp_out);

        /*The internal circle will be ready faster
         * so check it! */
        if(cir_in.y < cir_in.x) {
            lv_circ_next(&cir_in, &tmp_in);
        }
    }


#if LV_ANTIALIAS != 0

    /*Last parts of the outer anti-alias*/
    out_y_seg_end = cir_out.y;
    lv_coord_t seg_size = out_y_seg_end - out_y_seg_start;
    lv_point_t aa_p;

    aa_p.x = out_x_last;
    aa_p.y = out_y_seg_start;

    lv_coord_t i;
    for(i = 0; i  < seg_size; i++) {
        lv_opa_t aa_opa = opa - lv_draw_aa_get_opa(seg_size, i, opa);
        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
            px_fp(rb_origo.x + LV_CIRC_OCT1_X(aa_p) + 1, rb_origo.y + LV_CIRC_OCT1_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
            px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p) + i, rb_origo.y + LV_CIRC_OCT2_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
            px_fp(lb_origo.x + LV_CIRC_OCT3_X(aa_p) - i, lb_origo.y + LV_CIRC_OCT3_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
            px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p) - 1, lb_origo.y + LV_CIRC_OCT4_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
            px_fp(lt_origo.x + LV_CIRC_OCT5_X(aa_p) - 1, lt_origo.y + LV_CIRC_OCT5_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
            px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p) - i, lt_origo.y + LV_CIRC_OCT6_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
            px_fp(rt_origo.x + LV_CIRC_OCT7_X(aa_p) + i, rt_origo.y + LV_CIRC_OCT7_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
            px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p) + 1, rt_origo.y + LV_CIRC_OCT8_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
        }
    }

    /*In some cases the last pixel in the outer middle is not drawn*/
    if(LV_MATH_ABS(aa_p.x - aa_p.y) == seg_size) {
        aa_p.x = out_x_last;
        aa_p.y = out_x_last;

        lv_opa_t aa_opa = opa >> 1;

        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
            px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p), rb_origo.y + LV_CIRC_OCT2_Y(aa_p), mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
            px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p), lb_origo.y + LV_CIRC_OCT4_Y(aa_p), mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
            px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p), lt_origo.y + LV_CIRC_OCT6_Y(aa_p), mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
            px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p), rt_origo.y + LV_CIRC_OCT8_Y(aa_p), mask, style->body.border.color, aa_opa);
        }
    }

    /*Last parts of the inner anti-alias*/
    in_y_seg_end = cir_in.y;
    aa_p.x = in_x_last;
    aa_p.y = in_y_seg_start;
    seg_size = in_y_seg_end - in_y_seg_start;

    for(i = 0; i  < seg_size; i++) {
        lv_opa_t aa_opa =  lv_draw_aa_get_opa(seg_size, i, opa);
        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
            px_fp(rb_origo.x + LV_CIRC_OCT1_X(aa_p) - 1, rb_origo.y + LV_CIRC_OCT1_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
            px_fp(lb_origo.x + LV_CIRC_OCT3_X(aa_p) - i, lb_origo.y + LV_CIRC_OCT3_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
            px_fp(lt_origo.x + LV_CIRC_OCT5_X(aa_p) + 1, lt_origo.y + LV_CIRC_OCT5_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
        }

        if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
            px_fp(rt_origo.x + LV_CIRC_OCT7_X(aa_p) + i, rt_origo.y + LV_CIRC_OCT7_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
        }

        if(LV_CIRC_OCT1_X(aa_p) - 1 != LV_CIRC_OCT2_X(aa_p) + i) {
            if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
                px_fp(rb_origo.x + LV_CIRC_OCT2_X(aa_p) + i, rb_origo.y + LV_CIRC_OCT2_Y(aa_p) - 1, mask, style->body.border.color, aa_opa);
            }

            if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
                px_fp(lb_origo.x + LV_CIRC_OCT4_X(aa_p) + 1, lb_origo.y + LV_CIRC_OCT4_Y(aa_p) + i, mask, style->body.border.color, aa_opa);
            }

            if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
                px_fp(lt_origo.x + LV_CIRC_OCT6_X(aa_p) - i, lt_origo.y + LV_CIRC_OCT6_Y(aa_p) + 1, mask, style->body.border.color, aa_opa);
            }

            if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
                px_fp(rt_origo.x + LV_CIRC_OCT8_X(aa_p) - 1, rt_origo.y + LV_CIRC_OCT8_Y(aa_p) - i, mask, style->body.border.color, aa_opa);
            }
        }
    }

#endif

}


/**
 * Draw the middle part (rectangular) of a rectangle
 * @param coords the coordinates of the original rectangle
 * @param mask the rectangle will be drawn only  on this area
 * @param rects_p pointer to a rectangle style
 * @param opa_scale scale down all opacities by the factor
 */
static void lv_draw_rect_main_mid(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale)
{
    uint16_t radius = style->body.radius;

    lv_color_t mcolor = style->body.main_color;
    lv_color_t gcolor = style->body.grad_color;
    uint8_t mix;
    lv_coord_t height = lv_area_get_height(coords);
    lv_coord_t width = lv_area_get_width(coords);
    lv_opa_t opa = opa_scale == LV_OPA_COVER ? style->body.opa : (uint16_t)((uint16_t) style->body.opa * opa_scale) >> 8;

    radius = lv_draw_cont_radius_corr(radius, width, height);

    /*If the radius is too big then there is no body*/
    if(radius > height / 2)  return;

    lv_area_t work_area;
    work_area.x1 = coords->x1;
    work_area.x2 = coords->x2;

    if(mcolor.full == gcolor.full) {
        work_area.y1 = coords->y1 + radius;
        work_area.y2 = coords->y2 - radius;

        if(style->body.radius != 0) {
#if LV_ANTIALIAS
            work_area.y1 += 2;
            work_area.y2 -= 2;
#else
            work_area.y1 += 1;
            work_area.y2 -= 1;
#endif
        }

        fill_fp(&work_area, mask, mcolor, opa);
    } else {
        lv_coord_t row;
        lv_coord_t row_start = coords->y1 + radius;
        lv_coord_t row_end = coords->y2 - radius;
        lv_color_t act_color;

        if(style->body.radius != 0) {
#if LV_ANTIALIAS
            row_start += 2;
            row_end -= 2;
#else
            row_start += 1;
            row_end -= 1;
#endif
        }
        if(row_start < 0) row_start = 0;

        for(row = row_start; row <= row_end; row ++) {
            work_area.y1 = row;
            work_area.y2 = row;
            mix = (uint32_t)((uint32_t)(coords->y2 - work_area.y1) * 255) / height;
            act_color = lv_color_mix(mcolor, gcolor, mix);

            fill_fp(&work_area, mask, act_color, opa);
        }
    }
}


/**
 * Draw the straight parts of a rectangle border
 * @param coords the coordinates of the original rectangle
 * @param mask_ the rectangle will be drawn only  on this area
 * @param rstyle pointer to a rectangle style
 * @param opa_scale scale down all opacities by the factor
 */
static void lv_draw_rect_border_straight(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale)
{
    uint16_t radius = style->body.radius;

    lv_coord_t width = lv_area_get_width(coords);
    lv_coord_t height = lv_area_get_height(coords);
    uint16_t bwidth = style->body.border.width;
    lv_opa_t opa = opa_scale == LV_OPA_COVER ? style->body.border.opa : (uint16_t)((uint16_t) style->body.border.opa * opa_scale) >> 8;
    lv_border_part_t part = style->body.border.part;
    lv_color_t color = style->body.border.color;
    lv_area_t work_area;
    lv_coord_t length_corr = 0;
    lv_coord_t corner_size = 0;

    /*the 0 px border width drawn as 1 px, so decrement the b_width*/
    bwidth--;

    radius = lv_draw_cont_radius_corr(radius, width, height);

    if(radius < bwidth) {
        length_corr = bwidth - radius - LV_ANTIALIAS;
        corner_size = bwidth;
    } else {
        corner_size = radius + LV_ANTIALIAS;
    }

    /*If radius == 0 is a special case*/
    if(style->body.radius == 0) {
        /*Left top corner*/
        if(part & LV_BORDER_TOP) {
            work_area.x1 = coords->x1;
            work_area.x2 = coords->x2;
            work_area.y1 = coords->y1;
            work_area.y2 = coords->y1 + bwidth;
            fill_fp(&work_area, mask, color, opa);
        }

        /*Right top corner*/
        if(part & LV_BORDER_RIGHT) {
            work_area.x1 = coords->x2 - bwidth;
            work_area.x2 = coords->x2;
            work_area.y1 = coords->y1 + (part & LV_BORDER_TOP ? bwidth + 1 : 0);
            work_area.y2 = coords->y2 - (part & LV_BORDER_BOTTOM ? bwidth + 1 : 0);
            fill_fp(&work_area, mask, color, opa);
        }

        /*Left bottom corner*/
        if(part & LV_BORDER_LEFT) {
            work_area.x1 = coords->x1;
            work_area.x2 = coords->x1 + bwidth;
            work_area.y1 = coords->y1 + (part & LV_BORDER_TOP ? bwidth + 1 : 0);
            work_area.y2 = coords->y2 - (part & LV_BORDER_BOTTOM ? bwidth + 1 : 0);
            fill_fp(&work_area, mask, color, opa);
        }

        /*Right bottom corner*/
        if(part & LV_BORDER_BOTTOM) {
            work_area.x1 = coords->x1;
            work_area.x2 = coords->x2;
            work_area.y1 = coords->y2 - bwidth;
            work_area.y2 = coords->y2;
            fill_fp(&work_area, mask, color, opa);
        }
        return;
    }

    /* Modify the corner_size if corner is drawn */
    corner_size ++;

    /*Depending one which part's are drawn modify the area lengths */
    if(part & LV_BORDER_TOP) work_area.y1 = coords->y1 + corner_size;
    else  work_area.y1 = coords->y1 + radius;

    if(part & LV_BORDER_BOTTOM) work_area.y2 = coords->y2 - corner_size;
    else work_area.y2 = coords->y2 - radius;

    /*Left border*/
    if(part & LV_BORDER_LEFT) {
        work_area.x1 = coords->x1;
        work_area.x2 = work_area.x1 + bwidth;
        fill_fp(&work_area, mask, color, opa);
    }

    /*Right border*/
    if(part & LV_BORDER_RIGHT) {
        work_area.x2 = coords->x2;
        work_area.x1 = work_area.x2 - bwidth;
        fill_fp(&work_area, mask, color, opa);
    }

    work_area.x1 = coords->x1 + corner_size - length_corr;
    work_area.x2 = coords->x2 - corner_size + length_corr;

    /*Upper border*/
    if(part & LV_BORDER_TOP) {
        work_area.y1 = coords->y1;
        work_area.y2 = coords->y1 + bwidth;
        fill_fp(&work_area, mask, color, opa);
    }

    /*Lower border*/
    if(part & LV_BORDER_BOTTOM) {
        work_area.y2 = coords->y2;
        work_area.y1 = work_area.y2 - bwidth;
        fill_fp(&work_area, mask, color, opa);
    }

    /*Draw the a remaining rectangles if the radius is smaller then bwidth */
    if(length_corr != 0) {
        /*Left top correction*/
        if((part & LV_BORDER_TOP) && (part & LV_BORDER_LEFT)) {
            work_area.x1 = coords->x1;
            work_area.x2 = coords->x1 + radius + LV_ANTIALIAS;
            work_area.y1 = coords->y1 + radius + 1 + LV_ANTIALIAS;
            work_area.y2 = coords->y1 + bwidth;
            fill_fp(&work_area, mask, color, opa);
        }

        /*Right top correction*/
        if((part & LV_BORDER_TOP) && (part & LV_BORDER_RIGHT)) {
            work_area.x1 = coords->x2 - radius - LV_ANTIALIAS;
            work_area.x2 = coords->x2;
            work_area.y1 = coords->y1 + radius + 1 + LV_ANTIALIAS;
            work_area.y2 = coords->y1 + bwidth;
            fill_fp(&work_area, mask, color, opa);
        }

        /*Left bottom correction*/
        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_LEFT)) {
            work_area.x1 = coords->x1;
            work_area.x2 = coords->x1 + radius + LV_ANTIALIAS;
            work_area.y1 = coords->y2 - bwidth;
            work_area.y2 = coords->y2 - radius - 1 - LV_ANTIALIAS;
            fill_fp(&work_area, mask, color, opa);
        }

        /*Right bottom correction*/
        if((part & LV_BORDER_BOTTOM) && (part & LV_BORDER_RIGHT)) {
            work_area.x1 = coords->x2 - radius - LV_ANTIALIAS;
            work_area.x2 = coords->x2;
            work_area.y1 = coords->y2 - bwidth;
            work_area.y2 = coords->y2 - radius - 1 - LV_ANTIALIAS;
            fill_fp(&work_area, mask, color, opa);
        }
    }

    /*If radius == 0 one px on the corners are not drawn by main drawer*/
    if(style->body.radius == 0) {
        /*Left top corner*/
        if(part & (LV_BORDER_TOP | LV_BORDER_LEFT)) {
            work_area.x1 = coords->x1;
            work_area.x2 = coords->x1 + LV_ANTIALIAS;
            work_area.y1 = coords->y1;
            work_area.y2 = coords->y1 + LV_ANTIALIAS;
            fill_fp(&work_area, mask, color, opa);
        }

        /*Right top corner*/
        if(part & (LV_BORDER_TOP | LV_BORDER_RIGHT)) {
            work_area.x1 = coords->x2 - LV_ANTIALIAS;
            work_area.x2 = coords->x2;
            work_area.y1 = coords->y1;
            work_area.y2 = coords->y1 + LV_ANTIALIAS;
            fill_fp(&work_area, mask, color, opa);
        }

        /*Left bottom corner*/
        if(part & (LV_BORDER_BOTTOM | LV_BORDER_LEFT)) {
            work_area.x1 = coords->x1;
            work_area.x2 = coords->x1 + LV_ANTIALIAS;
            work_area.y1 = coords->y2 - LV_ANTIALIAS;
            work_area.y2 = coords->y2;
            fill_fp(&work_area, mask, color, opa);
        }

        /*Right bottom corner*/
        if(part & (LV_BORDER_BOTTOM | LV_BORDER_RIGHT)) {
            work_area.x1 = coords->x2 - LV_ANTIALIAS;
            work_area.x2 = coords->x2;
            work_area.y1 = coords->y2 - LV_ANTIALIAS;
            work_area.y2 = coords->y2;
            fill_fp(&work_area, mask, color, opa);
        }
    }
}

/**
 * Draw a rectangle
 * @param coords the coordinates of the rectangle
 * @param mask the rectangle will be drawn only in this mask
 * @param style pointer to a style
 * @param opa_scale scale down all opacities by the factor
 */
void lv_draw_rect(const lv_area_t * coords, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale)
{
    if(lv_area_get_height(coords) < 1 || lv_area_get_width(coords) < 1) return;

#if USE_LV_SHADOW && LV_VDB_SIZE
    if(style->body.shadow.width != 0) {
        lv_draw_shadow(coords, mask, style, opa_scale);
    }
#endif
    if(style->body.empty == 0 && style->body.opa >= LV_OPA_MIN) {
        lv_draw_rect_main_mid(coords, mask, style, opa_scale);

        if(style->body.radius != 0) {
            lv_draw_rect_main_corner(coords, mask, style, opa_scale);
        }
    }

    if(style->body.border.width != 0 && style->body.border.part != LV_BORDER_NONE && style->body.border.opa >= LV_OPA_MIN) {
        lv_draw_rect_border_straight(coords, mask, style, opa_scale);

        if(style->body.radius != 0) {
//            lv_draw_rect_border_corner(coords, mask, style, opa_scale);
        }
    }
}

//-----画线--------------------------------------------------------------------------------------------------------------

/**
 * Add a horizontal anti-aliasing segment (pixels with decreasing opacity)
 * @param x start point x coordinate
 * @param y start point y coordinate
 * @param length length of segment (negative value to start from 0 opacity)
 * @param mask draw only in this area
 * @param color color of pixels
 * @param opa maximum opacity
 */
void lv_draw_aa_hor_seg(lv_coord_t x, lv_coord_t y, lv_coord_t length, const lv_area_t * mask, lv_color_t color, lv_opa_t opa)
{
    bool aa_inv = false;
    if(length < 0) {
        aa_inv = true;
        length = -length;
    }

    lv_coord_t i;
    for(i = 0; i < length; i++) {
        lv_opa_t px_opa = lv_draw_aa_get_opa(length, i, opa);
        if(aa_inv) px_opa = opa - px_opa;
        px_fp(x + i, y, mask, color, px_opa);
    }
}

/**
 * Add a vertical  anti-aliasing segment (pixels with decreasing opacity)
 * @param x start point x coordinate
 * @param y start point y coordinate
 * @param length length of segment (negative value to start from 0 opacity)
 * @param mask draw only in this area
 * @param color color of pixels
 * @param opa maximum opacity
 */
void lv_draw_aa_ver_seg(lv_coord_t x, lv_coord_t y, lv_coord_t length, const lv_area_t * mask, lv_color_t color, lv_opa_t opa)
{
    bool aa_inv = false;
    if(length < 0) {
        aa_inv = true;
        length = -length;
    }

    lv_coord_t i;
    for(i = 0; i < length; i++) {
        lv_opa_t px_opa = lv_draw_aa_get_opa(length, i, opa);
        if(aa_inv) px_opa = opa - px_opa;
        px_fp(x, y + i, mask, color, px_opa);
    }
}


/*********************
 *      DEFINES
 *********************/
#if LV_COMPILER_VLA_SUPPORTED == 0
#define LINE_MAX_WIDTH        64
#endif

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_point_t p1;
    lv_point_t p2;
    lv_point_t p_act;
    lv_coord_t dx;
    lv_coord_t sx;      /*-1: x1 < x2; 1: x2 >= x1*/
    lv_coord_t dy;
    lv_coord_t sy;      /*-1: y1 < y2; 1: y2 >= y1*/
    lv_coord_t err;
    lv_coord_t e2;
    bool hor;   /*Rather horizontal or vertical*/
} line_draw_t;

typedef struct {
    lv_coord_t width;
    lv_coord_t width_1;
    lv_coord_t width_half;
} line_width_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void line_draw_hor(line_draw_t * main_line, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale);
static void line_draw_ver(line_draw_t * main_line, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale);
static void line_draw_skew(line_draw_t * main_line, bool dir_ori, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale);
static void line_init(line_draw_t * line, const lv_point_t * p1, const lv_point_t * p2);
static bool line_next(line_draw_t * line);
static bool line_next_y(line_draw_t * line);
static bool line_next_x(line_draw_t * line);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Draw a line
 * @param point1 first point of the line
 * @param point2 second point of the line
 * @param mask the line will be drawn only on this area
 * @param style pointer to a line's style
 * @param opa_scale scale down all opacities by the factor
 */
void lv_draw_line(const lv_point_t * point1, const lv_point_t * point2, const lv_area_t * mask,
                  const lv_style_t * style, lv_opa_t opa_scale)
{

    if(style->line.width == 0) return;
    if(point1->x == point2->x && point1->y == point2->y) return;

    line_draw_t main_line;
    lv_point_t p1;
    lv_point_t p2;

    /*If the line if rather vertical then be sure y1 < y2 else x1 < x2*/

    if(LV_MATH_ABS(point1->x - point2->x) > LV_MATH_ABS(point1->y - point2->y)) {

        /*Steps less in y then x -> rather horizontal*/
        if(point1->x < point2->x) {
            p1.x = point1->x;
            p1.y = point1->y;
            p2.x = point2->x;
            p2.y = point2->y;
        } else {
            p1.x = point2->x;
            p1.y = point2->y;
            p2.x = point1->x;
            p2.y = point1->y;
        }
    } else {
        /*Steps less in x then y -> rather vertical*/
        if(point1->y < point2->y) {
            p1.x = point1->x;
            p1.y = point1->y;
            p2.x = point2->x;
            p2.y = point2->y;
        } else {
            p1.x = point2->x;
            p1.y = point2->y;
            p2.x = point1->x;
            p2.y = point1->y;
        }
    }

    line_init(&main_line, &p1, &p2);


    /*Special case draw a horizontal line*/
    if(main_line.p1.y == main_line.p2.y) {
        line_draw_hor(&main_line, mask, style, opa_scale);
    }
        /*Special case draw a vertical line*/
    else if(main_line.p1.x == main_line.p2.x) {
        line_draw_ver(&main_line, mask, style, opa_scale);
    }
        /*Arbitrary skew line*/
    else {
        bool dir_ori = false;
#if LV_ANTIALIAS
        lv_point_t p_tmp;

        if(main_line.hor) {
            if(main_line.p1.y < main_line.p2.y) {
                dir_ori = true;
                p_tmp.x = main_line.p2.x;
                p_tmp.y = main_line.p2.y - 1;
                line_init(&main_line, &p1, &p_tmp);
                main_line.sy = LV_MATH_ABS(main_line.sy);   /*The sign can change if the line becomes horizontal*/
            }
            else if(main_line.p1.y > main_line.p2.y) {
                dir_ori = false;
                p_tmp.x = main_line.p2.x;
                p_tmp.y = main_line.p2.y + 1;
                line_init(&main_line, &p1, &p_tmp);
                main_line.sy = -LV_MATH_ABS(main_line.sy);  /*The sign can change if the line becomes horizontal*/
            }
        }
        else {
            if(main_line.p1.x < main_line.p2.x) {
                dir_ori = true;
                p_tmp.x = main_line.p2.x - 1;
                p_tmp.y = main_line.p2.y;
                line_init(&main_line, &p1, &p_tmp);
                main_line.sx = LV_MATH_ABS(main_line.sx);   /*The sign can change if the line becomes vertical*/
            }
            else if(main_line.p1.x > main_line.p2.x) {
                dir_ori = false;
                p_tmp.x = main_line.p2.x + 1;
                p_tmp.y = main_line.p2.y;
                line_init(&main_line, &p1, &p_tmp);
                main_line.sx = -LV_MATH_ABS(main_line.sx);  /*The sign can change if the line becomes vertical*/
            }
        }
#endif
        line_draw_skew(&main_line, dir_ori, mask, style, opa_scale);
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/


static void line_draw_hor(line_draw_t * main_line, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale)
{
    lv_coord_t width = style->line.width - 1;
    lv_coord_t width_half = width >> 1;
    lv_coord_t width_1 = width & 0x1;
    lv_opa_t opa = opa_scale == LV_OPA_COVER ? style->line.opa : (uint16_t)((uint16_t) style->line.opa * opa_scale) >> 8;

    lv_area_t act_area;
    act_area.x1 = main_line->p1.x;
    act_area.x2 = main_line->p2.x;
    act_area.y1 = main_line->p1.y - width_half - width_1;
    act_area.y2 = main_line->p2.y + width_half ;

    lv_area_t draw_area;
    draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
    draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
    draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
    draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
    fill_fp(&draw_area, mask, style->line.color, opa);
}

static void line_draw_ver(line_draw_t * main_line, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale)
{
    lv_coord_t width = style->line.width - 1;
    lv_coord_t width_half = width >> 1;
    lv_coord_t width_1 = width & 0x1;
    lv_opa_t opa = opa_scale == LV_OPA_COVER ? style->line.opa : (uint16_t)((uint16_t) style->line.opa * opa_scale) >> 8;


    lv_area_t act_area;
    act_area.x1 = main_line->p1.x - width_half;
    act_area.x2 = main_line->p2.x + width_half + width_1;
    act_area.y1 = main_line->p1.y;
    act_area.y2 = main_line->p2.y;

    lv_area_t draw_area;
    draw_area.x1 = LV_MATH_MIN(act_area.x1, act_area.x2);
    draw_area.x2 = LV_MATH_MAX(act_area.x1, act_area.x2);
    draw_area.y1 = LV_MATH_MIN(act_area.y1, act_area.y2);
    draw_area.y2 = LV_MATH_MAX(act_area.y1, act_area.y2);
    fill_fp(&draw_area, mask, style->line.color, opa);
}

static void line_draw_skew(line_draw_t * main_line, bool dir_ori, const lv_area_t * mask, const lv_style_t * style, lv_opa_t opa_scale)
{

    lv_opa_t opa = opa_scale == LV_OPA_COVER ? style->line.opa : (uint16_t)((uint16_t) style->line.opa * opa_scale) >> 8;

    lv_point_t vect_main, vect_norm;
    vect_main.x = main_line->p2.x - main_line->p1.x;
    vect_main.y = main_line->p2.y - main_line->p1.y;

    if(main_line->hor) {
        if(main_line->p1.y < main_line->p2.y + dir_ori) {
            vect_norm.x = - vect_main.y;
            vect_norm.y = vect_main.x;
        } else {
            vect_norm.x = vect_main.y;
            vect_norm.y = -vect_main.x;
        }
    } else {
        if(main_line->p1.x < main_line->p2.x + dir_ori) {
            vect_norm.x = vect_main.y;
            vect_norm.y = - vect_main.x;
        } else {
            vect_norm.x = - vect_main.y;
            vect_norm.y = vect_main.x;
        }
    }

    /* In case of a short but tick line the perpendicular ending is longer then the real line.
     * it would break the calculations so make the normal vector larger*/
    vect_norm.x = vect_norm.x << 4;
    vect_norm.y = vect_norm.y << 4;

    lv_coord_t width;
    width = style->line.width;

    /* The pattern stores the points of the line ending. It has the good direction and length.
     * The worth case is the 45° line where pattern can have 1.41 x `width` points*/
#if LV_COMPILER_VLA_SUPPORTED
    lv_point_t pattern[width * 2];
#else
    lv_point_t pattern[LINE_MAX_WIDTH];
#endif
    lv_coord_t i = 0;

    /*Create a perpendicular pattern (a small line)*/
    if(width != 0) {
        line_draw_t pattern_line;
        lv_point_t p0 = {0, 0};
        line_init(&pattern_line, &p0, &vect_norm);

        uint32_t width_sqr = width * width;
        /* Run for a lot of times. Meanwhile the real width will be determined as well */
        for(i = 0; i < (lv_coord_t)sizeof(pattern); i ++) {
            pattern[i].x = pattern_line.p_act.x;
            pattern[i].y = pattern_line.p_act.y;

            /*Finish the pattern line if it's length equal to the desired width (Use Pythagoras theorem)*/
            uint32_t  sqr = pattern_line.p_act.x * pattern_line.p_act.x + pattern_line.p_act.y * pattern_line.p_act.y;
            if(sqr >= width_sqr) {
                width = i;
#if LV_ANTIALIAS
                width--;
#endif
                break;
            }

            line_next(&pattern_line);
        }
    }

#if LV_ANTIALIAS
    lv_coord_t width_safe = width;
    if(width == 0) width_safe = 1;

    lv_coord_t aa_last_corner;
    aa_last_corner = 0;
#endif

    lv_coord_t x_center_ofs = 0;
    lv_coord_t y_center_ofs = 0;

    if(width != 0) {
        x_center_ofs = pattern[width - 1].x / 2;
        y_center_ofs = pattern[width - 1].y / 2;
    }
    else {
        if(main_line->hor && main_line->p1.y >= main_line->p2.y + dir_ori) pattern[0].y --;
        if(!main_line->hor && main_line->p1.x >= main_line->p2.x + dir_ori) pattern[0].x --;
    }

    /* Make the coordinates relative to the center */
    for(i = 0; i < width; i++) {
        pattern[i].x -= x_center_ofs;
        pattern[i].y -= y_center_ofs;
#if LV_ANTIALIAS
        if(i != 0) {
            if(main_line->hor) {
                if(pattern[i - 1].x != pattern[i].x) {
                    lv_coord_t seg_w = pattern[i].y - pattern[aa_last_corner].y;
                    if(main_line->sy < 0) {
                        lv_draw_aa_ver_seg(main_line->p1.x + pattern[aa_last_corner].x - 1, main_line->p1.y + pattern[aa_last_corner].y + seg_w + 1,
                                           seg_w, mask, style->line.color, opa);

                        lv_draw_aa_ver_seg(main_line->p2.x + pattern[aa_last_corner].x + 1, main_line->p2.y + pattern[aa_last_corner].y + seg_w + 1,
                                           -seg_w, mask, style->line.color, opa);
                    } else {
                        lv_draw_aa_ver_seg(main_line->p1.x + pattern[aa_last_corner].x - 1, main_line->p1.y + pattern[aa_last_corner].y,
                                           seg_w, mask, style->line.color, opa);

                        lv_draw_aa_ver_seg(main_line->p2.x + pattern[aa_last_corner].x + 1, main_line->p2.y + pattern[aa_last_corner].y,
                                           -seg_w, mask, style->line.color, opa);
                    }
                    aa_last_corner = i;
                }
            } else {
                if(pattern[i - 1].y != pattern[i].y) {
                    lv_coord_t seg_w = pattern[i].x - pattern[aa_last_corner].x;
                    if(main_line->sx < 0) {
                        lv_draw_aa_hor_seg(main_line->p1.x + pattern[aa_last_corner].x + seg_w + 1, main_line->p1.y + pattern[aa_last_corner].y - 1,
                                           seg_w, mask, style->line.color, opa);

                        lv_draw_aa_hor_seg(main_line->p2.x + pattern[aa_last_corner].x + seg_w + 1, main_line->p2.y + pattern[aa_last_corner].y + 1,
                                           -seg_w, mask, style->line.color, opa);
                    } else {
                        lv_draw_aa_hor_seg(main_line->p1.x + pattern[aa_last_corner].x, main_line->p1.y + pattern[aa_last_corner].y - 1,
                                           seg_w, mask, style->line.color, opa);

                        lv_draw_aa_hor_seg(main_line->p2.x + pattern[aa_last_corner].x, main_line->p2.y + pattern[aa_last_corner].y + 1,
                                           -seg_w, mask, style->line.color, opa);
                    }
                    aa_last_corner = i;
                }
            }

        }
#endif
    }



#if LV_ANTIALIAS
    /*Add the last part of anti-aliasing for the perpendicular ending*/
    if(width != 0) {    /*Due to rounding error with very thin lines it looks ugly*/
        if(main_line->hor) {
            lv_coord_t seg_w = pattern[width_safe - 1].y - pattern[aa_last_corner].y;
            if(main_line->sy < 0) {
                lv_draw_aa_ver_seg(main_line->p1.x + pattern[aa_last_corner].x - 1, main_line->p1.y + pattern[aa_last_corner].y + seg_w,
                                   seg_w + main_line->sy, mask, style->line.color, opa);

                lv_draw_aa_ver_seg(main_line->p2.x + pattern[aa_last_corner].x + 1, main_line->p2.y + pattern[aa_last_corner].y + seg_w,
                                   -(seg_w + main_line->sy), mask, style->line.color, opa);

            } else {
                lv_draw_aa_ver_seg(main_line->p1.x + pattern[aa_last_corner].x - 1, main_line->p1.y + pattern[aa_last_corner].y,
                                   seg_w + main_line->sy, mask, style->line.color, opa);

                lv_draw_aa_ver_seg(main_line->p2.x + pattern[aa_last_corner].x + 1, main_line->p2.y + pattern[aa_last_corner].y,
                                   -(seg_w + main_line->sy), mask, style->line.color, opa);
            }
        } else {
            lv_coord_t seg_w = pattern[width_safe - 1].x - pattern[aa_last_corner].x;
            if(main_line->sx < 0) {
                lv_draw_aa_hor_seg(main_line->p1.x + pattern[aa_last_corner].x + seg_w, main_line->p1.y + pattern[aa_last_corner].y - 1,
                                   seg_w + main_line->sx, mask, style->line.color, opa);

                lv_draw_aa_hor_seg(main_line->p2.x + pattern[aa_last_corner].x + seg_w, main_line->p2.y + pattern[aa_last_corner].y + 1,
                                   -(seg_w + main_line->sx), mask, style->line.color, opa);

            } else {
                lv_draw_aa_hor_seg(main_line->p1.x + pattern[aa_last_corner].x, main_line->p1.y + pattern[aa_last_corner].y - 1,
                                   seg_w + main_line->sx, mask, style->line.color, opa);

                lv_draw_aa_hor_seg(main_line->p2.x + pattern[aa_last_corner].x, main_line->p2.y + pattern[aa_last_corner].y + 1,
                                   -(seg_w + main_line->sx), mask, style->line.color, opa);
            }

        }
    }
#endif

#if LV_ANTIALIAS

    /*Shift the anti aliasing on the edges (-1, 1 or 0 (zero only in case width == 0))*/
    lv_coord_t aa_shift1;
    lv_coord_t aa_shift2;

    if(main_line->hor == false) {
        if(main_line->sx < 0) {
            aa_shift1 = -1;
            aa_shift2 = width == 0 ? 0 : aa_shift1;
        } else {
            aa_shift2 = 1;
            aa_shift1 = width == 0 ? 0 : aa_shift2;
        }
    } else {
        if(main_line->sy < 0) {
            aa_shift1 = -1;
            aa_shift2 = width == 0 ? 0 : aa_shift1;
        } else {
            aa_shift2 = 1;
            aa_shift1 = width == 0 ? 0 : aa_shift2;
        }
    }

#endif

    volatile lv_point_t prev_p;
    prev_p.x = main_line->p1.x;
    prev_p.y = main_line->p1.y;
    lv_area_t draw_area;
    bool first_run = true;

    if(main_line->hor) {
        while(line_next_y(main_line)) {
            for(i = 0; i < width; i++) {
                draw_area.x1 = prev_p.x + pattern[i].x;
                draw_area.y1 = prev_p.y + pattern[i].y;
                draw_area.x2 = draw_area.x1 + main_line->p_act.x - prev_p.x - 1;
                draw_area.y2 = draw_area.y1;
                fill_fp(&draw_area, mask, style->line.color, opa);

                /* Fill the gaps
                 * When stepping in y one pixel remains empty on every corner (don't do this on the first segment ) */
                if(i != 0 && pattern[i].x != pattern[i - 1].x && !first_run) {
                    px_fp(draw_area.x1, draw_area.y1 - main_line->sy, mask, style->line.color, opa);
                }
            }

#if LV_ANTIALIAS
            lv_draw_aa_hor_seg(prev_p.x + pattern[0].x, prev_p.y + pattern[0].y - aa_shift1,
                               -(main_line->p_act.x - prev_p.x), mask, style->line.color, opa);
            lv_draw_aa_hor_seg(prev_p.x + pattern[width_safe - 1].x, prev_p.y + pattern[width_safe - 1].y + aa_shift2,
                               main_line->p_act.x - prev_p.x, mask, style->line.color, opa);
#endif

            first_run = false;

            prev_p.x = main_line->p_act.x;
            prev_p.y = main_line->p_act.y;
        }

        for(i = 0; i < width; i++) {
            draw_area.x1 = prev_p.x + pattern[i].x;
            draw_area.y1 = prev_p.y + pattern[i].y;
            draw_area.x2 = draw_area.x1 + main_line->p_act.x - prev_p.x;
            draw_area.y2 = draw_area.y1;
            fill_fp(&draw_area, mask, style->line.color, opa);

            /* Fill the gaps
             * When stepping in y one pixel remains empty on every corner */
            if(i != 0 && pattern[i].x != pattern[i - 1].x && !first_run) {
                px_fp(draw_area.x1, draw_area.y1 - main_line->sy, mask, style->line.color, opa);
            }
        }

#if LV_ANTIALIAS
        lv_draw_aa_hor_seg(prev_p.x + pattern[0].x, prev_p.y + pattern[0].y - aa_shift1,
                           -(main_line->p_act.x - prev_p.x + 1), mask, style->line.color, opa);
        lv_draw_aa_hor_seg(prev_p.x + pattern[width_safe - 1].x, prev_p.y + pattern[width_safe - 1].y + aa_shift2,
                           main_line->p_act.x - prev_p.x + 1, mask, style->line.color, opa);
#endif
    }
        /*Rather a vertical line*/
    else {

        while(line_next_x(main_line)) {
            for(i = 0; i < width; i++) {
                draw_area.x1 = prev_p.x + pattern[i].x;
                draw_area.y1 = prev_p.y + pattern[i].y;
                draw_area.x2 = draw_area.x1;
                draw_area.y2 = draw_area.y1 + main_line->p_act.y - prev_p.y - 1;

                fill_fp(&draw_area, mask, style->line.color, opa);

                /* Fill the gaps
                 * When stepping in x one pixel remains empty on every corner (don't do this on the first segment ) */
                if(i != 0 && pattern[i].y != pattern[i - 1].y && !first_run) {
                    px_fp(draw_area.x1 - main_line->sx, draw_area.y1, mask, style->line.color, opa);
                }

            }

#if LV_ANTIALIAS
            lv_draw_aa_ver_seg(prev_p.x + pattern[0].x - aa_shift1, prev_p.y + pattern[0].y,
                               -(main_line->p_act.y - prev_p.y), mask, style->line.color, opa);
            lv_draw_aa_ver_seg(prev_p.x + pattern[width_safe - 1].x + aa_shift2, prev_p.y + pattern[width_safe - 1].y,
                               main_line->p_act.y - prev_p.y, mask, style->line.color, opa);
#endif

            first_run = false;

            prev_p.x = main_line->p_act.x;
            prev_p.y = main_line->p_act.y;
        }

        /*Draw the last part*/
        for(i = 0; i < width; i++) {
            draw_area.x1 = prev_p.x + pattern[i].x;
            draw_area.y1 = prev_p.y + pattern[i].y;
            draw_area.x2 = draw_area.x1;
            draw_area.y2 = draw_area.y1 + main_line->p_act.y - prev_p.y;

            fill_fp(&draw_area, mask, style->line.color, opa);

            /* Fill the gaps
             * When stepping in x one pixel remains empty on every corner */
            if(i != 0 && pattern[i].y != pattern[i - 1].y && !first_run) {
                px_fp(draw_area.x1 - main_line->sx, draw_area.y1, mask, style->line.color, opa);
            }
        }

#if LV_ANTIALIAS
        lv_draw_aa_ver_seg(prev_p.x + pattern[0].x - aa_shift1, prev_p.y + pattern[0].y,
                           -(main_line->p_act.y - prev_p.y + 1), mask, style->line.color, opa);
        lv_draw_aa_ver_seg(prev_p.x + pattern[width_safe - 1].x + aa_shift2, prev_p.y + pattern[width_safe - 1].y,
                           main_line->p_act.y - prev_p.y + 1, mask, style->line.color, opa);
#endif
    }
}


static void line_init(line_draw_t * line, const lv_point_t * p1, const lv_point_t * p2)
{
    line->p1.x = p1->x;
    line->p1.y = p1->y;
    line->p2.x = p2->x;
    line->p2.y = p2->y;

    line->dx = LV_MATH_ABS(line->p2.x - line->p1.x);
    line->sx = line->p1.x < line->p2.x ? 1 : -1;
    line->dy = LV_MATH_ABS(line->p2.y - line->p1.y);
    line->sy = line->p1.y < line->p2.y ? 1 : -1;
    line->err = (line->dx > line->dy ? line->dx : -line->dy) / 2;
    line->e2 = 0;
    line->hor = line->dx > line->dy ? true : false; /*Rather horizontal or vertical*/

    line->p_act.x = line->p1.x;
    line->p_act.y = line->p1.y;
}

static bool line_next(line_draw_t * line)
{
    if(line->p_act.x == line->p2.x && line->p_act.y == line->p2.y) return false;
    line->e2 = line->err;
    if(line->e2 > -line->dx) {
        line->err -= line->dy;
        line->p_act.x += line->sx;
    }
    if(line->e2 < line->dy) {
        line->err += line->dx;
        line->p_act.y += line->sy;
    }
    return true;
}

/**
 * Iterate until step one in y direction.
 * @param line
 * @return
 */
static bool line_next_y(line_draw_t * line)
{
    lv_coord_t last_y = line->p_act.y;

    do {
        if(!line_next(line)) return false;
    } while(last_y == line->p_act.y);

    return true;

}

/**
 * Iterate until step one in x direction.
 * @param line
 * @return
 */
static bool line_next_x(line_draw_t * line)
{
    lv_coord_t last_x = line->p_act.x;

    do {
        if(!line_next(line)) return false;
    } while(last_x == line->p_act.x);

    return true;

}
