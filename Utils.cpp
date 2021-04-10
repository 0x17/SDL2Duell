#include <cstdlib>
#include <string>

#include <SDL_mixer.h>
#include <SDL_image.h>

#include "Globals.h"
#include "Utils.h"

SDL_Window* screen;
SDL_Renderer* renderer;

int InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
		return 1;
	else
		atexit(Deinit);
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) == -1)
		return 1;

	screen = SDL_CreateWindow("SDL|Duell - by Andre\' Schnabel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
	if (screen == nullptr)
		return 1;

	renderer = SDL_CreateRenderer(screen, -1, 0);
	if (renderer == nullptr)
		return 1;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, SCREEN_W, SCREEN_H);

	SDL_ShowCursor(SDL_DISABLE);

	return 0;
}

void Deinit()
{
	Mix_CloseAudio();
	SDL_Quit();
}

SDL_Texture* LoadBMP(const std::string& filename, bool withColorKey)
{
	SDL_Surface* temp;
	temp = IMG_Load((ASSET_PATH + filename).c_str());
	if(withColorKey)
		SDL_SetColorKey(temp, 1, SDL_MapRGB(temp->format, 255, 0, 255));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	return tex;
}