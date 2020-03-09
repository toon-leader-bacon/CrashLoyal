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

#include "Controller_AI_KevinDill.h"

#include "Constants.h"
#include "EntityStats.h"
#include "iPlayer.h"
#include "Vec2.h"

static const Vec2 ksGiantPos(LEFT_BRIDGE_CENTER_X, RIVER_TOP_Y - 0.5f);
static const Vec2 ksArcherPos(LEFT_BRIDGE_CENTER_X, 0.f);

void Controller_AI_KevinDill::tick(float deltaTSec)
{
    assert(m_pPlayer);

    // wait for elixir
    if (m_pPlayer->getElixir() >= 9)
    {
        // convert the positions from player space to game space
        bool isNorth = m_pPlayer->isNorth();
        Vec2 giantPos_Game = ksGiantPos.Player2Game(isNorth);
        Vec2 archerPos_Game = ksArcherPos.Player2Game(isNorth);

        // Create two archers and a giant
        m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
        m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
        m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
    }
}

