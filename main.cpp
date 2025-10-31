/* 
  cd "/c/users/administrator/desktop/CPPway/"; clear;
  g++ main.cpp -o run.exe -lSDL2 -I/c/msys64/mingw64/include/SDL2 -L/msys64/mingw64/lib;
  ./run.exe
 */

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
	bool paused = false;
	bool brainType = true;
	SDL_Event event;
	unsigned int frameStart;
	unsigned int frameTime;
	unsigned int FPS = 5;
	unsigned int interval = 1000/FPS;
	srand(time(NULL)); // set random seed to be "random" for every run


	bool buffer1[80*60];
	bool buffer2[80*60];

	for (int i = 0; i < 4800; i++) {
		buffer1[i] = rand()%100 < 45 ? 1 : 0; // use with terrain
		// buffer1[i] = 0;
		buffer2[i] = 0;
	}
	// buffer1[2440] = 1;
	// buffer1[2520] = 1;
	// buffer1[2600] = 1;
	// buffer1[2680] = 1;
	// buffer1[2760] = 1;
	// buffer1[2840] = 1;
	// buffer1[2920] = 1;
	// buffer1[3000] = 1;
	// buffer1[2681] = 1;
	// buffer1[2682] = 1;
	// buffer1[2683] = 1;
	// buffer1[2684] = 1;
	// buffer1[2685] = 1;

	// buffer1[2282] = 1;
	// buffer1[2283] = 1;
	// buffer1[2284] = 1;
	// buffer1[2285] = 1;
	// buffer1[2286] = 1;
	// buffer1[2287] = 1;
	
	// buffer1[2207] = 1;
	// buffer1[2127] = 1;
	// buffer1[2047] = 1;
	// buffer1[1967] = 1;

	// buffer1[2367] = 1;
	// buffer1[2447] = 1;
	// buffer1[2527] = 1;

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