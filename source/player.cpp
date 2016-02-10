#include "player.h"



// Analog joystick deadzone
const int JOYSTICK_DEAD_ZONE = 8000;

// Player creation method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{
	// Set the player number 0 or 1
	playerNum = pNum;

	// Set float for player speed
	speed = 500.0f;

	laser = Mix_LoadWAV((audioPath + "laser.wav").c_str());

	// Initialize score and lives variables
	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 3;

	// Initialize the font system
	TTF_Init();

	// Load the font
	font = TTF_OpenFont((audioPath + "The Curious Cat.ttf").c_str(), 40);

	// X and Y locations for scores
	if(playerNum == 0)
	{
		scorePos.x = scorePos.y = 10;
		livesPos.x = 10;
		livesPos.y = 40;
	} else {
		scorePos.x = 650;
		scorePos.y = 10;
		livesPos.x = 650;
		livesPos.y = 40;
	}

	// Update score method
	UpdateScore(renderer);

	// Update lives method
	UpdateLives(renderer);


	// See if this is player 1 or player 2 and create the correct file path
	if(playerNum == 0)
	{
		// Create the player 1 texture
		playerPath = filePath + "player1.png";
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


void Player::CreateBullet()
{
	// see if there is a bullet active to fire
	for (int i = 0; i < bulletList.size(); i++)
	{
		//see if the bullet is not active
		if (bulletList[i].active == false)
		{
			// play the laser sound
			Mix_PlayChannel(-1, laser, 0);

			//set bullet to active
			bulletList[i].active = true;

			// use some math in the x position to get the bullet close
			// to the center of the player using width
			bulletList[i].posRect.x = (pos_X + (posRect.w / 2));

			// finish aligning to the player center using the texture width
			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w / 2));
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
	if (event.which == 0 && playerNum == 0)
	{
		if (event.button == 0)
		{
			// Test
			playerScore += 10;
			CreateBullet();
		}
	}


	if (event.which == 1 && playerNum == 1)
	{
		if (event.button == 0)
		{
			CreateBullet();
		}
	}
}


void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	if (event.which == 0 && playerNum == 0)
	{
		if (event.axis == 0)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				xDir = 1.0f;
			}
			else {
				xDir = 0.0f;
			}
		}

		if (event.axis == 1)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			}
			else {
				yDir = 0.0f;
			}
		}
	}
}


// Update lives
void Player::UpdateLives(SDL_Renderer *renderer)
{
	// Fix for to_string problems on linux
	string Result;
	ostringstream convert;
	convert << playerLives;
	Result = convert.str();

	// Create the text for the font texture
	tempLives = "Player Lives: " + Result;

	// Check to see what player this is and color the font as needed
	if(playerNum == 0)
	{
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);
	} else {
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP2);
	}

	// Create the player score texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	// Get the width and height of the texture
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	SDL_FreeSurface(scoreSurface);

	// Set old score
	oldScore = playerScore;
}


// Update score
void Player::UpdateScore(SDL_Renderer *renderer)
{
	// Fix for to_string problems on linux
	string Result;
	ostringstream convert;
	convert << playerScore;
	Result = convert.str();

	// Create the text for the font texture
	tempScore = "Player Score: " + Result;

	// Check to see what player this is and color the font as needed
	if(playerNum == 0)
	{
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);
	} else {
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP2);
	}

	// Create the player score texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	// Get the width and height of the texture
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	SDL_FreeSurface(scoreSurface);

	// Set old score
	oldScore = playerScore;
}


void Player::Update(float deltaTime, SDL_Renderer *renderer)
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

	if(posRect.x > 1024 - posRect.w) {
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y < 0) {
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h) {
		posRect.y = 768 - posRect.h;
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

	// Should score be updated?
	if(playerScore != oldScore)
	{
		UpdateScore(renderer);
	}

	// Should lives be updated?
	if(playerLives != oldLives)
	{
		UpdateLives(renderer);
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
			// Update the bullet
			bulletList[i].Draw(renderer);
		}
	}

	// Draw the player score
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

	// Draw the player lives
	SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos)
}


Player::~Player()
{
	SDL_DestroyTexture(texture);
}
