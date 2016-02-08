#include "bullet.h"

// Bullet creation method
Bullet::Bullet(SDL_Renderer *renderer, string filePath, float x, float y)
{
	// Set the bullet initial state
	active = false;

	// Set bullet speed
	speed = 800.0f;

	// Load the texture into a surface
	SDL_Surface *surface = IMG_Load(filePath.c_str());

	// Create the texture from the passed renderer and created surface
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	// Free the surface for later use
	SDL_FreeSurface(surface);

	// Set the width and height of the bullet's rectangle by querying the texture
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	// Set the x and y positions of the bullet rectangle
	posRect.x = x - (posRect.w/2);
	posRect.y = y;

	// Set the float position values of the texture for precision loss
	pos_X = x;
	pos_Y = y;

	// Initialize the direction variables for the bullet
	xDir = 0;
	yDir = -1;
}



// Bullet update
void Bullet::Update(float deltaTime)
{
	// check to see if the bullet is active
	if(active)
	{
		// Adjust position floats based on speed, direction (-1 for up), and deltaTime
		pos_Y += (speed * yDir) * deltaTime;

		// Update bullet position with code to account for precision loss
		posRect.y = (int)(pos_Y + 0.5f);

		// Check to see if the bullet is off the top of the screen
		if(posRect.y < (0 - posRect.h))
		{
			posRect.x = -1000;
			pos_X = posRect.x;
			active = false;
		}
	}
}



// Bullet draw method
void Bullet::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}



// Bullet destruction
Bullet::~Bullet()
{
	//SDL_DestroyTexture(texture);
}
