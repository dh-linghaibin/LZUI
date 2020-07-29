#include <iostream>
#include <vector>
#include <algorithm>
#include "SDL2/SDL.h"
#include "bsp/display_sdl.h"

using namespace std;

static int tick_thread(void *data);
static uint8_t running = 1 ;

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


int main(int argc, char** argv) {
	display_sdl * sdl_lcd;
	sdl_lcd = new display_sdl();

	SDL_CreateThread(tick_thread, "tick", NULL);

	auto lambda =[=](int value){std::cout <<"value: "<< value << std::endl;};

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

	AddressBook global_address_book;
	global_address_book.findMatchingAddresses(
			// we're declaring a lambda here; the [] signals the start
			[] (const string& addr) { return addr.find( ".org" ) != string::npos; }
	);

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
