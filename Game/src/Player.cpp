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
    // for now, all mob types are available.
    for (size_t i = 0; i < iEntityStats::numMobTypes; ++i)
    {
        m_AvailableMobs.push_back((iEntityStats::MobType)i);
    }

    if (m_pControl)
        m_pControl->setPlayer(*this);
}

Player::~Player()
{
    delete m_pControl;      // it's safe to delete NULL
}

iPlayer::PlacementResult Player::placeMob(iEntityStats::MobType type, const Vec2& pos)
{
    // Adjust the position to be a tile center.  Tiles are 1 unit wide.
    // TODO: move the code for converting to tile position somewhere shared
    const int iTileX = (int)pos.x;
    const int iTileY = (int)pos.y;
    const float fTileX = (float)iTileX + 0.5f;
    const float fTileY = (float)iTileY + 0.5f;
    Vec2 tilePos(fTileX, fTileY);

    // Validate the position
    // TODO: move this functionality somewhere shared.
    if ((tilePos.x <= 0) || (tilePos.x >= GAME_GRID_WIDTH))
    {
        std::cout << "Invalid Location (X): (" << tilePos.x << ", " <<
            tilePos.y << ")\n";
        return InvalidX;
    }

    if (m_bNorth)
    {
        if (tilePos.y >= RIVER_TOP_Y)
        {
            std::cout << "Invalid Location (Y): (" << tilePos.x << ", " <<
                tilePos.y << ")\n";

            return InvalidY;
        }
    }
    else
    {
        if (tilePos.y <= RIVER_BOT_Y)
        {
            std::cout << "Invalid Location (Y): (" << tilePos.x << ", " << 
                tilePos.y << ")\n";

            return InvalidY;
        }
    }

    // Validate that we have enough elixir
    const iEntityStats& stats = iEntityStats::getStats(type);
    const float cost = stats.getElixirCost();
    if (cost > m_Elixir)
    {
        std::cout << "Insufficient Elixir: " << cost << " > " << m_Elixir << 
            std::endl;

        return InsufficientElixir;
    }

    // Make sure that the mob type is one that's currently available
    if (std::find(m_AvailableMobs.begin(), m_AvailableMobs.end(), type) == m_AvailableMobs.end())
    {
        std::cout << "Mob type not available\n";

        return MobTypeUnavailable;
    }

    // Checks are done - make the mob.
    m_Elixir -= cost;
    Mob* pMob = new Mob(stats, tilePos, m_bNorth);
    Game::get().addMob(pMob);

    return Success;
}

void Player::tick(float deltaTSec)
{
    m_Elixir += deltaTSec * ELIXIR_PER_SECOND;

    if (m_pControl) 
        m_pControl->tick(deltaTSec);
}
