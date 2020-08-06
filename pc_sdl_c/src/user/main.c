
#include <stdio.h>
#include <lzui/lz_tick.h>
#include "SDL2/SDL.h"
#include "bsp/display_sdl.h"
#include "../../include/SDL2/SDL_events.h"

#include "lzui/lz_draw.h"
#include "lzui/lz_obj_bar.h"
#include "lzui/lz_obj_button.h"
#include "lzui/lz_obj_lable.h"
#include "lzui/lz_obj_png.h"
#include "lzui/lz_obj_chart.h"

void lui_loop(void);

static uint8_t running = 1;
uint8_t touch_flag = 0;
extern const struct png_t usb_in_png_v;

static int tick_thread(void *data) {
    while(running) {
        SDL_Delay(1);
        lz_tick_inc(1);
        lz_tick_loop();
    }
    return 0;
}

void bt_event(struct _lz_tick_t * tick) {
    static uint16_t x = 0;
    static uint16_t y = 0;
//    lz_button_t * but = tick->obj->val;
    lz_obj_set_x(tick->obj, x);
    lz_obj_set_y(tick->obj, y);
    if(x < 320) {
        x += 2;
    } else {
        x = 0;
    }

    if(y < 240) {
        y += 2;
    } else {
        y = 0;
    }
}

extern  pos_t pos_val;

int main(int argc, char** argv) {
	display_sdl();
	display_sdl_up_data();

	lz_draw_set_updata(display_sdl_fill);

//	lz_obj_t * bar = lz_create_bar(10,150);
//	lz_obj_add_child(lz_get_root(),bar);

	lz_obj_t * lable = lz_create_lable(100,10);
	lz_obj_add_child(lz_get_root(),lable);

    lz_obj_t * pngxx = lz_create_png(10,10);
    lz_icon_set_val(pngxx, usb_in_png_v);
    lz_obj_add_child(lz_get_root(),pngxx);

    lz_obj_t * but = lz_create_button(10,10);
    lz_obj_add_child(lz_get_root(),but);

    lz_obj_t * chart = lz_create_chart(50,100);
    lz_obj_add_child(lz_get_root(),chart);

//    lz_tick_create(but, bt_event, 50);

    SDL_CreateThread(tick_thread, "tick", NULL);

	SDL_Event event;
	while(running) {
		if ( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
				case SDL_QUIT: {
					running = 0;
				} break;
				case SDL_MOUSEMOTION: {
                    pos_val.mouse_x = event.button.x;
                    pos_val.mouse_y = event.button.y;
					if(touch_flag == 1) {
						lz_obj_even(event.button.x, event.button.y, touch_flag);
					}
				} break;
				case SDL_MOUSEWHEEL: {

				} break;
				case SDL_MOUSEBUTTONDOWN: {
                    pos_val.is_mouse_down = 1;
					if( event.button.button == SDL_BUTTON_LEFT ){
						touch_flag = 1;
						lz_obj_even(event.button.x, event.button.y, 2);
					}
				} break;
				case SDL_MOUSEBUTTONUP: {
                    pos_val.is_mouse_down = 0;
					if( event.button.button == SDL_BUTTON_LEFT ){
						touch_flag = 0;
						lz_obj_even(event.button.x, event.button.y, touch_flag);
					}
				} break;
			}
		}
		lui_loop();
		display_sdl_up_data();
		SDL_Delay(5);
	}
	SDL_Quit();
	return 0;
}


void lui_loop(void) {
	for(int i = 0; i < LCD_LENGTH; i += CACHE_LENGTH) {
		for(int j = 0; j < LCD_WIDTH; j += CACHE_WIDTH) {
			lz_drawcache_size_set(j,i,CACHE_WIDTH,CACHE_LENGTH);
			lz_obj_traverse(lz_get_root());
			lz_draw_cache_to_lcd(j,i,CACHE_WIDTH,CACHE_LENGTH);
		}
	}
}



