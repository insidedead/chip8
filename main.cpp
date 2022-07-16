#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
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
	
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

	while(true)
	{
		u->run();
	
		if(u->draw)
		{
			uint32_t *buffer = new uint32_t[32 * 64];
			
			for(int i = 0; i < (32 * 64); i++)
			{
				buffer[i] = 0xFFFFFFFF;
			}

			SDL_UpdateTexture(texture, NULL, buffer, 64 * sizeof(uint32_t));
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}	
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	delete u;
}
