#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"

#endif


#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

#endif


#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#endif



#include <string>
#include <iostream>
using namespace std;

#include <stdlib.h>



class Enemy
{
public:

	// Boolean for the state of the enemy
	bool active;

	// Enemy texture
	SDL_Texture *texture;

	// Enemy rectangle for position and size
	SDL_Rect posRect;

	// Movement directions
	float xDir, yDir;

	// Enemy speed
	float speed;

	// Enemy float positions to prevent precision loss
	float pos_X, pos_Y;

	// Enemy angle
	float angle;

	// Point for the center of an object
	SDL_Point center;

	// Enemy creation method
	Enemy(SDL_Renderer *renderer, string filePath);

	// Enemy update
	void Update(float deltaTime);

	// Enemy draw
	void Draw(SDL_Renderer *renderer);

	// Enemy reset
	void Reset();

	// Enemy destruction
	~Enemy();
};
