#pragma once

#include <string>
#include <SDL/SDL.h>

// Sprite class
class Sprite
{
public:
	virtual ~Sprite() {}

	void SetImage(const std::string& filename);
	void SetImage(SDL_Texture* nimage);

	void Visible(bool nstate);
	bool Visible();

	void SetPos(int nx, int ny);
	void SetSpeed(int nspeed);
	void Move(int dir);

	void Draw();

	SDL_Rect* GetRect();

protected:
	SDL_Texture* image;
	SDL_Rect imgrect;
	bool draw;
	int speed;
};