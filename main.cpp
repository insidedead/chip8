#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include "cpu.h"

int main()
{
	Chip8 *u = new Chip8();

	u->loadRoam("/home/insidedead/Downloads/flightrunner.ch8");
	u->loadFont();

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("Chip 8 Emulator", 100, 100, 640, 320, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
	
	SDL_Texture *texture;

	while(true)
	{
		for(int i = 0; i < (64 *32); i++)
		{
		}
		u->run();
	}

	delete u;
}
