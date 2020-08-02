//
// Created by a6735 on 2020/7/29.
//

#include "bsp/display_sdl.h"

SDL_Renderer *renderer;
SDL_Texture *texture;

unsigned short screen[LCD_WIDTH * LCD_LENGTH];
unsigned scroll_pos;
unsigned scroll_top;
unsigned scroll_bottom;

void display_sdl_update_texture( );
void display_sdl_render_texture( );

void display_sdl( void ) {
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){

	}

	SDL_Window* window = SDL_CreateWindow("LZUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                      320, 240, SDL_WINDOW_OPENGL);
	if(window == NULL){

	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, LCD_WIDTH, LCD_LENGTH);

	texture = SDL_CreateTexture(renderer,
	                            SDL_PIXELFORMAT_RGB565,
	                            SDL_TEXTUREACCESS_STREAMING,
	                            LCD_WIDTH, LCD_LENGTH);
	if (!window || !renderer || !texture)
		exit(2);

	scroll_pos = scroll_top = scroll_bottom = 0;

	memset(screen, 0x00, sizeof(screen));
	display_sdl_up_data();
}

void display_sdl_fill(int x, int y, int width, int length, uint16_t * color) {
	for(int j = y; j < y+length; j++) {
		for(int i = x; i < x+width; i++) {
			screen[j*LCD_WIDTH+i] = *color++;
		}
	}
}

void display_sdl_up_data() {
	display_sdl_update_texture();
	display_sdl_render_texture();
}

void display_sdl_draw_point( uint16_t x, uint16_t y, uint16_t color) {
	if(x > 320) {
		return;
	}
	if(y > 240) {
		return;
	}
	screen[y*LCD_WIDTH+x] = color;
}

void display_sdl_update_texture( ) {
	SDL_UpdateTexture(texture, NULL, screen, LCD_WIDTH * sizeof(screen[0]));
}

void display_sdl_render_texture( ) {
	SDL_Rect src, dst;
	unsigned w = LCD_WIDTH - (scroll_bottom + scroll_top);

	SDL_RenderClear(renderer);

	/* bottom */
	if (scroll_bottom) {
		src.x = 0;
		src.y = 0;
		src.w = scroll_bottom;
		src.h = LCD_LENGTH;

		SDL_RenderCopy(renderer, texture, &src, &src);
	}

	/* scrolling area */
	src.x = scroll_bottom + scroll_pos;
	src.y = 0;
	src.w = w - scroll_pos;
	src.h = LCD_LENGTH;
	dst.x = scroll_bottom;
	dst.y = 0;
	dst.w = w - scroll_pos;
	dst.h = LCD_LENGTH;

	SDL_RenderCopy(renderer, texture, &src, &dst);

	if (scroll_pos) {
		src.x = scroll_bottom;
		src.y = 0;
		src.w = scroll_pos;
		src.h = LCD_LENGTH;
		dst.x = scroll_bottom + w - scroll_pos;
		dst.y = 0;
		dst.w = scroll_pos;
		dst.h = LCD_LENGTH;
		SDL_RenderCopy(renderer, texture, &src, &dst);
	}

	/* top */
	if (scroll_top) {
		src.x = LCD_WIDTH - scroll_top;
		src.y = 0;
		src.w = scroll_top;
		src.h = LCD_LENGTH;

		SDL_RenderCopy(renderer, texture, &src, &src);
	}

	SDL_RenderPresent(renderer);
}



