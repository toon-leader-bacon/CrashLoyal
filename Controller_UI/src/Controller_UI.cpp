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

#include "Controller_UI.h"

#include "Constants.h"
#include "EntityStats.h"
#include "iPlayer.h"
#include "Vec2.h"

#include "SDL.h"

Controller_UI* Singleton<Controller_UI>::s_Obj = NULL;

Controller_UI::~Controller_UI()
{
    std::cout << "Controller_UI is being deleted. This probably means that "
        << "you made more than one." << std::endl;
}

void Controller_UI::tick(float deltaTSec) {
    while(!events.empty()) {
        SDL_Event e = events.front();
        events.pop();

        if ((e.type == SDL_MOUSEBUTTONUP) && (e.button.button == SDL_BUTTON_LEFT)) {
            int pixelX = -1;
            int pixelY = -1;
            SDL_GetMouseState(&pixelX, &pixelY);
            const Vec2 mousePos((float)(pixelX / PIXELS_PER_METER), (float)(pixelY / PIXELS_PER_METER));

            iEntityStats::MobType mobType;
            if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
            {
                mobType = iEntityStats::Archer;
            }
            else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S])
            {
                mobType = iEntityStats::Swordsman;
            }
            else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_G])
            {
                mobType = iEntityStats::Giant;
            }
            else
            {
                continue;
            }

            assert(m_pPlayer);
            m_pPlayer->placeMob(mobType, mousePos);
        }
    }
}

void Controller_UI::loadEvent(SDL_Event e) {
    events.push(e);
}