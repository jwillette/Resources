#include "player.h"



// Analog joystick deadzone
const int JOYSTICK_DEAD_ZONE = 8000;

// Player creation method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y)
{
	// Set the player number 0 or 1
	playerNum = pNum;

	// Set float for player speed
	speed = 500.0f;

	// See if this is player 1 or player 2 and create the correct file path
	if(playerNum == 0)
	{
		// Create the player 1 texture
		playerPath = filePath + "player.png";
	} else {
		playerPath = filePath + "player2.png";
	}

	// Load the surface
	surface = IMG_Load(playerPath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// Set the SDL_Rect X and Y for the player
	posRect.x = x;
	posRect.y = y;

	// Use SDL_QueryTexture to get the W and H of the player's texture
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	// Set the movement floats to the player's original X and Y
	pos_X = x;
	pos_Y = y;

	// Set the xDir and yDir for the joysticks
	xDir = 0;
	yDir = 0;



	// String to create the path to the player's bullet image
	string bulletPath;

	if(playerNum == 0)
	{
		// Create the player 1 texture
		bulletPath = filePath + "bullet.png";
	} else {
		bulletPath = filePath + "bullet2.png";
	}

	// Create the bullet pool
	for(int i = 0; i < 10; i++)
	{
		// create  the bullet and move offscreen, out of the game play area
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000);

		// add to bulletList
		bulletList.push_back(tmpBullet);
	}
}


void Player::Update(float deltaTime)
{
	// Adjust position floats based on speed, direction of joystick axis, and deltaTime
	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;

	// Update player position with code to account for precision loss
	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if(posRect.x < 0) {
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024) {
		posRect.x = 1024;
		pos_X = posRect.x;
	}

	if(posRect.y < 0) {
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768) {
		posRect.y = 768;
		pos_Y = posRect.y;
	}


	// Update the player's bullets
	for(int i = 0; i < bulletList.size(); i++)
	{
		// Check to see if the bullet is active
		if(bulletList[i].active)
		{
			// Update the bullet
			bulletList[i].Update(deltaTime);
		}
	}
}


// Player draw method
void Player::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

	// Draw the player's bullets
	for(int i = 0; i < bulletList.size(); i++)
	{
		// Check to see if the bullet is active
		if(bulletList[i].active)
		{
			//// Update the bullet
			//bulletList[i].Draw(renderer);
		}
	}
}


void Player::CreateBullet()
{
	// see if there is a bullet active to fire
	for(int i = 0; i < bulletList.size(); i++)
	{
		//see if the bullet is not active
		if(bulletList[i].active == false)
		{
			//set bullet to active
			bulletList[i].active = true;

			// use some math in the x position to get the bullet close
			// to the center of the player using width
			bulletList[i].posRect.x = (pos_X + (posRect.w/2));

			// finish aligning to the player center using the texture width
			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w/2));
			bulletList[i].posRect.y = posRect.y;

			// set the x and y positions of the bullet's float positions
			bulletList[i].pos_X = pos_X;
			bulletList[i].pos_Y = pos_Y;

			// once bullet is found, break out of loop
			break;
		}
	}
}


void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if(event.which == 0 && playerNum == 0)
	{
		if(event.button == 0)
		{
			cout << "Player 1 - Button A" << endl;

			//CreateBullet();
		}
	}


	if(event.which == 1 && playerNum == 1)
	{
		if(event.button == 0)
		{
			cout << "Player 2 - Button A" << endl;

			//CreateBullet();
		}
	}
}


void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	if(event.which == 0 && playerNum == 0)
	{
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;
			} else if (event.value > JOYSTICK_DEAD_ZONE) {
				xDir = 1.0f;
			} else {
				xDir = 0.0f;
			}
		}

		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;
			} else if(event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			} else {
				yDir = 0.0f;
			}
		}
	}
}
