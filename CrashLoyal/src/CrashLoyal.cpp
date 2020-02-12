
#include "GameState.h"
#include "Mob_Archer.h"
#include "Mob_Swordsman.h"
#include "Point.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Waypoint.h"

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <time.h>
#include <chrono>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

//The window that houses the renderrer
SDL_Window* gWindow = NULL;
//The window renderer itself
SDL_Renderer* gRenderer = NULL;
// The font used to write the names of mobs
TTF_Font* sans;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	// init the text libraries
	if (TTF_Init() < 0) {
		printf("Text library TTF could not be Initialized correctly.\n");
	}


	// Load in the font 
	sans = TTF_OpenFont("fonts/abelregular.ttf", 36);
	if (!sans) { printf("TTF_OpenFont: %s\n", TTF_GetError()); }
	return success;
}

void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	IMG_Quit();
	SDL_Quit();
}


void drawSquare(float centerX, float centerY, float size) {
	// Draws a square at the given pixel coorinate
	SDL_Rect rect = {
		(int)(centerX - (size / 2.f)),
		(int)(centerY - (size / 2.f)),
		(int)(size),
		(int)(size)
	};
	SDL_RenderFillRect(gRenderer, &rect);
}

void drawBuilding(std::shared_ptr<Building> b) {
	switch (b->getType())
	{
	case BuildingType::NorthKing:
	case BuildingType::NorthLeftTower:
	case BuildingType::NorthRightTower:
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
		break;
	case BuildingType::SouthKing:
	case BuildingType::SouthLeftTower:
	case BuildingType::SouthRightTower:
	default:
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		break;
	}

	drawSquare(b->getPoint().x * PIXELS_PER_METER, 
			   b->getPoint().y * PIXELS_PER_METER,
			   b->GetSize() * PIXELS_PER_METER);
}

void drawMob(std::shared_ptr<Mob> m) {
	int healthToAlpha = int(((float)m->GetHealth() / (float)m->GetMaxHealth()) * 155) + 100;
	if (m->IsAttackingNorth()) { SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, healthToAlpha); }
	else { SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, healthToAlpha); }

	float centerX = m->pos.x * PIXELS_PER_METER;
	float centerY = m->pos.y * PIXELS_PER_METER;
	float squareSize = m->GetSize() * 2 * PIXELS_PER_METER;

	drawSquare(centerX, centerY, squareSize);

	SDL_Color white = {0, 0, 0, 254};
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, m->GetDisplayLetter(), white); // TODO Make this print something other than m
	if (!surfaceMessage) { printf("TTF_OpenFont: %s\n", TTF_GetError()); }
	SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
	if (!message) { printf("Error 2\n"); }
	SDL_Rect messageRect = {
		(int)(centerX - (squareSize / 2.f)),
		(int)(centerY - (squareSize / 2.f)),
		(int)squareSize,
		(int)squareSize 
	};
	SDL_RenderCopy(gRenderer, message, NULL, &messageRect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}



Point pixelToGrid(int x, int y) {
	// Given a pixel coordinate, this function returns the grid coordinate that contains the provided pixel
	// As always, (0,0) is top left

	Point result;
	result.x = fmax(0.f, x / 10.0f);
	result.y = fmax(0.f, y / 10.0f);
	return result;
}

void drawGrid(Point grid) {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	drawSquare(grid.x * PIXELS_PER_METER, grid.y * PIXELS_PER_METER, PIXELS_PER_METER);
}

void processClick(int x, int y, bool leftClick)
{
	static const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	const Point pos(x / (float)PIXELS_PER_METER, y / (float)PIXELS_PER_METER);
	std::shared_ptr<Mob> m =
		keyboardState[SDL_SCANCODE_LSHIFT]			// if left-shift is down
		? std::shared_ptr<Mob>(new Mob_Archer)
		: std::shared_ptr<Mob>(new Mob_Swordsman);
	m->Init(pos, leftClick);
	GameState::mobs.push_back(m);
}

void drawBG() {
	SDL_Rect bgRect = {
		0,
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	};
	SDL_SetRenderDrawColor(gRenderer, 79, 161, 0, 0xFF); // Dark green
	SDL_RenderFillRect(gRenderer, &bgRect);
	int upshift = 5;
	// Draw the river
	SDL_Rect riverRect = {
		(int)(RIVER_LEFT_X * PIXELS_PER_METER),
		(int)(RIVER_TOP_Y * PIXELS_PER_METER),
		(int)(SCREEN_WIDTH * PIXELS_PER_METER),
		(int)((RIVER_BOT_Y - RIVER_TOP_Y) * PIXELS_PER_METER)
	};
	SDL_SetRenderDrawColor(gRenderer, 51, 119, 255, 0xFF); // Light blue
	SDL_RenderFillRect(gRenderer, &riverRect);

	// Draw bridges
	SDL_Rect bridgeLeft = {
		(int)(LEFT_BRIDGE_CENTER_X - (BRIDGE_WIDTH / 2.0)) * PIXELS_PER_METER,
		(int)(LEFT_BRIDGE_CENTER_Y - (BRIDGE_HEIGHT / 2.0)) * PIXELS_PER_METER,
		(int)(BRIDGE_WIDTH * PIXELS_PER_METER),
		(int)(BRIDGE_HEIGHT * PIXELS_PER_METER)
	};
	SDL_SetRenderDrawColor(gRenderer, 179, 59, 0, 0xFF); // Brown
	SDL_RenderFillRect(gRenderer, &bridgeLeft);


	SDL_Rect bridgeRight = {
		(int)(RIGHT_BRIDGE_CENTER_X - (BRIDGE_WIDTH / 2.0)) * PIXELS_PER_METER,
		(int)(RIGHT_BRIDGE_CENTER_Y - (BRIDGE_HEIGHT / 2.0)) * PIXELS_PER_METER,
		(int)(BRIDGE_WIDTH * PIXELS_PER_METER),
		(int)(BRIDGE_HEIGHT * PIXELS_PER_METER)
	};
	SDL_SetRenderDrawColor(gRenderer, 179, 59, 0, 0xFF); // Brown
	SDL_RenderFillRect(gRenderer, &bridgeRight);
}

int main(int argc, char* args[]) {
	//Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		// Number of frames since start of application
		int frame = 0;

		// Time at the start of the world, used to calculate the time between update cycles
		auto previousTime = std::chrono::high_resolution_clock::now();
		auto now = std::chrono::high_resolution_clock::now();

		//While application is running
		while (!quit) {


			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			drawBG();

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0) {
				//User requests quit
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				if (e.type == SDL_MOUSEBUTTONUP) {
					const SDL_MouseButtonEvent& mouse_event = e.button;
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (mouse_event.button == SDL_BUTTON_RIGHT) { processClick(x, y, false); }
					else if (mouse_event.button == SDL_BUTTON_LEFT) { processClick(x, y, true); }
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					int pixelX, pixelY;
					SDL_GetMouseState(&pixelX, &pixelY);
					drawGrid(pixelToGrid(pixelX, pixelY));
				}
			}

			// Draw waypoints
			// TODO remove this
			for (std::shared_ptr<Waypoint> wp : GameState::waypoints)
			{
				drawSquare(wp->pos.x * PIXELS_PER_METER, 
						   wp->pos.y * PIXELS_PER_METER, 
						   WAYPOINT_SIZE * PIXELS_PER_METER);
			}

			now = std::chrono::high_resolution_clock::now();;
			double deltaTSec = (std::chrono::duration_cast<std::chrono::duration<double>>(now - previousTime)).count() * 10;
			previousTime = now;

			// Draw and update Buildings
			for (std::shared_ptr<Building> b : GameState::buildings) {
				if (!b->isDead()) {
					b->update(deltaTSec);
					drawBuilding(b);
				}
			}

			// Draw and update mobs
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);

			for (std::shared_ptr<Mob> m : GameState::mobs) {
				if (!m->isDead()) {
					drawMob(m);
					m->update(deltaTSec);
				}
			}

			// Clean up dead mobs
			// NOTE: remove_if moves all the dead ones to the end of the 
			//	vector, but you still have to call erase() to change the 
			//	size of the vector. 
			GameState::mobs.erase(std::remove_if(GameState::mobs.begin(),
				GameState::mobs.end(),
				[](std::shared_ptr<Mob> m) {return m->isDead(); }),
				GameState::mobs.end());

			size_t numDead = 0;
			for (int i = 0; i < (int)GameState::mobs.size(); ++i)
			{

			}

			// Push changes to the screen
			SDL_RenderPresent(gRenderer);
			frame++;
		}

	}

	close();
	return 0;
}

