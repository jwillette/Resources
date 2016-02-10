#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#endif

#if defined(_WIN32) || (_WIN64)

#include <direct.h>
#define getcwd _getcwd

#endif


#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"

#endif


#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#endif

#if defined(__linux__)

#include <unistd.h>

#endif





#include <stdio.h>
#include <iostream>

using namespace std;



// variables for background movement and position
int bkgdSpeed = 100;

SDL_Rect bkgd1Pos, bkgd2Pos;		// rectangle for background texture

float b1pos_X = 0, b1pos_Y = 0;		// float variables for background movement
float b2pos_X = 0, b2pos_Y = -768;


// code for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;


// move the background
void UpdateBackground()
{
	// Update background 1
	b1pos_Y += (bkgdSpeed * 1) * deltaTime;

	// Set the new bkgd1 position
	bkgd1Pos.y = (int)(b1pos_Y + 0.5f);

	// Reset when off the bottom of the screen
	if (bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y = -768;
		b1pos_Y = bkgd1Pos.y;
	}

	// Update background 2
	b2pos_Y += (bkgdSpeed * 1) * deltaTime;

	// Set the new bkgd1 position
	bkgd2Pos.y = (int)(b2pos_Y + 0.5f);

	// Reset when off the bottom of the screen
	if (bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y = -768;
		b2pos_Y = bkgd2Pos.y;
	}
}


// new joystick variables
// analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

// joystick directions
float xDir, yDir;

// cursor float variables for movement
float pos_X, pos_Y;

SDL_Rect cursorPos, activePos;

int cursorSpeed = 400;


// move cursor function
void moveCursor(const SDL_ControllerAxisEvent event)
{
	if(event.which == 0)
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


// update cursor onscreen
void UpdateCursor(float deltaTime)
{
	// float values to update cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	// assign to SDL_Rect ints X and Y
	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_Y + 0.5f);

	// update active position of cursor - collision box
	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	// off the screen in X
	if(cursorPos.x < 0)
	{
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if(cursorPos.x > 1024 - cursorPos.w)
	{
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	// off the screen in Y
	if(cursorPos.y < 0)
	{
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.h)
	{
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}


// variables for the menu button over states
bool players1Over = false, players2Over = false, instructionsOver = false, quitOver = false,
		menuOver = false, playOver = false;

// class header
#include "player.h"
#include "enemy.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

// variable to hold the list of enemies
vector<Enemy> enemyList;





int main(int argc, char* argv[])
{
	srand(time(NULL));

#if defined(_WIN32) || (_WIN64)

	cout << "Running on Windows..." << endl;
	cout << "Added on Windows..." << endl;

	string cwd(getcwd(NULL, 0));

	string images_dir = cwd + "\\Resources\\images\\";

	string audio_dir = cwd + "\\Resources\\audio\\";

#endif


#if defined(__APPLE__)

	cout << "Running on Apple..." << endl;
	cout << "Added on Apple..." << endl;

	// get the current working directory
	string cwd(getcwd(NULL, 0));

	// create a string linking to the mac's images folder
	string images_dir = cwd + "/Resources/images/";

	// create a string to link to the audio folder
	string audio_dir = cwd + "/Resources/audio/";

#endif


#if defined(__linux__)

	cout << "Running on Linux..." << endl;
	cout << "Added on Linux..." << endl;

	string cwd(getcwd(NULL, 0));

	string images_dir = cwd + "/Resources/images/";

	string audio_dir = cwd + "/Resources/audio/";

#endif



	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_EVERYTHING);         // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
			"An SDL2 window",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1024,                               // width, in pixels
			768,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}



	// Declare the renderer
	SDL_Renderer *renderer = NULL;

	// Create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	/////////////////////////////////////////
	//     ***** Create Background *****
	/////////////////////////////////////////

	// Create an SDL surface to hold the background image
	SDL_Surface *surface = IMG_Load((images_dir + "bg2.png").c_str());

	// Create an SDL texture
	SDL_Texture *bkgd1;

	// Place surface into the texture bkgd1
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	// Create a second SDL texture
	SDL_Texture *bkgd2;
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	// Free the SDL surface
	SDL_FreeSurface(surface);


	// Set the x, y, w, and h for the rectangles
	bkgd1Pos.x = 0;
	bkgd1Pos.y = 0;
	bkgd1Pos.w = 1024;
	bkgd1Pos.h = 768;

	bkgd2Pos.x = 0;
	bkgd2Pos.y = -768;
	bkgd2Pos.w = 1024;
	bkgd2Pos.h = 768;



	/////////////////////////////////////////
	//        ***** Create Menu *****
	/////////////////////////////////////////


	// ***** Title ***** //

	surface = IMG_Load((images_dir + "chatnoire-title.png").c_str());
	SDL_Texture *title;
	title = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect titlePos;

	titlePos.x = 390;
	titlePos.y = 30;
	titlePos.w = 244;
	titlePos.h = 178;


	// ***** Players1 ***** //

	surface = IMG_Load((images_dir + "1player.png").c_str());
	SDL_Texture *players1N;
	players1N = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "1player-over.png").c_str());
	SDL_Texture *players1O;
	players1O = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect players1NPos;

	players1NPos.x = 420;
	players1NPos.y = 440;
	players1NPos.w = 195;
	players1NPos.h = 47;


	// ***** Players2 ***** //

	surface = IMG_Load((images_dir + "2player.png").c_str());
	SDL_Texture *players2N;
	players2N = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "2player-over.png").c_str());
	SDL_Texture *players2O;
	players2O = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect players2NPos;

	players2NPos.x = 400;
	players2NPos.y = 540;
	players2NPos.w = 229;
	players2NPos.h = 47;


	// ***** Instructions ***** //

	surface = IMG_Load((images_dir + "instructions.png").c_str());
	SDL_Texture *instructionsN;
	instructionsN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "instructions-over.png").c_str());
	SDL_Texture *instructionsO;
	instructionsO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect instructionsNPos;

	instructionsNPos.x = 390;
	instructionsNPos.y = 340;
	instructionsNPos.w = 262;
	instructionsNPos.h = 47;


	// ***** Quit ***** //

	surface = IMG_Load((images_dir + "quit.png").c_str());
	SDL_Texture *quitN;
	quitN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "quit-over.png").c_str());
	SDL_Texture *quitO;
	quitO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect quitNPos;

	quitNPos.x = 475;
	quitNPos.y = 640;
	quitNPos.w = 92;
	quitNPos.h = 60;



	//////////////////////////////////////////////////
	//        ***** Create Instructions *****
	//////////////////////////////////////////////////


	// ***** Instructions ***** //

	surface = IMG_Load((images_dir + "instructions-text.png").c_str());
	SDL_Texture *instructionsText;
	instructionsText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect instructionsTextPos;

	instructionsTextPos.x = 250;
	instructionsTextPos.y = 300;
	instructionsTextPos.w = 529;
	instructionsTextPos.h = 287;


	// ***** Main Menu ***** //

	surface = IMG_Load((images_dir + "menu.png").c_str());
	SDL_Texture *menuN;
	menuN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "menu-over.png").c_str());
	SDL_Texture *menuO;
	menuO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect menuNPos;

	menuNPos.x = 10;
	menuNPos.y = 700;
	menuNPos.w = 203;
	menuNPos.h = 47;



	///////////////////////////////////////////
	//        ***** Create Win *****
	///////////////////////////////////////////


	// ***** Win ***** //

	surface = IMG_Load((images_dir + "win-text.png").c_str());
	SDL_Texture *winText;
	winText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect winTextPos;

	winTextPos.x = 390;
	winTextPos.y = 30;
	winTextPos.w = 215;
	winTextPos.h = 178;


	// ***** Play Again ***** //

	surface = IMG_Load((images_dir + "play.png").c_str());
	SDL_Texture *playN;
	playN = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "play-over.png").c_str());
	SDL_Texture *playO;
	playO = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect playNPos;

	playNPos.x = 810;
	playNPos.y = 700;
	playNPos.w = 193;
	playNPos.h = 47;



	///////////////////////////////////////////
	//        ***** Create Lose *****
	///////////////////////////////////////////


	// ***** Lose ***** //

	surface = IMG_Load((images_dir + "lose-text.png").c_str());
	SDL_Texture *loseText;
	loseText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect loseTextPos;

	loseTextPos.x = 390;
	loseTextPos.y = 30;
	loseTextPos.w = 239;
	loseTextPos.h = 190;



	/////////////////////////////////////////
	//        ***** Create Cursor *****
	/////////////////////////////////////////
	surface = IMG_Load((images_dir + "cursor.png").c_str());

	SDL_Texture *cursor;

	cursor = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	//SDL_Rect cursorPos, activePos;

	// rectangle for position for cursor's graphic
	cursorPos.x = 0;
	cursorPos.y = 0;
	cursorPos.w = 32;
	cursorPos.h = 32;

	// rectangle for position for cursor's collision detection
	activePos.x = 10;
	activePos.y = 10;
	activePos.w = 10;
	activePos.h = 10;



	/*
	SDL_Surface* screenSurface = NULL;

	screenSurface = SDL_GetWindowSurface(window);

	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 42, 254));

	SDL_UpdateWindowSurface(window);
	 */



	/////////////////////////////////////////
	//     ***** Create Controller *****
	/////////////////////////////////////////

	// Turn on game controller events
	SDL_GameControllerEventState(SDL_ENABLE);

	// Set up a game controller variable
	SDL_GameController* gGameController0 = NULL;

	// Open game controller
	gGameController0 = SDL_GameControllerOpen(0);

	// Set up a second game controller
	SDL_GameController* gGameController1 = NULL;
	gGameController1 = SDL_GameControllerOpen(1);


	// SDL event to handle input
	SDL_Event event;

	// Set up variables for the game states
	enum GameState { MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE };

	// Set up the initial state
	GameState gameState = MENU;

	// Boolean values to control movement through the states
	bool menu, instructions, players1, players2, win, lose, quit;
	quit = false;



	/////////////////////////////////////////
	//           ***** Audio *****
	/////////////////////////////////////////

	// Open audio channel
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// Load a music file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "Disco-Ants-Go-Clubbin.mp3").c_str());

	// If the music file is not playing, play it
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	// Set up a Sound Effect chunk for the button over state
	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	// Set up a Sound Effect chunk for the button over state
	Mix_Chunk *pressedSound = Mix_LoadWAV((audio_dir + "pressed.wav").c_str());

	// Bool value to control the over sound effect and the buttons
	bool alreadyOver = false;



	// Create the players
	Player player1 = Player(renderer, 0, images_dir.c_str(), audio_dir.c_str(), 250.0, 500.0);
	Player player2 = Player(renderer, 1, images_dir.c_str(), audio_dir.c_str(), 750.0, 500.0);





	// The window is open: could enter program loop here (see SDL_PollEvent())

	while (!quit)
	{
		switch (gameState)
		{
		case MENU:
			alreadyOver = false;
			menu = true;

			while (menu)
			{
				// Set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;


				// Check for input events
				if (SDL_PollEvent(&event))
				{
					// Check to see if the SDL window is closed - player clicks X in window
					if (event.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(players1Over)
								{
									// Play the over sound - plays fine through levels, must pause for quit
									Mix_PlayChannel(-1, pressedSound, 0);

									menu = false;
									gameState = PLAYERS1;
									players1Over = false;
								}

								if(players2Over)
								{
									Mix_PlayChannel(-1, pressedSound, 0);

									menu = false;
									gameState = PLAYERS2;
									players2Over = false;
								}

								if(instructionsOver)
								{
									Mix_PlayChannel(-1, pressedSound, 0);

									menu = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

								if(quitOver)
								{
									Mix_PlayChannel(-1, pressedSound, 0);
									SDL_Delay(1000);

									menu = false;
									quit = true;
									quitOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;

					}
				}


				// Update
				UpdateBackground();

				// Update cursor
				UpdateCursor(deltaTime);



				// Check for collision between cursor active state and buttons
				players1Over = SDL_HasIntersection(&activePos, &players1NPos);
				players2Over = SDL_HasIntersection(&activePos, &players2NPos);
				instructionsOver = SDL_HasIntersection(&activePos, &instructionsNPos);
				quitOver = SDL_HasIntersection(&activePos, &quitNPos);



				// If the cursor is over a button, play the over sound
				if(players1Over || players2Over || instructionsOver || quitOver)
				{
					if(alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				// If the cursor is not over any button, reset the alreadyOver variable
				if(!players1Over && !players2Over && !instructionsOver && !quitOver)
				{
					alreadyOver = false;
				}



				// Start drawing
				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the background image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw title
				SDL_RenderCopy(renderer, title, NULL, &titlePos);


				// Draw 1player
				if(players1Over)
				{
					SDL_RenderCopy(renderer, players1O, NULL, &players1NPos);
				} else {
					SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);
				}

				// Draw 2player
				if(players2Over)
				{
					SDL_RenderCopy(renderer, players2O, NULL, &players2NPos);
				} else {
					SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);
				}

				// Draw instructions
				if(instructionsOver)
				{
					SDL_RenderCopy(renderer, instructionsO, NULL, &instructionsNPos);
				} else {
					SDL_RenderCopy(renderer, instructionsN, NULL, &instructionsNPos);
				}

				// Draw quit
				if(quitOver)
				{
					SDL_RenderCopy(renderer, quitO, NULL, &quitNPos);
				} else {
					SDL_RenderCopy(renderer, quitN, NULL, &quitNPos);
				}


				// Draw cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


				// SDL render present
				SDL_RenderPresent(renderer);
			}


			break;	// end main menu case





		case INSTRUCTIONS:
			alreadyOver = false;
			instructions = true;

			while (instructions)
			{
				// Set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;


				// Check for input events
				if (SDL_PollEvent(&event))
				{
					// Check to see if the SDL window is closed - player clicks X in window
					if (event.type == SDL_QUIT)
					{
						quit = true;
						instructions = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									instructions = false;
									gameState = MENU;
									menuOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}

				UpdateBackground();
				UpdateCursor(deltaTime);


				// Collision
				menuOver = SDL_HasIntersection(&activePos, &menuNPos);


				if(menuOver)
				{
					if(alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if(!menuOver)
				{
					alreadyOver = false;
				}


				// Start drawing
				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw title
				SDL_RenderCopy(renderer, title, NULL, &titlePos);

				// Draw instructions text
				SDL_RenderCopy(renderer, instructionsText, NULL, &instructionsTextPos);


				// Draw menu
				if(menuOver)
				{
					SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
				}
				else {
					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
				}


				// Draw cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


				// SDL render present
				SDL_RenderPresent(renderer);

			}
			break;	// end instructions case





		case PLAYERS1:
			alreadyOver = false;
			players1 = true;

			enemyList.clear();

			for(int i = 0; i < 6; i++)
			{
				Enemy tmpEnemy(renderer, images_dir);
				enemyList.push_back(tmpEnemy);
			}



			while (players1)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// Check for input events
				if (SDL_PollEvent(&event))
				{
					// Check to see if the SDL window is closed - player clicks X in window
					if (event.type == SDL_QUIT)
					{
						quit = true;
						players1 = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								players1 = false;
								gameState = WIN;
							}

							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								players1 = false;
								gameState = LOSE;
							}

							// send button press info to player1
							player1.OnControllerButton(event.cbutton);
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						player1.OnControllerAxis(event.caxis);
						break;
					}
				}

				UpdateBackground();

				// Update player
				player1.Update(deltaTime, renderer);



				for(int i = 0; i < enemyList.size(); i++)
				{
					enemyList[i].Update(deltaTime);
				}



				// Start drawing
				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);


				// Draw player 1
				player1.Draw(renderer);


				for(int i = 0; i < enemyList.size(); i++)
				{
					enemyList[i].Draw(renderer);
				}


				// SDL render present
				SDL_RenderPresent(renderer);

			}
			break;	// end players1 case





		case PLAYERS2:
			alreadyOver = false;
			players2 = true;

			enemyList.clear();

			for(int i = 0; i < 12; i++)
			{
				Enemy tmpEnemy(renderer, images_dir);
				enemyList.push_back(tmpEnemy);
			}



			while (players2)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						players2 = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0 || event.cdevice.which == 1)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								players2 = false;
								gameState = WIN;
							}

							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								players2 = false;
								gameState = LOSE;
							}
						}

						player1.OnControllerButton(event.cbutton);
						player2.OnControllerButton(event.cbutton);

						break;

					case SDL_CONTROLLERAXISMOTION:
						player1.OnControllerAxis(event.caxis);
						player2.OnControllerAxis(event.caxis);
						break;
					}
				}

				UpdateBackground();


				player1.Update(deltaTime, renderer);
				player2.Update(deltaTime, renderer);


				for(int i = 0; i < enemyList.size(); i++)
				{
					enemyList[i].Update(deltaTime);
				}


				// Start drawing
				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);


				for(int i = 0; i < enemyList.size(); i++)
				{
					enemyList[i].Draw(renderer);
				}


				player1.Draw(renderer);
				player2.Draw(renderer);


				// SDL render present
				SDL_RenderPresent(renderer);

			}
			break;	// end players2 case





		case WIN:
			alreadyOver = false;
			win = true;

			while (win)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// Check for input events
				if (SDL_PollEvent(&event))
				{
					// Check to see if the SDL window is closed - player clicks X in window
					if (event.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									win = false;
									gameState = MENU;
									menuOver = false;
								}

								if(playOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									win = false;
									gameState = PLAYERS1;
									menuOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;

					}
				}

				UpdateBackground();
				UpdateCursor(deltaTime);


				// collision
				menuOver = SDL_HasIntersection(&activePos, &menuNPos);
				playOver = SDL_HasIntersection(&activePos, &playNPos);


				if(menuOver || playOver)
				{
					if(alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if(!menuOver && !playOver)
				{
					alreadyOver = false;
				}


				// Start drawing
				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw win text
				SDL_RenderCopy(renderer, winText, NULL, &winTextPos);


				// Draw menu
				if(menuOver) {

					SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
				} else {

					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
				}

				// Draw play again
				if(playOver) {

					SDL_RenderCopy(renderer, playO, NULL, &playNPos);
				} else {

					SDL_RenderCopy(renderer, playN, NULL, &playNPos);
				}


				// Draw cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


				// SDL render present
				SDL_RenderPresent(renderer);

			}
			break;	// end win case





		case LOSE:
			alreadyOver = false;
			lose = true;

			while (lose)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// Check for input events
				if (SDL_PollEvent(&event))
				{
					// Check to see if the SDL window is closed - player clicks X in window
					if (event.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}

					switch (event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									lose = false;
									gameState = MENU;
									menuOver = false;
								}

								if(playOver) {
									Mix_PlayChannel(-1, pressedSound, 0);

									lose = false;
									gameState = PLAYERS1;
									menuOver = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;

					}
				}

				UpdateBackground();
				UpdateCursor(deltaTime);


				// collision
				menuOver = SDL_HasIntersection(&activePos, &menuNPos);
				playOver = SDL_HasIntersection(&activePos, &playNPos);


				if(menuOver || playOver)
				{
					if(alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if(!menuOver && !playOver)
				{
					alreadyOver = false;
				}


				// Start drawing
				SDL_RenderClear(renderer);

				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw lose text
				SDL_RenderCopy(renderer, loseText, NULL, &loseTextPos);


				// Draw menu
				if(menuOver) {

					SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
				} else {

					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
				}

				// Draw play again
				if(playOver) {

					SDL_RenderCopy(renderer, playO, NULL, &playNPos);
				} else {

					SDL_RenderCopy(renderer, playN, NULL, &playNPos);
				}


				// Draw cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


				// SDL render present
				SDL_RenderPresent(renderer);

			}
			break;	// end lose case
		}
	}



	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
