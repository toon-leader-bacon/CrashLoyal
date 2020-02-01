#include <memory>
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include "Swordsman.h"
#include "Point.h"
#include "Waypoint.h"
#include "GameState.h"

//The window that houses the renderrer
SDL_Window* gWindow = NULL;
//The window renderer itself
SDL_Renderer* gRenderer = NULL;

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
	switch (b->type)
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

	drawSquare(b->pos.x, b->pos.y, b->GetSize());
}

void drawMob(std::shared_ptr<Mob> m) {
	int healthToAlpha = int(((float)m->GetHealth() / (float)m->GetMaxHealth()) * 155) + 100;
	if (m->IsAttackingNorth()) { SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, healthToAlpha); }
	else { SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, healthToAlpha); }
	drawSquare(m->pos.x, m->pos.y, m->GetSize() * 2);
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
	drawSquare(grid.x * GAME_GRID_SIZE, grid.y * GAME_GRID_SIZE, GAME_GRID_SIZE);
}

void processClick(int x, int y, bool leftClick) {
	const Point pos((float)x, (float)y);
	std::shared_ptr<Mob> m = std::shared_ptr<Mob>(new Swordsman(pos, leftClick));
	GameState::mobs.insert(m);
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
		0,
		(SCREEN_HEIGHT / 2) - upshift,
		SCREEN_WIDTH,
		20
	};
	SDL_SetRenderDrawColor(gRenderer, 51, 119, 255, 0xFF); // Light blue
	SDL_RenderFillRect(gRenderer, &riverRect);

	// Draw bridges
	SDL_Rect bridgeLeft = {
		SCREEN_WIDTH / 5,
		(SCREEN_HEIGHT / 2) - 2 - upshift,
		30,
		25
	};
	SDL_SetRenderDrawColor(gRenderer, 179, 59, 0, 0xFF); // Brown
	SDL_RenderFillRect(gRenderer, &bridgeLeft);


	SDL_Rect bridgeRight = {
		(SCREEN_WIDTH * 3 / 4) - 15,
		(SCREEN_HEIGHT / 2) - 2 - upshift,
		30,
		25
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
					if (mouse_event.button == SDL_BUTTON_RIGHT)     { processClick(x, y, false); }
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
				drawSquare(wp->pos.x, wp->pos.y, 5);
			}

			// Draw Buildings
			for (std::shared_ptr<Building> b : GameState::buildings)
			{
				drawBuilding(b);
			}

			// Draw and update mobs
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
			for (std::shared_ptr<Mob> m : GameState::mobs) {
				if (frame % 20 == 0) {
					m->update();
				}
				drawMob(m);
			}

			// Push changes to the screen
			SDL_RenderPresent(gRenderer);
			frame++;
		}

	}

	close();
	return 0;
}

