//
// Created by a6735 on 2020/7/29.
//

#ifndef SDL_DEMO_DISPLAY_SDL_H
#define SDL_DEMO_DISPLAY_SDL_H

#include "SDL2/SDL.h"
#include "user/config.h"

void display_sdl( void );
void display_sdl_up_data( void );
void display_sdl_draw_point( uint16_t x, uint16_t y, uint16_t color);

#endif //SDL_DEMO_DISPLAY_SDL_H
