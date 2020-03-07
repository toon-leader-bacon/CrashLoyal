// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Constants.h"
#include "Controller_UI.h"
#include "Building.h"
#include "Game.h"
#include "Mob.h"
#include "Player.h"
#include "Vec2.h"
#include "Waypoint.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

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
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, SDL_WINDOW_SHOWN);
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

int healthToAlpha(const Entity* e)
{
    float health = std::max(0.f, (float)e->getHealth());
    float maxHealth = (float)e->getMaxHealth();
    return (int)(((health / maxHealth) * 200.f) + 55.f);
}

void drawBuilding(Building* b) {
    int alpha = healthToAlpha(b);

    if (b->isDead())
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 100);
    else if (b->isNorth())
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, alpha);
    else
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, alpha);

    drawSquare(b->getPosition().x * PIXELS_PER_METER,
               b->getPosition().y * PIXELS_PER_METER,
               b->getSize() * PIXELS_PER_METER);
}

void drawText(const char* textToDraw, SDL_Rect messageRect, SDL_Color color) {
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

void drawMob(Mob* m) {
    int alpha = healthToAlpha(m);

    if (m->isNorth()) 
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, alpha);
    else 
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, alpha);

    float centerX = m->getPosition().x * PIXELS_PER_METER;
    float centerY = m->getPosition().y * PIXELS_PER_METER;
    float squareSize = m->getSize() * 2 * PIXELS_PER_METER;

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



Vec2 pixelToGrid(int x, int y) {
    // Given a pixel coordinate, this function returns the grid coordinate that contains the provided pixel
    // As always, (0,0) is top left

    Vec2 result;
    result.x = fmax(0.f, x / 10.0f);
    result.y = fmax(0.f, y / 10.0f);
    return result;
}

void drawGrid() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    // Draw Vert lines
    for (int xIndex = 0; xIndex < GAME_GRID_WIDTH; xIndex++) {
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

void drawBG() {
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

    
    drawGrid();
}

void drawWinScreen(int winningSide) {
    if (winningSide == 0) { return; }

    const char* msg = (winningSide > 0) ? "Game Over. North Wins!" : "Game Over. South Wins!" ;
    int topY = (int)(SCREEN_HEIGHT_PIXELS / 5);
    int leftX = (int)(SCREEN_WIDTH_PIXELS / 15);
    int height = (int)(SCREEN_HEIGHT_PIXELS / 3);
    int width = (int)(SCREEN_WIDTH_PIXELS * (14.f / 15));

    SDL_Rect stringRect = { leftX, topY, width, height};
    SDL_Color color = { 0, 0, 0, 255 };
    drawText(msg, stringRect, color);
}

int main(int argc, char* args[]) {
    Game& game = Game::get();
    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        using namespace std::chrono;
        high_resolution_clock::time_point prevTime = high_resolution_clock::now();

        bool quit = false;
        while (!quit) {
            // Get the elapsed time, and ensure it's at between TICK_MIN and TICK_MAX
            high_resolution_clock::time_point now = high_resolution_clock::now();
            double deltaTSec = (float)duration_cast<milliseconds>(now - prevTime).count() / 1000;

            if (deltaTSec > TICK_MAX)
            {
                std::cout << "Tick duration over budget: " << deltaTSec << std::endl;
                deltaTSec = TICK_MAX;
            }

            if (deltaTSec < TICK_MIN)
                continue;

            prevTime = now;

            //Clear rendering
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            drawBG();

            // TICK 

            if (Controller_UI::exists())
            {
                Controller_UI::get().tick((float)deltaTSec);
            }

            game.tick((float)deltaTSec);

            // RENDER

            // TODO: Move the rendering into a separate singleton (like the 
            // Game singleton)

            // Debug draw waypoints
            //for (const Waypoint* wp : game.getWaypoints())
            //{
            //    drawSquare(wp->pos.x * PIXELS_PER_METER, 
            //               wp->pos.y * PIXELS_PER_METER, 
            //               WAYPOINT_SIZE * PIXELS_PER_METER);
            //}

            for (size_t i = 0; i < BuildingType::NumBuildingTypes; ++i)
            {
                Building* pBuilding = game.getBuilding((BuildingType)i);
                if (pBuilding)
                    drawBuilding(pBuilding);
            }

            for (Mob* m : game.getMobs()) {
                if (!m->isDead()) {
                    drawMob(m);
                }
            }
            
            // If there is a winner, draw the message to the screen
            drawWinScreen(game.checkGameOver());

            SDL_RenderPresent(gRenderer);
        }

    }

    close();
    return 0;
}

