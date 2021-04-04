#pragma once

#include "Sprite.h"
#include "Globals.h"

// Player class
class Player : public Sprite
{
public:
	Player(int npnr);
	virtual ~Player();
	void Shoot();
	void MoveBullets(int bspeed);
	void DrawBullets();
	int GetHealth() const;
	void Hurt();
	void InvisibleBullet(int bulletnr);
	bool BulletVisible(int bulletnr);
	SDL_Rect* GetBulletRect(int bulletnr);
private:
	Sprite bullets[BULLETS];
	Uint32 lshoot;
	int health, pnr;
};
