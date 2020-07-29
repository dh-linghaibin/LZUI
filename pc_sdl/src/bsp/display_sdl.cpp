//
// Created by a6735 on 2020/7/29.
//

#include "bsp/display_sdl.h"
#include "SDL2/SDL.h"
#include "user/config.h"
#include <iostream>

using namespace std;

static SDL_Renderer *renderer;
static SDL_Texture *texture;

static unsigned short screen[LCD_WIDTH * LCD_LENGTH];
static unsigned scroll_pos;
static unsigned scroll_top;
static unsigned scroll_bottom;

static void update_texture(void)
{
	SDL_UpdateTexture(texture, NULL, screen, LCD_WIDTH * sizeof(screen[0]));
}

static void render_texture(void)
{
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

display_sdl :: display_sdl( ) {
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		cout << "Something went wrong! " << SDL_GetError() << endl;
	}

	SDL_Window* window = SDL_CreateWindow("SDL_Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                      320, 240, SDL_WINDOW_OPENGL);
	if(window == nullptr){
		cout << "Something also went wrong here" << endl;
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
	update_texture();
	render_texture();
}
