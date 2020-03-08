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

// Final Project: A controller is a thing that controls a player - i.e. either
// a UI or an AI.  Your controller will be in its own project (instructions
// for that are in the assignment), but will inherit from this and implement
// all of the pure virtual functions.  The game will call those functions when
// it is time for the controler to do its work.

#include <assert.h>
#include "SDL.h"

class iPlayer;

class iController
{
public:
    iController() : m_pPlayer(NULL) {}
    virtual ~iController() {}

    void setPlayer(iPlayer& player) { assert(!m_pPlayer); m_pPlayer = &player; }

    // Final Project: This is where you will do most of your work.  This is 
    // called as part of the game loop.  deltaTSec is the elapsed time (in
    // seconds, and in game time) since the last tick.
    virtual void tick(float deltaTSec) = 0;

protected:
    iPlayer* m_pPlayer; // NOT owned, guaranteed to exist when tick() is called

private:
    // DELIBERATELY UNDEFINED
    iController(const iController& rhs);
    iController& operator=(const iController& rhs);
    bool operator==(const iController& rhs) const;
    bool operator<(const iController& rhs) const;
};