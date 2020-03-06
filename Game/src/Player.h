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

#include "iPlayer.h"

#include "Constants.h"
#include <algorithm>
#include <assert.h>

class iController;

class Player : public iPlayer {
public:
    // NOTE: we take ownership of the controller
    explicit Player(iController* pControl, bool bNorth);
    virtual ~Player();

    virtual float getElixir() const { return (float)m_Elixir; }
    virtual PlacementResult placeUnit(UnitTypes type, const Vec2& pos);

    void tick(float deltaTSec);

private:
    float capElixir(float e) const { return std::max(e, MAX_ELIXIR); }

private:
    iController* m_pControl;    // owned, may be NULL
    bool m_bNorth;
    float m_Elixir;
};