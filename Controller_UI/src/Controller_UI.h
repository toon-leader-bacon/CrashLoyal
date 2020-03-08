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

#pragma once

#include "iController.h"
#include <queue>
#include "SDL.h"
#include <Singleton.h>

struct SDL_MouseButtonEvent;

// TODO List:
// - Get the list of units (and stats) from EntityStats.h
// - Display elixir on the side of screen (get it from m_pPlayer->getElixir()
// - Buttons on the side of screen for spawning mobs
//    - left click on button to select mob, left click on screen to drop
//    - display reason when spawn fails (and maybe play a sound?)
// - Split out rendering:
//    - Make a rendering singleton that has a preTick, a tick, and some functions to add other stuff to draw (e.g. attack visualization)
//    - Move the rest of the code from main() into here, so all event handling is here.

// Rendering TODO

// Extra Bonus Stuff:
// - Stats for each mob displayed in mouse-over
// - health bars on mobs & towers
// - sound

class Controller_UI : public iController, public Singleton<Controller_UI>
{
public:
    Controller_UI() {}
    virtual ~Controller_UI();

    void tick(float deltaTSec);
    void loadEvent(SDL_Event e);

private:
    
    std::queue<SDL_Event> events;

};