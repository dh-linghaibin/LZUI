#include <iostream>
#include <vector>
#include <algorithm>

#include "include/SDL2/SDL.h"

using namespace std;

uint8_t running = 1 ;
static int tick_thread(void *data) {
	while(running) {
		SDL_Delay(1);
		//lui_tick_inc(1);
	}
}

#define LCD_WIDTH  320
#define LCD_LENGTH 240

#define CACHE_WIDTH  320
#define CACHE_LENGTH 240

#define CACHE_SIZE   CACHE_LENGTH * CACHE_WIDTH

#define BEST_CACHE_LENGTH_BLACK LCD_LENGTH/CACHE_LENGTH
#define BEST_CACHE_WIDTH_BLACK  LCD_WIDTH/CACHE_WIDTH

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

class AddressBook
{
public:
	// using a template allows us to ignore the differences between functors, function pointers
	// and lambda
	template<typename Func>
	std::vector<std::string> findMatchingAddresses (Func func)
	{
		std::vector<std::string> results;
		for ( auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr )
		{
			// call the function passed into findMatchingAddresses and see if it matches
			if ( func( *itr ) )
			{
				results.push_back( *itr );
			}
		}
		return results;
	}

private:
	std::vector<std::string> _addresses;
};


int main(int argc, char** argv) {
	SDL_Event event;
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

	SDL_CreateThread(tick_thread, "tick", NULL);

	auto lambda =[=](int value){std::cout <<"value: "<< value << std::endl;};

	int a = 0;
	auto f = ([a]()->void {cout << a <<  20 << endl;});
	f();

	vector<int> data;
	vector<int> result;
	for (int i = 0; i < 10; ++i)
		data.push_back(i);
	for_each(data.begin(), data.end(), [&result](int &elem){
		if (elem % 2 == 0)
			result.push_back(elem);
	});
	for_each(result.begin(), result.end(), [](int &elem){
		cout << elem << endl;
	});

//	button_on_callback([](int cmd){
//
//	});
	AddressBook global_address_book;
	global_address_book.findMatchingAddresses(
			// we're declaring a lambda here; the [] signals the start
			[] (const string& addr) { return addr.find( ".org" ) != string::npos; }
	);

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