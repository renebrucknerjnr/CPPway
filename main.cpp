/* 
  cd "/c/users/administrator/desktop/CPPway/"; clear;
  g++ main.cpp -o run.exe -lSDL2 -I/c/msys64/mingw64/include/SDL2 -L/msys64/mingw64/lib;
  ./run.exe
 */

// pacman -S mingw-w64-x86_64-cmake

// git clone https://github.com/libsdl-org/SDL.git
// cd SDL
// mkdir build && cd build
// cmake -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DSDL_STATIC=ON ..
// mingw32-make


// g++ main.cpp -o Cells.exe -I/c/msys64/mingw64/include/SDL2 -L/msys64/mingw64/lib -lSDL2 -lSDL2main -static

// needed to prevent "undefined reference to `winMain`" (tells SDL that you are haldling the main function yourself)
#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>
#include <ctime>

int main() {
	
	if (!(SDL_Init(SDL_INIT_EVERYTHING) == 0)) {
		std::cout << "Failed to initialize everything" << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow(
		"Cellular Automata",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_SHOWN);
	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		SDL_Quit();
		return 2;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cout << "Failed to create renderer" << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 3;
	}

	std::cout << "Use spacebar to pause" << std::endl;
	std::cout << "Use C to clear" << std::endl;
	std::cout << "Use F to fill" << std::endl;
	std::cout << "Use T to change algorithm type" << std::endl;
	std::cout << "Use left mouse button to place" << std::endl;
	std::cout << "Use right mouse button to remove" << std::endl;
	bool running = true;
	bool paused = true;
	bool brainType = false;
	SDL_Event event;
	unsigned int frameStart;
	unsigned int frameTime;
	unsigned int FPS = 5;
	unsigned int interval = 1000/FPS;
	srand(time(NULL)); // set random seed to be "random" for every run


	bool buffer1[80*60];
	bool buffer2[80*60];

	for (int i = 0; i < 4800; i++) {
		// buffer1[i] = rand()%100 < 45 ? 1 : 0; // use with terrain
		buffer1[i] = 0;
		buffer2[i] = 0;
	}

	// S
	buffer1[81+0+80*0] = 1;
	buffer1[81+0+80*1] = 1;
	buffer1[81+0+80*2] = 1;
	buffer1[81+1+80*2] = 1;
	buffer1[81+2+80*2] = 1;
	buffer1[81+2+80*3] = 1;
	buffer1[81+2+80*4] = 1;
	buffer1[81+1+80*4] = 1;
	buffer1[81+0+80*4] = 1;
	buffer1[81+1+80*0] = 1;
	buffer1[81+2+80*0] = 1;
	
	// P
	buffer1[85+0+80*0] = 1;
	buffer1[85+1+80*0] = 1;
	buffer1[85+2+80*0] = 1;
	buffer1[85+2+80*1] = 1;
	buffer1[85+2+80*2] = 1;
	buffer1[85+1+80*2] = 1;
	buffer1[85+0+80*2] = 1;
	buffer1[85+0+80*1] = 1;
	buffer1[85+0+80*3] = 1;
	buffer1[85+0+80*4] = 1;
	
	// A
	buffer1[89+1+80*0] = 1;
	buffer1[89+0+80*1] = 1;
	buffer1[89+0+80*2] = 1;
	buffer1[89+1+80*2] = 1;
	buffer1[89+0+80*3] = 1;
	buffer1[89+0+80*4] = 1;
	buffer1[89+2+80*1] = 1;
	buffer1[89+2+80*2] = 1;
	buffer1[89+2+80*3] = 1;
	buffer1[89+2+80*4] = 1;
	
	// C
	buffer1[93+1+80*0] = 1;
	buffer1[93+2+80*0] = 1;
	buffer1[93+0+80*1] = 1;
	buffer1[93+0+80*2] = 1;
	buffer1[93+0+80*3] = 1;
	buffer1[93+1+80*4] = 1;
	buffer1[93+2+80*4] = 1;
	
	// E
	buffer1[97+0+80*0] = 1;
	buffer1[97+1+80*0] = 1;
	buffer1[97+2+80*0] = 1;
	buffer1[97+0+80*1] = 1;
	buffer1[97+0+80*2] = 1;
	buffer1[97+1+80*2] = 1;
	buffer1[97+0+80*3] = 1;
	buffer1[97+0+80*4] = 1;
	buffer1[97+1+80*4] = 1;
	buffer1[97+2+80*4] = 1;
	
	// C
	buffer1[561+1+80*0] = 1;
	buffer1[561+2+80*0] = 1;
	buffer1[561+0+80*1] = 1;
	buffer1[561+0+80*2] = 1;
	buffer1[561+0+80*3] = 1;
	buffer1[561+1+80*4] = 1;
	buffer1[561+2+80*4] = 1;
	
	// F
	buffer1[1041+0+80*0] = 1;
	buffer1[1041+1+80*0] = 1;
	buffer1[1041+2+80*0] = 1;
	buffer1[1041+0+80*1] = 1;
	buffer1[1041+0+80*2] = 1;
	buffer1[1041+1+80*2] = 1;
	buffer1[1041+0+80*3] = 1;
	buffer1[1041+0+80*4] = 1;
	
	// T
	buffer1[1521+0+80*0] = 1;
	buffer1[1521+1+80*0] = 1;
	buffer1[1521+2+80*0] = 1;
	buffer1[1521+1+80*1] = 1;
	buffer1[1521+1+80*2] = 1;
	buffer1[1521+1+80*3] = 1;
	
	// M
	buffer1[2001+0+80*0] = 1;
	buffer1[2001+0+80*1] = 1;
	buffer1[2001+0+80*2] = 1;
	buffer1[2001+0+80*3] = 1;
	buffer1[2001+0+80*4] = 1;
	buffer1[2001+1+80*1] = 1;
	buffer1[2001+2+80*0] = 1;
	buffer1[2001+2+80*1] = 1;
	buffer1[2001+2+80*2] = 1;
	buffer1[2001+2+80*3] = 1;
	buffer1[2001+2+80*4] = 1;
	
	// 1
	buffer1[2005+0+80*1] = 1;
	buffer1[2005+1+80*0] = 1;
	buffer1[2005+1+80*1] = 1;
	buffer1[2005+1+80*2] = 1;
	buffer1[2005+1+80*3] = 1;
	buffer1[2005+0+80*4] = 1;
	buffer1[2005+1+80*4] = 1;
	buffer1[2005+2+80*4] = 1;

	// M
	buffer1[2481+0+80*0] = 1;
	buffer1[2481+0+80*1] = 1;
	buffer1[2481+0+80*2] = 1;
	buffer1[2481+0+80*3] = 1;
	buffer1[2481+0+80*4] = 1;
	buffer1[2481+1+80*1] = 1;
	buffer1[2481+2+80*0] = 1;
	buffer1[2481+2+80*1] = 1;
	buffer1[2481+2+80*2] = 1;
	buffer1[2481+2+80*3] = 1;
	buffer1[2481+2+80*4] = 1;
	
	// 2
	buffer1[2485+0+80*0] = 1;
	buffer1[2485+1+80*0] = 1;
	buffer1[2485+2+80*0] = 1;
	buffer1[2485+2+80*1] = 1;
	buffer1[2485+2+80*2] = 1;
	buffer1[2485+1+80*2] = 1;
	buffer1[2485+0+80*2] = 1;
	buffer1[2485+0+80*3] = 1;
	buffer1[2485+0+80*4] = 1;
	buffer1[2485+1+80*4] = 1;
	buffer1[2485+2+80*4] = 1;

	while (running) {
		frameStart = SDL_GetTicks();

		// do cells
		if (!paused) {
			for (int i = 0; i < 4800; i++) { // copy buffer
				buffer2[i] = buffer1[i];
			}
			for (int i = 0; i < 4800; i++) { // check
				char kern = 0;
				if (i - 1 >= 0 && (i - 1)%80 != 79 && buffer2[i - 1] == true) kern++;
				if (i + 1 < 80*60 && (i + 1)%80 != 0 && buffer2[i + 1] == true) kern++;
				if (i - 80 >= 0 && (i - 80)/80 > 0 && buffer2[i - 80] == true) kern++;
				if (i + 80 < 80*60 && (i + 80)/80 < 60 && buffer2[i + 80] == true) kern++;

				// cool, but false
				// if ((i - 1 >= 0 && (i - 1)%80 != 79 && buffer2[i - 1] == true) && (i - 80 >= 0 && (i - 80)/80 > 0 && buffer2[i - 80] == true)) kern++;
				// if ((i - 1 >= 0 && (i - 1)%80 != 79 && buffer2[i - 1] == true) && (i + 80 < 80*60 && (i + 80)/80 < 60 && buffer2[i + 80] == true)) kern++;
				// if ((i + 1 < 80*60 && (i + 1)%80 != 0 && buffer2[i + 1] == true) && (i - 80 >= 0 && (i - 80)/80 > 0 && buffer2[i - 80] == true)) kern++;
				// if ((i + 1 < 80*60 && (i + 1)%80 != 0 && buffer2[i + 1] == true) && (i + 80 < 80*60 && (i + 80)/80 < 60 && buffer2[i + 80] == true)) kern++;

				if (i - 1 >= 0 && (i - 1)%80 != 79 && i - 80 >= 0 && (i - 80)/80 > 0 && buffer2[i - 1 - 80] == true) kern++;
				if (i - 1 >= 0 && (i - 1)%80 != 79 && (i + 80 < 80*60 && (i + 80)/80 < 60 && buffer2[i - 1 + 80] == true)) kern++;
				if (i + 1 < 80*60 && (i + 1)%80 != 0 && (i - 80 >= 0 && (i - 80)/80 > 0 && buffer2[i + 1 - 80] == true)) kern++;
				if (i + 1 < 80*60 && (i + 1)%80 != 0 && (i + 80 < 80*60 && (i + 80)/80 < 60 && buffer2[i + 1 + 80] == true)) kern++;

				if (brainType) { // terrain
					if (kern > 4) buffer1[i] = 1;
					if (kern < 3) buffer1[i] = 0;
					// if (kern == 0) buffer1[i] = rand()%100 < 10 ? 1 : 0;
				} else { // conway's
					if (buffer2[i] && (kern < 2 || kern > 3)) buffer1[i] = 0;
					if (!buffer2[i] && kern == 3) buffer1[i] = 1;
				}
			}
		}

		// handle events
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case (SDL_QUIT):
				running = false;
				break;

			case (SDL_MOUSEBUTTONDOWN):
				// std::cout << "x: " << event.button.x/10 << ", y: " << event.button.y/10 << ", i: " << event.button.x/10+event.button.y/10*80 << std::endl;
				if (event.button.button == SDL_BUTTON_LEFT) {
					buffer1[(int)(event.button.x/10+event.button.y/10*80)] = 1;
					buffer2[(int)(event.button.x/10+event.button.y/10*80)] = 1;
				} else {
					buffer1[(int)(event.button.x/10+event.button.y/10*80)] = 0;
					buffer2[(int)(event.button.x/10+event.button.y/10*80)] = 0;
				}
				break;

			case (SDL_KEYUP):
				if (event.key.keysym.sym == SDLK_SPACE) {
					paused = !paused;
				} else if (event.key.keysym.sym == SDLK_t) {
					brainType = !brainType;
				} else if (event.key.keysym.sym == SDLK_c) {
					for (int i = 0; i < 4800; i++) {
						buffer1[i] = 0;
						buffer2[i] = 0;
					}
				} else if (event.key.keysym.sym == SDLK_f) {
					for (int i = 0; i < 4800; i++) {
						buffer1[i] = rand()%100 < 45 ? 1 : 0;
						buffer2[i] = 0;
					}
				}
				break;

			default:
				break;
			}
		}


		// clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// draw
		for (int i = 0; i < 4800; i++) {
			if (!buffer1[i]) continue;

			SDL_Rect r = {(i%80)*10, (int)(i/80)*10, 10, 10};
			// SDL_SetRenderDrawColor(renderer, rand()%128+128, rand()%128+128, rand()%128+128, 255);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &r);
			SDL_SetRenderDrawColor(renderer, 191, 191, 191, 255);
			SDL_RenderDrawRect(renderer, &r);
		}

		// show
		SDL_RenderPresent(renderer);

		// slow down
		frameTime = SDL_GetTicks() - frameStart;
		if (interval > frameTime) {
			SDL_Delay(interval - frameTime);
		}
	}

	// cleanup
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}