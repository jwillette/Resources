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





class Player
{
public:

	// string to hold the path to the player's texture
	string playerPath;

	// integer for the player's number
	int playerNum;

	// player's surface to load png into
	SDL_Surface *surface;

	// player's texture to hold img
	SDL_Texture *texture;

	// player's SDL_Rect for the x, y, w, and h of texture
	SDL_Rect posRect;

	// floats to hold the joystick axis (-1, 0, or 1)
	float xDir;
	float yDir;

	// float for player's speed
	float speed;

	// float for the player's position to avoid precision loss
	float pos_X, pos_Y;



	// Player's creation method using passed in values for renderer, player number, and path to texture
	// starting position X, starting position Y
	Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y);

	// Method to allow the player to move via joystick axis
	void OnControllerAxis(const SDL_ControllerAxisEvent event);

	// Method to allow the player to interact with joystick buttons
	void OnControllerButton(const SDL_ControllerButtonEvent event);

	// Update the player using the passed in deltaTime
	void Update(float deltaTime);

	// Draw the player main's passed in renderer
	void Draw(SDL_Renderer *renderer);

	// Player's destruction method
	~Player();
};
