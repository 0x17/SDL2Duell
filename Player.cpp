#include <SDL/SDL_mixer.h>
#include "Player.h"
#include "Duell.h"

// Functions of Player class
// Constructor
Player::Player(int npnr)
{
	// Init the image of the Player and the rectangle
	pnr = npnr;
	switch (npnr)
	{
	case 1:
		SetImage("player1.bmp");
		SetPos(10, 10);
		break;
	case 2:
		SetImage("player2.bmp");
		int w;
		SDL_QueryTexture(image, nullptr, nullptr, &w, nullptr);
		SetPos(SCREEN_W - w - 10, 10);
		break;
	}
	// Init the Bullets of the Player
	for (int i = 0; i < BULLETS; i++)
	{
		bullets[i].SetSpeed(4);
		bullets[i].SetImage(bullet);
		bullets[i].Visible(false);
	}
	// Init health and heart stuff for the player
	health = LIVES;
	SetSpeed(WALKSPEED);
	lshoot = SDL_GetTicks();
}
// Deconstructor
Player::~Player()
{
	SDL_DestroyTexture(image);
}
// Add new Bullet if the last is over 100 msecs ago
void Player::Shoot()
{
	if (SDL_GetTicks() - lshoot > RELOADTIME)
	{
		for (int i = 0; i < BULLETS; i++)
		{
			if (!bullets[i].Visible())
			{
				int bullet_w;
				SDL_QueryTexture(bullet, nullptr, nullptr, &bullet_w, nullptr);
				if (pnr == 1)
					bullets[i].SetPos(imgrect.x + imgrect.w, imgrect.y + imgrect.w / 2);
				else if (pnr == 2)
					bullets[i].SetPos(imgrect.x - bullet_w, imgrect.y + imgrect.w / 2);
				bullets[i].Visible(true);
				break;
			}
		}
		if (Mix_PlayChannel(0, shoot, 0) == -1)
			printf("Can't play shoot sound!");
		lshoot = SDL_GetTicks();
	}
}
// Move all Bullets of this Player which should
void Player::MoveBullets(int bspeed)
{
	for (int i = 0; i < BULLETS; i++)
	{
		if (bullets[i].Visible())
		{
			bullets[i].SetSpeed(bspeed);
			if (pnr == 1)
				bullets[i].Move(RIGHT);
			else if (pnr == 2)
				bullets[i].Move(LEFT);
		}
	}
}
// Draw all Bullets of this Player which should
void Player::DrawBullets()
{
	for (int i = 0; i < BULLETS; i++)
	{
		if (bullets[i].Visible())
			bullets[i].Draw();
	}
}
// Get the Health of the Player
int Player::GetHealth() { return health; }
// Hurts player
void Player::Hurt()
{
	health--;
	switch (health)
	{
	case 2:
		if (pnr == 1)
			SetImage("player1hurt1.bmp");
		else
			SetImage("player2hurt1.bmp");
		break;
	case 1:
		if (pnr == 1)
			SetImage("player1hurt2.bmp");
		else
			SetImage("player2hurt2.bmp");
		break;
	case 0:
		if (pnr == 1)
			SetImage("player1hurt3.bmp");
		else
			SetImage("player2hurt3.bmp");
		break;
	}
}
// Makes a Bullet invisible
void Player::InvisibleBullet(int bulletnr)
{
	bullets[bulletnr].Visible(false);
	if (Mix_PlayChannel(0, explode, 0) == -1)
		printf("Can't play explode sound!");
}
// Returns if the Bullet is Visible
bool Player::BulletVisible(int bulletnr) { return bullets[bulletnr].Visible(); }
// Get Rectangle of Bullet
SDL_Rect* Player::GetBulletRect(int bulletnr) { return bullets[bulletnr].GetRect(); }
