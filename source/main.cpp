#if defined(_WIN32) || (_WIN64)

	#include "SDL.h"

#endif

#if defined(__APPLE__)

	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"

#endif

#if defined(__linux__)

	#include "SDL2/SDL.h"

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
	if(bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y = -768;
		b1pos_Y = bkgd1Pos.y;
	}

	// Update background 2
	b2pos_Y += (bkgdSpeed * 1) * deltaTime;

	// Set the new bkgd1 position
	bkgd2Pos.y = (int)(b2pos_Y + 0.5f);

	// Reset when off the bottom of the screen
	if(bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y = -768;
		b2pos_Y = bkgd2Pos.y;
	}
}





int main(int argc, char* argv[])
{
#if defined(_WIN32) || (_WIN64)

	cout << "Running on Windows..." << endl;
	cout << "Added on Windows..." << endl;

#endif


#if defined(__APPLE__)

	cout << "Running on Apple..." << endl;
	cout << "Added on Apple..." << endl;

	// get the current working directory
	string cwd(getcwd(NULL,0));

	// create a string linking to the mac's images folder
	string images_dir = cwd + "/Resources/images/";

	// test
	cout << images_dir << endl;

#endif


#if defined(__linux__)

	cout << "Running on Linux..." << endl;
	cout << "Added on Linux..." << endl;

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
		SDL_Surface *surface = IMG_Load((images_dir + "bg-temp.png").c_str());

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


//		// ***** Title ***** //
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *title;
//		title = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		SDL_Rect titlePos;
//
//		titlePos.x = 0;
//		titlePos.y = 0;
//		titlePos.w = 1024;
//		titlePos.h = 768;
//
//
//		// ***** Players1 ***** //
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *players1N;
//		players1N = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *players1O;
//		players1O = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		SDL_Rect players1NPos;
//
//		players1NPos.x = 0;
//		players1NPos.y = 0;
//		players1NPos.w = 1024;
//		players1NPos.h = 768;
//
//
//		// ***** Players2 ***** //
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *players2N;
//		players2N = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *players2O;
//		players2O = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		SDL_Rect players2NPos;
//
//		players2NPos.x = 0;
//		players2NPos.y = 0;
//		players2NPos.w = 1024;
//		players2NPos.h = 768;
//
//
//		// ***** Instructions ***** //
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *instructionsN;
//		instructionsN = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *instructionsO;
//		instructionsO = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		SDL_Rect instructionsNPos;
//
//		instructionsNPos.x = 0;
//		instructionsNPos.y = 0;
//		instructionsNPos.w = 1024;
//		instructionsNPos.h = 768;
//
//
//		// ***** Quit ***** //
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *quitN;
//		quitN = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		//surface = IMG_Load((images_dir + "bg-temp.png").c_str());
//		SDL_Texture *quitO;
//		quitO = SDL_CreateTextureFromSurface(renderer, surface);
//		SDL_FreeSurface(surface);
//
//		SDL_Rect quitNPos;
//
//		quitNPos.x = 0;
//		quitNPos.y = 0;
//		quitNPos.w = 1024;
//		quitNPos.h = 768;



	/////////////////////////////////////////
	//        ***** Create Cursor *****
	/////////////////////////////////////////
	surface = IMG_Load((images_dir + "cursor.png").c_str());

	SDL_Texture *cursor;

	cursor = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect cursorPos, activePos;

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

	// cursor speed
	int cursorSpeed = 400;



	/*
	SDL_Surface* screenSurface = NULL;

	screenSurface = SDL_GetWindowSurface(window);

	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 42, 254));

	SDL_UpdateWindowSurface(window);
	*/



	/////////////////////////////////////////
	//     ***** Create Controller *****
	/////////////////////////////////////////

	// Set up a game controller variable
	SDL_GameController* gGameController = NULL;

	// Open game controller
	gGameController = SDL_GameControllerOpen(0);

	// Turn on game controller events
	SDL_GameControllerEventState(SDL_ENABLE);

	// SDL event to handle input
	SDL_Event event;

	// Set up variables for the game states
	enum GameState{MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};

	// Set up the initial state
	GameState gameState = MENU;

	// Boolean values to control movement through the states
	bool menu, instructions, players1, players2, win, lose, quit;
	quit = false;





	// The window is open: could enter program loop here (see SDL_PollEvent())

	while(!quit)
	{
		switch(gameState)
		{
			case MENU:
				menu = true;
				cout << "The game state is Menu" << endl;
				cout << "Press the A Button for Instructions" << endl;
				cout << "Press the B Button for 1 Player Game" << endl;
				cout << "Press the X Button for 2 Player Game" << endl;
				cout << "Press the Y Button for Quit Game" << endl;
				cout << endl;

				while(menu)
				{
					// Set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;


					// Check for input events
					if(SDL_PollEvent(&event))
					{
						// Check to see if the SDL window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							menu = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										menu = false;
										gameState = INSTRUCTIONS;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										menu = false;
										gameState = PLAYERS1;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
									{
										menu = false;
										gameState = PLAYERS2;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
									{
										menu = false;
										quit = true;
									}
								}
								break;
						}

					}


					// Update
					UpdateBackground();



					// Start drawing
					// Clear SDL renderer
					SDL_RenderClear(renderer);

					// Draw the background image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					// Draw cursor
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					// Draw cursor
					//SDL_RenderCopy(renderer, title, NULL, &titlePos);

					// SDL render present
					SDL_RenderPresent(renderer);

				}


				break;	// end main menu case





			case INSTRUCTIONS:
				instructions = true;
				cout << "The game state is Instructions" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << endl;

				while(instructions)
				{
					// Set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;


					// Check for input events
					if(SDL_PollEvent(&event))
					{
						// Check to see if the SDL window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							instructions = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										instructions = false;
										gameState = MENU;
									}
								}
								break;
						}

					}
				}


				break;	// end instructions case





			case PLAYERS1:
				players1 = true;
				cout << "The game state is Players 1" << endl;
				cout << "Press the A Button for Win Screen" << endl;
				cout << "Press the B Button for Lose Screen" << endl;
				cout << endl;

				while(players1)
				{
					// Check for input events
					if(SDL_PollEvent(&event))
					{
						// Check to see if the SDL window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							players1 = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										players1 = false;
										gameState = WIN;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										players1 = false;
										gameState = LOSE;
									}
								}
								break;
						}

					}
				}


				break;	// end players1 case





			case PLAYERS2:
				players2 = true;
				cout << "The game state is Players 2" << endl;
				cout << "Press the A Button for Win Screen" << endl;
				cout << "Press the B Button for Lose Screen" << endl;
				cout << endl;

				while(players2)
				{
					// Check for input events
					if(SDL_PollEvent(&event))
					{
						// Check to see if the SDL window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							players2 = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										players2 = false;
										gameState = WIN;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										players2 = false;
										gameState = LOSE;
									}
								}
								break;
						}

					}
				}


				break;	// end players2 case





			case WIN:
				win = true;
				cout << "The game state is Win" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << "Press the B Button to Quit" << endl;
				cout << endl;

				while(win)
				{
					// Check for input events
					if(SDL_PollEvent(&event))
					{
						// Check to see if the SDL window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							win = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										win = false;
										gameState = MENU;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										win = false;
										quit = true;
									}
								}
								break;
						}

					}
				}


				break;	// end win case





			case LOSE:
				lose = true;
				cout << "The game state is Lose" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << "Press the B Button to Quit" << endl;
				cout << endl;

				while(lose)
				{
					// Check for input events
					if(SDL_PollEvent(&event))
					{
						// Check to see if the SDL window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							lose = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0)
								{
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
									{
										lose = false;
										gameState = MENU;
									}

									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
									{
										lose = false;
										quit = true;
									}
								}
								break;
						}

					}
				}


				break;	// end lose case
		}
	}










	//SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

					  	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
