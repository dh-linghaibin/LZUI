//
// Created by a6735 on 2020/7/29.
//

#include "bsp/display_sdl.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class AddressBook {
public:
	// using a template allows us to ignore the differences between functors, function pointers
	// and lambda
	template<typename Func>
	std::vector<std::string> findMatchingAddresses (Func func) {
		std::vector<std::string> results;
		for ( auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr ) {
			// call the function passed into findMatchingAddresses and see if it matches
			if ( func( *itr ) ) {
				results.push_back( *itr );
			}
		}
		return results;
	}
private:
	std::vector<std::string> _addresses;
};

display_sdl :: display_sdl( ) {
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		cout << "Something went wrong! " << SDL_GetError() << endl;
	}

	SDL_Window* window = SDL_CreateWindow("LZUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	up_data();


//	auto lambda =[=](int value){std::cout <<"value: "<< value << std::endl;};
//	vector<int> data;
//	vector<int> result;
//	for (int i = 0; i < 10; ++i)
//		data.push_back(i);
//	for_each(data.begin(), data.end(), [&result](int &elem){
//		if (elem % 2 == 0)
//			result.push_back(elem);
//	});
//	for_each(result.begin(), result.end(), [](int &elem){
//		cout << elem << endl;
//	});

	AddressBook global_address_book;
	global_address_book.findMatchingAddresses(
			// we're declaring a lambda here; the [] signals the start
			[] (const string& addr) {
				cout << "测试";
				return addr.find( ".org" ) != string::npos;
			}
	);
}

void display_sdl::up_data() {
	update_texture();
	render_texture();
}

void display_sdl::draw_point( uint16_t x, uint16_t y, uint16_t color) {
	screen[y*LCD_WIDTH+x] = color;
	up_data();
}

void display_sdl :: update_texture( ) {
	SDL_UpdateTexture(texture, NULL, screen, LCD_WIDTH * sizeof(screen[0]));
}

void display_sdl :: render_texture( ) {
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



