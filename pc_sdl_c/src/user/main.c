
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
#include "lzui/lz_easing.h"
#include "lzui/lz_obj_pos.h"
#include "lzui/lz_obj_mouse.h"

static uint8_t running = 1;
uint8_t touch_flag = 0;
void lui_loop(void);

extern const struct png_t usb_in_png_v;

static int tick_thread(void *data) {
    while(running) {
        SDL_Delay(1);
        lz_tick_inc(1);
        lz_tick_loop();
    }
    return 0;
}

lz_obj_t * pngxx;

void amm( double val) {
    lz_obj_set_y( pngxx, (uint16_t)val );
//    printf("%f,",val);
}

int main(int argc, char** argv) {
	display_sdl();
	display_sdl_up_data();
	lz_draw_set_updata(display_sdl_fill);

    lz_obj_t * mouse = lz_create_mouse(0, 0);
    lz_obj_add_child(lz_get_root(),mouse);

    pngxx = lz_create_png(10,10);
    lz_icon_set_val(pngxx, usb_in_png_v);
    lz_obj_add_child(lz_get_root(),pngxx);
//
//    but = lz_create_button(10,10);
//    lz_obj_add_child(lz_get_root(),but);

//    lz_obj_t * lable = lz_create_lable(0,0);
//    lz_obj_add_child(but,lable);
//    lz_lable_text_set(lable, "23");

    lz_obj_t * pos = lz_create_pos(0,0);
    lz_obj_add_child(lz_get_root(), pos);

    lz_leasing_t * le = lz_easing_create();
    le->start_val = 10;
    le->end_val = 80;
    le->total_time = 40;
    le->func = lz_sine_in_out;
    le->func_c = amm;

//    lz_obj_t * chart = lz_create_chart(50,100);
//    lz_obj_add_child(lz_get_root(),chart);

//    lz_tick_create(but, bt_event, 50);

    SDL_CreateThread(tick_thread, "tick", NULL);

	SDL_Event event;
	while(running) {
		if ( SDL_PollEvent( &event ) ) {
            switch( event.key.keysym.sym ) {
                case SDLK_w: {
//                    printf("w\n");
                    lz_mouse_y_set( mouse, 0 );
                    break;
                }
                case SDLK_s: {
                    lz_mouse_y_set( mouse, 1 );
//                    printf("s\n");
                    break;
                }
                case SDLK_a: {
//                    printf("a\n");
                    lz_mouse_x_set( mouse, 0 );
                    break;
                }
                case SDLK_d: {
                    lz_mouse_x_set( mouse, 1 );
//                    printf("d\n");
                    break;
                }
            }
			switch( event.type ) {
				case SDL_QUIT: {
					running = 0;
				} break;
				case SDL_MOUSEMOTION: {
                    lz_post_set_mouse_x_y( pos, event.button.x, event.button.y );
					if(touch_flag == 1) {
						lz_obj_even(event.button.x, event.button.y, touch_flag);
					}
				} break;
				case SDL_MOUSEWHEEL: {

				} break;
				case SDL_MOUSEBUTTONDOWN: {
                    lz_post_set_is_mouse_down( pos, 1 );
					if( event.button.button == SDL_BUTTON_LEFT ){
						touch_flag = 1;
						lz_obj_even(event.button.x, event.button.y, 2);
					}
				} break;
				case SDL_MOUSEBUTTONUP: {
                    lz_post_set_is_mouse_down( pos, 0 );
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



