#include "Sprite.h"
#include "Globals.h"
#include "Utils.h"

Sprite::Sprite() : image(nullptr), draw(true), speed(0) {
	memset(&imgrect, 0, sizeof(imgrect));
}

// Functions of Sprite class
// Change the image to a image that is loaded from a file
void Sprite::SetImage(const std::string & filename)
{
	image = LoadBMP(filename, true);
	if (image == nullptr)
	{
		printf("Can't load image!");
		exit(1);
	}
	int w, h;
	SDL_QueryTexture(image, nullptr, nullptr, &w, &h);
	imgrect.w = w;
	imgrect.h = h;
}
// Change the image to an image that is already in the memory
void Sprite::SetImage(SDL_Texture* nimage)
{
	image = nimage;
	int w, h;
	SDL_QueryTexture(image, nullptr, nullptr, &w, &h);
	imgrect.w = w;
	imgrect.h = h;
}
// Set, if the Sprite is visible or not
void Sprite::Visible(bool nstate) { draw = nstate; }
// Check if the Sprite is visible
bool Sprite::Visible() { return draw; }
// Set the position of the Sprite to the given coords
void Sprite::SetPos(int nx, int ny)
{
	imgrect.x = nx;
	imgrect.y = ny;
}
// Set new speed
void Sprite::SetSpeed(int nspeed) { speed = nspeed; }
// Move the Sprite with the given speed in the given direction
void Sprite::Move(int dir)
{
	switch (dir)
	{
	case LEFT:
		imgrect.x -= speed;
		break;
	case RIGHT:
		imgrect.x += speed;
		break;
	case UP:
		imgrect.y -= speed;
		break;
	case DOWN:
		imgrect.y += speed;
		break;
	}
}
// Draw the Sprite onto the Screen
void Sprite::Draw() {
	SDL_RenderCopy(renderer, image, nullptr, &imgrect);
}
// Get rectangle of Sprite
SDL_Rect* Sprite::GetRect() { return &imgrect; }