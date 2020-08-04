//
// Created by a6735 on 2020/8/2.
//

#include "lzui/lz_draw.h"


typedef struct _lcache {
	uint16_t array[CACHE_SIZE];
	lui_layout_t coordinate;
} lui_cache;

static void (*updata)(int x, int y, int width, int length, uint16_t * color) = NULL;
static lui_cache cache;
lui_layout_t f_layout;

static void l_point(int s_x, int s_y ,int m_x, int m_y, uint16_t color);
static void lui_draw_point_clor5658(int s_x, int s_y ,int m_x, int m_y, lui_color5658_t color);

void lui_draw_set_updata( void (*up)(int x, int y, int width, int length, uint16_t * color) ) {
	updata = up;
}

void lui_draw_frame(int x, int y, int width, int length, lui_color5658_t color) {
	if(color.alpha < LUI_COLOR_ALPHA_FALL) {
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
				for(int y_i = y1*cache.coordinate.size.width; y_i < y2*cache.coordinate.size.width; y_i += cache.coordinate.size.width) {
					for(int x_j = x1; x_j < x2; x_j++) {
						if(color.alpha == LUI_COLOR_ALPHA_NULL) {
							cache.array[y_i+x_j] = color.color.rgb565;
						} else {
							cache.array[y_i+x_j] = lui_color_alpha_blend(cache.array[y_i+x_j],color.color.rgb565,color.alpha);
						}
					}
				}
			}
		}
	}
}

void lui_draw_line(int s_x, int s_y, int e_x, int e_y, uint16_t color) {
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

void lui_draw_empty_frame( int x1, int y1, int x2, int y2, uint16_t color ) {
	lui_draw_line(x1,y1,x2,y1,color);
	lui_draw_line(x1,y2,x2,y2,color);
	lui_draw_line(x1,y1,x1,y2,color);
	lui_draw_line(x2,y1,x2,y2,color);
}

void lui_draw_arc( int x0, int y0, int r, int s, uint16_t color ) {
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

void lui_draw_circle( int x0, int y0, int r, uint16_t c ) {
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

void lui_draw_fill_circle( int x0, int y0, int r, uint16_t c ) {
	int x,y,xd;
	if ( x0<0 ) return;
	if ( y0<0 ) return;
	if ( r<=0 ) return;
	xd = 3 - (r << 1);
	x = 0;
	y = r;
	while ( x <= y ) {
		if( y > 0 ) {
			lui_draw_line(x0 - x, y0 - y,x0 - x, y0 + y, c);
			lui_draw_line(x0 + x, y0 - y,x0 + x, y0 + y, c);
		}
		if( x > 0 ) {
			lui_draw_line(x0 - y, y0 - x,x0 - y, y0 + x, c);
			lui_draw_line(x0 + y, y0 - x,x0 + y, y0 + x, c);
		}
		if ( xd < 0 ) {
			xd += (x << 2) + 6;
		} else {
			xd += ((x - y) << 2) + 10;
			y--;
		}
		x++;
	}
	lui_draw_circle(x0, y0, r,c);
}

void lui_draw_round_frame( int x1, int y1, int x2, int y2, int r, uint16_t c ) {
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
	lui_draw_line(x1+r, y1, x2-r, y1, c);
	lui_draw_line(x1+r, y2, x2-r, y2, c);
	lui_draw_line(x1, y1+r, x1, y2-r, c);
	lui_draw_line(x2, y1+r, x2, y2-r, c);
	lui_draw_arc(x1+r, y1+r, r, 0x0C, c);
	lui_draw_arc(x2-r, y1+r, r, 0x03, c);
	lui_draw_arc(x1+r, y2-r, r, 0x30, c);
	lui_draw_arc(x2-r, y2-r, r, 0xC0, c);
}

void lui_draw_fill_round_frame( int x1, int y1, int x2, int y2, int r, lui_color5658_t color ) {
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
	lui_draw_frame(x1 + r, y1, x2 - r - x1 - r, (y2-y1), color);
	while ( x <= y ) {
		if( y > 0 ) {
			lui_draw_line(x2 + x - r, y1 - y + r, x2+ x - r, y + y2 - r, color.color.rgb565);
			lui_draw_line(x1 - x + r, y1 - y + r, x1- x + r, y + y2 - r, color.color.rgb565);
		}
		if( x > 0 ) {
			lui_draw_line(x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r, color.color.rgb565);
			lui_draw_line(x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r, color.color.rgb565);
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

void lui_draw_mesh( int x1, int y1, int x2, int y2, lui_color5658_t color ) {
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

void lui_draw_font(int x, int y, uint8_t wighth, uint8_t length, uint16_t color, uint8_t * mate) {
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
//							cache.array[y_i+x_j] = lui_color_alpha_blend(color,cache.array[y_i+x_j],mate[(ptr)]);
                            color_rel = lui_color_alpha_blend(color,cache.array[y_i+x_j],mate[(ptr)]);
						}
                        cache.array[y_i+x_j] = lui_color_alpha_blend(color_rel,cache.array[y_i+x_j],255);
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

void lui_draw_jpg(int x, int y, int width, int length, uint8_t * material) {
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

void lui_draw_png(int x, int y, int width, int length, const unsigned char * material) {
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
						cache.array[y_i+x_j] = lui_color_alpha_blend(cache.array[y_i+x_j],color, 0xff-material[ptr+2]);
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

void lui_draw_screen(int x, int y, int width, int length, uint32_t * material) {
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

static void lui_draw_point_clor5658(int s_x, int s_y ,int m_x, int m_y, lui_color5658_t color) {
	if(color.alpha < LUI_COLOR_ALPHA_FALL) {
		if( ( s_x  >= cache.coordinate.point.x && s_x  < m_x ) &&
		    (  s_y >= cache.coordinate.point.y && s_y < m_y ) ) {
			uint32_t pos = (s_y-cache.coordinate.point.y)*cache.coordinate.size.width+(s_x-cache.coordinate.point.x);
			if(color.alpha == LUI_COLOR_ALPHA_NULL) {
				cache.array[pos] = color.color.rgb565;
			} else {
				cache.array[pos] = lui_color_alpha_blend(cache.array[pos],color.color.rgb565,color.alpha);
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

void lui_drawcache_size_set(int x, int y, int width, int length) {
	for(int i = 0; i < CACHE_SIZE; i++) {
		cache.array[i] = 0xffff;
	}
	cache.coordinate.point.x = x;
	cache.coordinate.point.y = y;
	cache.coordinate.size.width = width;
	cache.coordinate.size.length = length;
}

lui_layout_t * lui_draw_cache_size(void) {
	return &cache.coordinate;
}

void lui_draw_cache_to_lcd(int x, int y, int width, int length) {
	if(updata != NULL) {
		updata(x,y,width,length,cache.array);
	}
}