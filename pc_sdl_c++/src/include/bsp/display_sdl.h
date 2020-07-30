//
// Created by a6735 on 2020/7/29.
//

#ifndef SDL_DEMO_DISPLAY_SDL_H
#define SDL_DEMO_DISPLAY_SDL_H

#include "SDL2/SDL.h"
#include "user/config.h"

class display_sdl {
public:
	display_sdl();
	void up_data();
	void draw_point( uint16_t x, uint16_t y, uint16_t color);

private:
	SDL_Renderer *renderer;
	SDL_Texture *texture;

	unsigned short screen[LCD_WIDTH * LCD_LENGTH];
	unsigned scroll_pos;
	unsigned scroll_top;
	unsigned scroll_bottom;

	void update_texture( );
	void render_texture( );
};


#endif //SDL_DEMO_DISPLAY_SDL_H
