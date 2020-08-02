
#include <stdio.h>
#include "SDL2/SDL.h"
#include "bsp/display_sdl.h"
#include "../../include/SDL2/SDL_events.h"

#include "lzui/lz_draw.h"
#include "lzui/lz_obj_bar.h"
#include "lzui/lz_obj_button.h"
#include "lzui/lz_obj_lable.h"

void lui_loop(void);

static uint8_t running = 1;
uint8_t touch_flag = 0;

int main(int argc, char** argv) {
	display_sdl();
	display_sdl_up_data();

	lui_draw_set_updata(display_sdl_fill);

	lui_obj_t * bar = lui_create_bar(10,150);
	lui_obj_add_child(lui_get_root(),bar);

	lui_obj_t * but = lui_create_button(10,10);
	lui_obj_add_child(lui_get_root(),but);

	lui_obj_t * lable = lui_create_lable(100,10);
	lui_obj_add_child(lui_get_root(),lable);

	lui_loop();

	SDL_Event event;
	while(running) {
		if ( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
				case SDL_QUIT: {
					running = 0;
				} break;
				case SDL_MOUSEMOTION: {
					if(touch_flag == 1) {
						lui_obj_even(event.button.x, event.button.y, touch_flag);
					}
				} break;
				case SDL_MOUSEWHEEL: {

				} break;
				case SDL_MOUSEBUTTONDOWN: {
					if( event.button.button == SDL_BUTTON_LEFT ){
						touch_flag = 1;
						lui_obj_even(event.button.x, event.button.y, 2);
					}
				} break;
				case SDL_MOUSEBUTTONUP: {
					if( event.button.button == SDL_BUTTON_LEFT ){
						touch_flag = 0;
						lui_obj_even(event.button.x, event.button.y, touch_flag);
					}
				} break;
			}
		}
		lui_loop();
		display_sdl_up_data();
		SDL_Delay(10);
	}
	SDL_Quit();
	return 0;
}


void lui_loop(void) {
	for(int i = 0; i < LCD_LENGTH; i += CACHE_LENGTH) {
		for(int j = 0; j < LCD_WIDTH; j += CACHE_WIDTH) {
			lui_drawcache_size_set(j,i,CACHE_WIDTH,CACHE_LENGTH);
			lui_obj_traverse(lui_get_root());
			lui_draw_cache_to_lcd(j,i,CACHE_WIDTH,CACHE_LENGTH);
		}
	}
}



