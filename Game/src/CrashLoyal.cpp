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
#include "Graphics.h"
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


Graphics* graphics;

bool init() {
    graphics = &Graphics::get();
    return true;
}

void close() {
    graphics->~Graphics();

    IMG_Quit();
    SDL_Quit();
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
        SDL_Event e;
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

            graphics->resetFrame();

            // TICK 

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) { quit = true; }
                if (Controller_UI::exists()) {
                    Controller_UI::get().loadEvent(e);
                }
            }

            if (Controller_UI::exists()) {
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

            for (Building* pBuilding : Game::get().getBuildings()) {
                graphics->drawBuilding(pBuilding);
            }

            for (Mob* m : game.getMobs()) {
                if (!m->isDead()) {
                    graphics->drawMob(m);
                }
            }

            // If there is a winner, draw the message to the screen
            graphics->drawWinScreen(game.checkGameOver());

            graphics->render();
        }

    }

    close();
    return 0;
}

