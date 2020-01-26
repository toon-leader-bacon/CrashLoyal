#ifndef __ENGINE__
#define __ENGINE__

#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include "Mob.h"
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
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		if( ! SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( ! (IMG_Init(imgFlags) & imgFlags)) {
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


void drawSquare(int centerX, int centerY, int size) {
	// Draws a square at the given pixel coorinate
	SDL_Rect rect = {
		centerX - (size / 2),
		centerY - (size / 2), 
		size,
		size
	};
	SDL_RenderFillRect(gRenderer, &rect);
}

void drawMob(std::shared_ptr<Mob> m) {
	int healthToAlpha = (m->maxHealth / m->currentHealth) * 255;
	if (m->attackingNorth) { SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, healthToAlpha);	} 
	else                   { SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, healthToAlpha); }
	drawSquare(m->pos.x, m->pos.y, m->size * 2);
}



Point pixelToGrid(int x, int y) {
	// Given a pixel coordinate, this function returns the grid coordinate that contains the provided pixel
	// As always, (0,0) is top left

	Point result;
	result.x = fmax(0, x / 10); 
	result.y = fmax(0, y / 10);
	return result;
}

void drawGrid(Point grid) {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	drawSquare(grid.x * GAME_GRID_SIZE, grid.y * GAME_GRID_SIZE, GAME_GRID_SIZE);
}

void processClick(int x, int y, bool leftClick) {
	std::shared_ptr<Mob> m = std::shared_ptr<Mob>(new Mob(x, y, leftClick));
	GameState::mobs.insert(m);
}


int main( int argc, char* args[] ) {
	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	} else {	
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		// Number of frames since start of application
		int frame = 0;

		//While application is running
		while( !quit ) {
			
			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 ) {
				//User requests quit
				if( e.type == SDL_QUIT ) {
					quit = true;
				}
				if (e.type == SDL_MOUSEBUTTONUP) {
					const SDL_MouseButtonEvent& mouse_event = e.button;
					int x, y;
					SDL_GetMouseState(&x, &y);
					if      (mouse_event.button == SDL_BUTTON_RIGHT) { processClick(x, y, false); }
					else if (mouse_event.button == SDL_BUTTON_LEFT)  { processClick(x, y, true); }
					printf("MouseUp detected at pos: ( %d, %d)\n", x, y);
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					int pixelX, pixelY;
					SDL_GetMouseState(&pixelX, &pixelY);
					drawGrid(pixelToGrid(pixelX, pixelY));
				}
			}

			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
			//drawMob(m);

			// Draw waypoints
			// TODO remove this
			for (int i = 0; i < GameState::WAYPOINT_COUNT; i++) {
				// NOTE there are 22 waypoints.
				std::shared_ptr<Waypoint> wp = GameState::waypoints.at(i);
				drawSquare(wp->pos.x, wp->pos.y, 5);
			}

			
			
			// Draw and move mobs
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
			for (std::shared_ptr<Mob> m : GameState::mobs) {
				if (frame % 20 == 0) {
					m->update();
				}
				drawMob(m);
			}
			
			

			//Update screen
			SDL_RenderPresent( gRenderer );

			frame++;
		}
		
	}

	//Free resources and close SDL
	close();

	return 0;
}

#endif