#include <iostream>
#include "SDL2/SDL.h"
#include "bsp/display_sdl.h"
#include "lzui/ugui.h"

using namespace std;

static int tick_thread(void *data);
static uint8_t running = 1 ;
display_sdl * sdl_lcd;
UG_GUI guiST7735;

void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c) {
	sdl_lcd->draw_point(x,y,c);
}

UG_COLOR color[3]= {C_RED, C_GREEN, C_BLUE};


#define MAX_OBJECTS 10
#define BuffOUT     40

char bufferOut[BuffOUT];

uint16_t Px, Py, PosX, PosY;

UG_WINDOW window_1;
UG_BUTTON button_0;
UG_BUTTON button_1;
UG_BUTTON button_2;
UG_BUTTON button_3;
UG_TEXTBOX textbox_1;
UG_IMAGE image_1;

UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];

void window_1_callback(UG_MESSAGE *msg);

int main(int argc, char** argv) {
	sdl_lcd = new display_sdl();

	UG_Init(&guiST7735, Display_PSet, 320, 240);

	/* Clear screen. */
	UG_FillScreen(C_BLACK);
	UG_DrawFrame(0, 0, 320-1, 240-1, C_LIGHT_GRAY);     // Draw a Frame

	//Some Texts
	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_YELLOW);
	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_YELLOW);
	UG_PutString(5, 23, "STM32f103RB and ST7789");

	// Create the window
	UG_WindowCreate(&window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback);
	// Window Title
	UG_WindowSetTitleText(&window_1, "Window \xe6GUI v0.31");      //  \xhh : Special CHR the ASCII value is given by hh interpreted as a hexadecimal number (Check FONT Table)
	UG_WindowSetXStart(&window_1, 25);
	UG_WindowSetYStart(&window_1, 50);
	UG_WindowSetXEnd(&window_1, 220);       // Window 175x250
	UG_WindowSetYEnd(&window_1, 300);

	// Create Buttons
	UG_ButtonCreate(&window_1, &button_0, BTN_ID_0, 10, 10, 110, 60);
	UG_ButtonCreate(&window_1, &button_1, BTN_ID_1, 10, 80, 110, 130);
	UG_ButtonCreate(&window_1, &button_2, BTN_ID_2, 10, 150, 110, 200);

	UG_ButtonSetForeColor(&window_1,BTN_ID_0, C_BLUE);
	UG_ButtonSetText(&window_1,BTN_ID_0,"Button \n0");
	UG_ButtonSetForeColor(&window_1,BTN_ID_1, C_RED);
	UG_ButtonSetText(&window_1,BTN_ID_1,"Button \n1");
	UG_ButtonSetForeColor(&window_1,BTN_ID_2, C_FOREST_GREEN);
	UG_ButtonSetText(&window_1,BTN_ID_2,"Button \n2");

	// Create Textbox
	UG_TextboxCreate(&window_1, &textbox_1, TXB_ID_0, 120, 10, 180, 80);
	UG_TextboxSetText(&window_1, TXB_ID_0, "Text\nbox");
	UG_TextboxSetForeColor(&window_1, TXB_ID_0, C_BLACK);
	UG_TextboxSetAlignment(&window_1, TXB_ID_0, ALIGN_CENTER);


	UG_WindowShow(&window_1);
	UG_Update();

	SDL_CreateThread(tick_thread, "tick", NULL);
	SDL_Event event;
	while(running) {
		if ( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
				case SDL_QUIT: {
					running = 0;
				} break;
				case SDL_MOUSEMOTION: {

				} break;
				case SDL_MOUSEWHEEL: {

				} break;
				case SDL_MOUSEBUTTONDOWN: {

				} break;
				case SDL_MOUSEBUTTONUP: {

				} break;
			}
		}
		SDL_Delay(10);
	}
    SDL_Quit();
    return 0;
}

static int tick_thread(void *data) {
	while(running) {
		SDL_Delay(1);
		//lui_tick_inc(1);
	}
}
