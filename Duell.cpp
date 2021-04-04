#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

#include "Duell.h"
#include "Player.h"
#include "Utils.h"

SDL_Texture *bg, * bullet;

const Uint8 *keys;
int state = SPLASH;
bool done = false;

Player *player1, *player2;
Mix_Chunk *shoot, *explode;
Mix_Music *music;

// The function where everything begins...
int main(int argc, char *argv[])
{
	SDL_Event event;

	// Inits
	if(InitSDL() != 0)
	{
		std::cout << "Can't init SDL!" << std::endl;
		return 1;
	}
	// Main loop
	while(!done)
	{
		// Message processing
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDL_SCANCODE_ESCAPE) {
					done = true;
				}
				break;
            default:
                break;
			}
		}
		// Gameworks
		switch(state)
		{
        default:
		case SPLASH:
			ShowSplash();
			break;
		case START:
			InitGame();
			break;
		case RUNNING:
			UpdateGame();
			break;
		}
	}

	if(bg != nullptr)
		SDL_DestroyTexture(bg);
	if(bullet != nullptr)
		SDL_DestroyTexture(bullet);	

	delete player1, player2;

	Mix_FreeChunk(shoot);
	Mix_FreeChunk(explode);
	Mix_FreeMusic(music);

	return 0;
}

// Show Splashscreen until pressing enter
void ShowSplash()
{
	SDL_Texture *splashimg;

	splashimg = LoadBMP("splash.jpg");

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, splashimg, nullptr, nullptr);
	SDL_RenderPresent(renderer);

	while(!done)
	{
		// First we need to refresh the keytable array
		SDL_PumpEvents();
		// And then we read it out...
		keys = SDL_GetKeyboardState(nullptr);
		// Now we look, if some special key is pressed
		if(keys[SDL_SCANCODE_ESCAPE])
		{
			done = true;
			exit(0);
		}
		if (keys[SDL_SCANCODE_RETURN]) {
			break;
		}
	}
	// Free the memory from the splashscreen
	SDL_DestroyTexture(splashimg);
	state = START;
}
// Init the game
void InitGame()
{
	shoot = Mix_LoadWAV((ASSET_PATH + "shoot.wav").c_str());
	explode = Mix_LoadWAV((ASSET_PATH + "explode.wav").c_str());
	music = Mix_LoadMUS((ASSET_PATH + "music.wav").c_str());
	bg = LoadBMP("bg.jpg");

	if(bg == nullptr)
	{
		printf("Can't load background image!");
		exit(1);
	}
	
	bullet = LoadBMP("bullet.bmp");
	if(bullet == nullptr)
	{
		printf("Can't load bullet image!");
		exit(1);
	}

	SDL_RenderCopy(renderer, bg, nullptr, nullptr);

	// Init Players
	player1 = new Player(1);
	player2 = new Player(2);
	state = RUNNING;
	if(Mix_PlayMusic(music, -1) == -1)
		printf("Can't load music!");	
}
// Update the complete game stuff
void UpdateGame()
{	
	// Get the time of the start of this frame
	Uint32 lframe = SDL_GetTicks();
	// Input handling
	keys = SDL_GetKeyboardState(nullptr);
	// Player 1 Controls
	if(keys[SDL_SCANCODE_UP])
	{
		if(player1->GetRect()->y > 0)
			player1->Move(UP);
	}
	if(keys[SDL_SCANCODE_DOWN])
	{
		if(player1->GetRect()->y + player1->GetRect()->h < SCREEN_H)
			player1->Move(DOWN);
	}
	if(keys[SDL_SCANCODE_RIGHT])
	{
		if(player1->GetRect()->x + player1->GetRect()->w < SCREEN_W / 2)
			player1->Move(RIGHT);
	}
	if(keys[SDL_SCANCODE_LEFT])
	{
		if(player1->GetRect()->x > 0)
			player1->Move(LEFT);
	}
	if(keys[SDL_SCANCODE_RCTRL])
	{
		player1->Shoot();
	}
	// Player 2 Controls
	if(keys[SDL_SCANCODE_W])
	{
		if(player2->GetRect()->y > 0)
			player2->Move(UP);
	}
	if(keys[SDL_SCANCODE_S])
	{
		if(player2->GetRect()->y + player2->GetRect()->h < SCREEN_H)
			player2->Move(DOWN);
	}
	if(keys[SDL_SCANCODE_D])
	{
		if(player2->GetRect()->x + player2->GetRect()->w < SCREEN_W)
			player2->Move(RIGHT);
	}
	if(keys[SDL_SCANCODE_A])
	{
		if(player2->GetRect()->x > SCREEN_W / 2)
			player2->Move(LEFT);
	}
	if(keys[SDL_SCANCODE_LCTRL])
	{
		player2->Shoot();
	}

	if (keys[SDL_SCANCODE_ESCAPE]) {
		exit(0);
	}

	// Move all Objects that should move
	player1->MoveBullets(BULLETSPEED);
	player2->MoveBullets(BULLETSPEED);
	// Test Colls
	TestColl();
	// Redraw the Background

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, bg, nullptr, nullptr);

	// Draws all Objects that should draw
	player1->DrawBullets();
	player2->DrawBullets();
	// Draw players
	player1->Draw();
	player2->Draw();

	SDL_RenderPresent(renderer);

	// Wait, that the game won't become faster as 24fps
	while(SDL_GetTicks() - lframe < 40);

	if(player1->GetHealth() == 0)
	{
		SDL_Delay(3000);
		exit(0);
	}
	if(player2->GetHealth() == 0)
	{
		SDL_Delay(3000);
		exit(0);
	}
}
// Look if the 2 given rectangles touch each other
bool Coll(SDL_Rect *rect1, SDL_Rect *rect2)
{
	int left1 = rect1->x;
    int left2 = rect2->x;
    int right1 = rect1->x + rect1->w;
    int right2 = rect2->x + rect2->w;
    int top1 = rect1->y;
    int top2 = rect2->y;
    int bottom1 = rect1->y + rect1->h;
    int bottom2 = rect2->y + rect2->h;

	return !((bottom1 < top2) || (top1 > bottom2) || (right1 < left2) || (left1 > right2));
}
// React on Collision
void TestColl()
{
    int i;
	// Check if a bullet touch another
	for(int a=0; a<BULLETS; a++)
	{
		for(int b=0; b<BULLETS; b++)
		{
			if(player1->BulletVisible(a) && player2->BulletVisible(b))
			{
				if(Coll(player1->GetBulletRect(a), player2->GetBulletRect(b)))
				{
					player1->InvisibleBullet(a);
					player2->InvisibleBullet(b);
				}
			}
		}
	}
	// Check if a bullet from p1 touch p2
	for(i=0; i<BULLETS; i++)
	{
		if(player1->BulletVisible(i))
		{
			if(Coll(player2->GetRect(), player1->GetBulletRect(i)))
			{
				player2->Hurt();
				player1->InvisibleBullet(i);
			}
		}
	}
	// Check if a bullet from p2 touch p1
	for(i=0; i<BULLETS; i++)
	{
		if(player2->BulletVisible(i))
		{
			if(Coll(player1->GetRect(), player2->GetBulletRect(i)))
			{
				player1->Hurt();
				player2->InvisibleBullet(i);
			}
		}
	}
	// Check if a bullet from p1 goes out of the screen
	SDL_Rect *bulletrect;
	for(i=0; i<BULLETS; i++)
	{
		if(player1->BulletVisible(i))
		{
			bulletrect = player1->GetBulletRect(i);
			if(bulletrect->x + bulletrect->w > SCREEN_W)
				player1->InvisibleBullet(i);
		}
	}
	// Check if a bullet from p2 goes out of the screen
	for(i=0; i<BULLETS; i++)
	{
		if(player2->BulletVisible(i))
		{
			bulletrect = player2->GetBulletRect(i);
			if(bulletrect->x <= 0)
				player2->InvisibleBullet(i);
		}
	}
}