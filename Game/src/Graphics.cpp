#include "Graphics.h"

#include "Constants.h"
#include <algorithm>

Graphics* Singleton<Graphics>::s_Obj = NULL;

Graphics::Graphics() {
	gWindow = SDL_CreateWindow("Crash Loyal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		gRenderer = NULL;
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
	else {
		//Create renderer for window
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer == NULL) {
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		}
		else {
			//Initialize renderer color
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
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
}

Graphics::~Graphics() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
}

void Graphics::render() {
    SDL_RenderPresent(gRenderer);
}

void Graphics::resetFrame() {
    drawBG();
    drawUI();
}

void Graphics::drawMob(Mob* m) {
	int alpha = healthToAlpha(m);

	if (m->isNorth())
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, alpha);
	else
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, alpha);

	float centerX = m->getPosition().x * PIXELS_PER_METER;
	float centerY = m->getPosition().y * PIXELS_PER_METER;
	float squareSize = m->getStats().getSize() * PIXELS_PER_METER;

	drawSquare(centerX, centerY, squareSize);

	SDL_Rect stringRect = {
		(int)(centerX - (squareSize / 2.f)),
		(int)(centerY - (squareSize / 2.f)),
		(int)squareSize,
		(int)squareSize
	};
	SDL_Color stringColor = { 0, 0, 0, 255 };
	drawText(m->getStats().getDisplayLetter(), stringRect, stringColor);
}


void Graphics::drawSquare(float centerX, float centerY, float size) {
    // Draws a square at the given pixel coorinate
    SDL_Rect rect = {
        (int)(centerX - (size / 2.f)),
        (int)(centerY - (size / 2.f)),
        (int)(size),
        (int)(size)
    };
    SDL_RenderFillRect(gRenderer, &rect);
}

int Graphics::healthToAlpha(const Entity* e)
{
    float health = std::max(0.f, (float)e->getHealth());
    float maxHealth = (float)e->getStats().getMaxHealth();
    return (int)(((health / maxHealth) * 200.f) + 55.f);
}

void Graphics::drawBuilding(Building* b) {
    int alpha = healthToAlpha(b);

    if (b->isDead())
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 100);
    else if (b->isNorth())
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, alpha);
    else
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, alpha);

    drawSquare(b->getPosition().x * PIXELS_PER_METER,
        b->getPosition().y * PIXELS_PER_METER,
        b->getStats().getSize() * PIXELS_PER_METER);
}

void Graphics::drawText(const char* textToDraw, SDL_Rect messageRect, SDL_Color color) {
    // Draws the given text in a box with the specified position and dimention

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, textToDraw, color); // TODO Make this print something other than m
    if (!surfaceMessage) { printf("TTF_OpenFont: %s\n", TTF_GetError()); }
    SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
    if (!message) { printf("Error 2\n"); }
    //SDL_Rect messageRect = {
    //    topLeftXPix,
    //    topLeftYPix,
    //    boxWidth,
    //    boxHeight
    //    //(int)(centerX - (squareSize / 2.f)),
    //    //(int)(centerY - (squareSize / 2.f)),
    //    //(int)squareSize,
    //    //(int)squareSize
    //};
    SDL_RenderCopy(gRenderer, message, NULL, &messageRect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void Graphics::drawGrid() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    // Draw Vert lines
    for (int xIndex = 0; xIndex < GAME_GRID_WIDTH + 1; xIndex++) {
        int xIndexPix = xIndex * PIXELS_PER_METER;
        SDL_RenderDrawLine(gRenderer,
            xIndexPix, 0,
            xIndexPix, SCREEN_HEIGHT_PIXELS);
    }

    // Draw Horiz lines
    for (int yIndex = 0; yIndex < GAME_GRID_HEIGHT; yIndex++) {
        int yIndexPix = yIndex * PIXELS_PER_METER;
        SDL_RenderDrawLine(gRenderer,
            0, yIndexPix,
            GAME_GRID_WIDTH * PIXELS_PER_METER, yIndexPix);
    }
}

void Graphics::drawBG() {
    SDL_Rect bgRect = {
        0,
        0,
        SCREEN_WIDTH_PIXELS,
        SCREEN_HEIGHT_PIXELS
    };
    SDL_SetRenderDrawColor(gRenderer, 79, 161, 0, 0xFF); // Dark green
    SDL_RenderFillRect(gRenderer, &bgRect);
    int upshift = 5;
    // Draw the river
    SDL_Rect riverRect = {
        (int)(RIVER_LEFT_X * PIXELS_PER_METER),
        (int)(RIVER_TOP_Y * PIXELS_PER_METER),
        (int)(SCREEN_WIDTH_PIXELS),
        (int)((RIVER_BOT_Y - RIVER_TOP_Y) * PIXELS_PER_METER)
    };
    SDL_SetRenderDrawColor(gRenderer, 51, 119, 255, 0xFF); // Light blue
    SDL_RenderFillRect(gRenderer, &riverRect);

    // Draw bridges
    SDL_Rect bridgeLeft = {
        (int)((LEFT_BRIDGE_CENTER_X - (BRIDGE_WIDTH / 2.f)) * PIXELS_PER_METER),
        (int)((BRIDGE_CENTER_Y - (BRIDGE_HEIGHT / 2.0)) * PIXELS_PER_METER),
        (int)(BRIDGE_WIDTH * PIXELS_PER_METER),
        (int)(BRIDGE_HEIGHT * PIXELS_PER_METER)
    };
    SDL_SetRenderDrawColor(gRenderer, 179, 59, 0, 0xFF); // Brown
    SDL_RenderFillRect(gRenderer, &bridgeLeft);


    SDL_Rect bridgeRight = {
        (int)((RIGHT_BRIDGE_CENTER_X - (BRIDGE_WIDTH / 2.0)) * PIXELS_PER_METER),
        (int)((BRIDGE_CENTER_Y - (BRIDGE_HEIGHT / 2.0)) * PIXELS_PER_METER),
        (int)(BRIDGE_WIDTH * PIXELS_PER_METER),
        (int)(BRIDGE_HEIGHT * PIXELS_PER_METER)
    };
    SDL_SetRenderDrawColor(gRenderer, 179, 59, 0, 0xFF); // Brown
    SDL_RenderFillRect(gRenderer, &bridgeRight);


    drawGrid();
}

void Graphics::drawWinScreen(int winningSide) {
    if (winningSide == 0) { return; }

    const char* msg = (winningSide > 0) ? "Game Over. North Wins!" : "Game Over. South Wins!";
    int topY = SCREEN_HEIGHT_PIXELS / 5;
    int leftX = SCREEN_WIDTH_PIXELS / 15;
    int height = SCREEN_HEIGHT_PIXELS / 3;
    int width = (SCREEN_WIDTH_PIXELS * 14) / 15;

    SDL_Rect stringRect = { leftX, topY, width, height };
    SDL_Color color = { 0, 0, 0, 255 };
    drawText(msg, stringRect, color);
}

void Graphics::drawUI() {
    // Draws the rectangle to the right of the play area that contains the UI

    SDL_Rect uiRect = {
        (int)(GAME_GRID_WIDTH * PIXELS_PER_METER),
        (int)0,
        (int)(UI_WIDTH * PIXELS_PER_METER),
        (int)(UI_HEIGHT * PIXELS_PER_METER),
    };
    SDL_SetRenderDrawColor(gRenderer, 0x50, 0x50, 0x50, 100);
    SDL_RenderFillRect(gRenderer, &uiRect);


}
