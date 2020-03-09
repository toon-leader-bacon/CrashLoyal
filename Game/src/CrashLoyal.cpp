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

#include "Building.h"
#include "Constants.h"
#include "Controller_UI.h"
#include "Game.h"
#include "Graphics.h"
#include "Player.h"

#include <chrono>

bool init() {
    return true;
}

void close() {
    // You should never call the destructor directly!!!
    //graphics->~Graphics();

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    Game& game = Game::get();
    Graphics& graphics = Graphics::get();

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

            graphics.resetFrame();

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
            Player& northPlayer = game.getPlayer(true);
            Player& southPlayer = game.getPlayer(false);

            for (Entity* pBuilding : northPlayer.getBuildings()) {
                graphics.drawBuilding(pBuilding);
            }

            for (Entity* pBuilding : southPlayer.getBuildings()) {
                graphics.drawBuilding(pBuilding);
            }

            for (Entity* m : northPlayer.getMobs()) {
                if (!m->isDead()) {
                    graphics.drawMob(m);
                }
            }

            for (Entity* m : southPlayer.getMobs()) {
                if (!m->isDead()) {
                    graphics.drawMob(m);
                }
            }

            // If there is a winner, draw the message to the screen
            graphics.drawWinScreen(game.checkGameOver());

            graphics.render();
        }

    }

    close();
    return 0;
}

