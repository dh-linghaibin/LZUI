
#include <stdio.h>
#include <lzui/ugui_config.h>
#include <lzui/ugui.h>
#include "SDL2/SDL.h"
#include "bsp/display_sdl.h"

static uint8_t running = 1;

UG_GUI guiST7735;

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

void Display_PSet(UG_S16 x, UG_S16 y, UG_COLOR c) {
	display_sdl_draw_point(x,y,c);
}


UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
	for(int y = y1; y < y2; y++) {
		for(int x = x1; x < x2; x++) {
			display_sdl_draw_point(x,y,c);
		}
	}
	return UG_RESULT_OK;
}

UG_RESULT HW_DrawLine( UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c )
{
	if((x1 < 0) ||(x1 >= 320) || (y1 < 0) || (y1 >= 240)) return UG_RESULT_FAIL;
	if((x2 < 0) ||(x2 >= 320) || (y2 < 0) || (y2 >= 240)) return UG_RESULT_FAIL;

	// If it is a vertical or a horizontal line, draw it.
	// If not, then use original drawline routine.
	if ((x1 == x2) || (y1 == y2))
	{
		HW_FillFrame(x1, y1, x2, y2, c);
		return UG_RESULT_OK;
	}

	return UG_RESULT_FAIL;
}

int main(int argc, char** argv) {
	display_sdl();

	UG_Init(&guiST7735, Display_PSet, 320, 240);
	UG_DriverRegister(DRIVER_FILL_FRAME, (void*)HW_FillFrame);
	UG_DriverRegister(DRIVER_DRAW_LINE, (void*)HW_DrawLine);
//	UG_DriverRegister(DRIVER_DRAW_IMAGE, (void*)HW_DrawImage);

	UG_DrawFrame(0, 0, 320-1, 240-1, C_LIGHT_GRAY);     // Draw a Frame

	//Some Texts
	UG_FontSelect(&FONT_8X14);
	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_YELLOW);
	sprintf(bufferOut,"test TFT 2,4\" (%dx%d)", 320, 240);
	UG_PutString(5, 5 , bufferOut);
	UG_FontSelect(&FONT_8X14);
	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_YELLOW);
	UG_PutString(5, 23, "STM32f103RB and ST7789");

	// Create the window
	UG_WindowCreate(&window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback);
	// Window Title
	UG_WindowSetTitleText(&window_1, "Window \xe6GUI v0.31");      //  \xhh : Special CHR the ASCII value is given by hh interpreted as a hexadecimal number (Check FONT Table)
	UG_WindowSetTitleTextFont(&window_1, &FONT_8X14);
	UG_WindowSetXStart(&window_1, 25);
	UG_WindowSetYStart(&window_1, 50);
	UG_WindowSetXEnd(&window_1, 220);       // Window 175x250
	UG_WindowSetYEnd(&window_1, 300);

	// Create Buttons
	UG_ButtonCreate(&window_1, &button_0, BTN_ID_0, 10, 10, 110, 60);
	UG_ButtonCreate(&window_1, &button_1, BTN_ID_1, 10, 80, 110, 130);
	UG_ButtonCreate(&window_1, &button_2, BTN_ID_2, 10, 150, 110, 200);
//	UG_ButtonCreate(&window_1, &button_3, BTN_ID_3, 120-2, 150-2, 120+2+ hare.width, 150+2 + hare.height);  // + logo.width, 100 + logo.height); // Button under image to have an image with touch event
	//Label Buttons
	UG_ButtonSetFont(&window_1,BTN_ID_0,&FONT_8X14);
	UG_ButtonSetForeColor(&window_1,BTN_ID_0, C_BLUE);
	UG_ButtonSetText(&window_1,BTN_ID_0,"Button \n0");
	UG_ButtonSetFont(&window_1,BTN_ID_1,&FONT_8X14);
	UG_ButtonSetForeColor(&window_1,BTN_ID_1, C_RED);
	UG_ButtonSetText(&window_1,BTN_ID_1,"Button \n1");
	UG_ButtonSetFont(&window_1,BTN_ID_2,&FONT_8X14);
	UG_ButtonSetForeColor(&window_1,BTN_ID_2, C_FOREST_GREEN);
	UG_ButtonSetText(&window_1,BTN_ID_2,"Button \n2");

	// Create Textbox
	UG_TextboxCreate(&window_1, &textbox_1, TXB_ID_0, 120, 10, 180, 80);
	UG_TextboxSetFont(&window_1, TXB_ID_0, &FONT_8X14);
	UG_TextboxSetText(&window_1, TXB_ID_0, "Text\nbox");
	UG_TextboxSetForeColor(&window_1, TXB_ID_0, C_BLACK);
	UG_TextboxSetAlignment(&window_1, TXB_ID_0, ALIGN_CENTER);

	// Create Image
//	UG_ImageCreate(&window_1, &image_1, IMG_ID_0, 120, 150, 120 + hare.width, 150 + hare.height); // Image doesn't support touch events so there's a button under the image
//	UG_ImageSetBMP(&window_1, IMG_ID_0, &hare);

	UG_WindowShow(&window_1);
	UG_Update();

	display_sdl_up_data();

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


void window_1_callback(UG_MESSAGE *msg)
{
	if(msg->type == MSG_TYPE_OBJECT)
	{
		if(msg->id == OBJ_TYPE_BUTTON)
		{
			if(msg->event == OBJ_EVENT_PRESSED)
			{
				switch(msg->sub_id)
				{
					case BTN_ID_0:
					{
						UG_ButtonHide(&window_1,BTN_ID_0);
						break;
					}
					case BTN_ID_1:
					{
						UG_TextboxSetText(&window_1, TXB_ID_0, "But 1");
						break;
					}
					case BTN_ID_2:
					{
						UG_ButtonShow(&window_1,BTN_ID_0);
						break;
					}
					case BTN_ID_3:
					{
						UG_TextboxSetText(&window_1, TXB_ID_0, "hare");
						break;
					}
				}
			}
			if(msg->event == OBJ_EVENT_RELEASED)
			{
				if(msg->sub_id == BTN_ID_1)
				{
					UG_TextboxSetText(&window_1, TXB_ID_0, "AGAIN");
				}
				if(msg->sub_id == BTN_ID_3)
				{
					UG_TextboxSetText(&window_1, TXB_ID_0, "YEAH!");
				}
			}
		}
		//HAL_Delay(5);
	}
}

