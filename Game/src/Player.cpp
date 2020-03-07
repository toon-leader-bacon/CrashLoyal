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

#include "Player.h"

#include "Constants.h"
#include "iController.h"
#include "Game.h"
#include "Mob.h"

Player::Player(iController* pControl, bool bNorth) 
    : m_pControl(pControl)
    , m_bNorth(bNorth)
    , m_Elixir(capElixir(STARTING_ELIXIR))
{
    if (m_pControl) 
        m_pControl->setPlayer(*this);
}

Player::~Player()
{
    delete m_pControl;      // it's safe to delete NULL
}

iPlayer::PlacementResult Player::placeMob(MobType type, const Vec2& pos)
{
    Mob* pMob = new Mob(iMobStats::getStats(type), pos, m_bNorth);
    assert(!!pMob);

    Game::get().addMob(pMob);

    return Success;
}

void Player::tick(float deltaTSec)
{
    m_Elixir += deltaTSec * ELIXIR_PER_SECOND;

    if (m_pControl) 
        m_pControl->tick(deltaTSec);
}
