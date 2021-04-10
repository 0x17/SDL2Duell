#pragma once

#include <SDL.h>
#include <string>

extern SDL_Window* screen;
extern SDL_Renderer* renderer;

SDL_Texture* LoadBMP(const std::string &filename, bool withColorKey = false);
bool Coll(SDL_Rect* rect1, SDL_Rect* rect2);
void TestColl();

int InitSDL();
void Deinit();
