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

#include "Building.h"
#include "Constants.h"
#include "iController.h"
#include "Game.h"
#include "Mob.h"

Player::Player(iController* pControl, bool bNorth)
    : m_pControl(pControl)
    , m_bNorth(bNorth)
    , m_Elixir(capElixir(STARTING_ELIXIR))
{
    buildBuildings();

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
    for (Entity* pBuilding : m_Buildings) delete pBuilding;
    for (Entity* pMob : m_Mobs) delete pMob;
    for (Entity* pMob : m_DeadMobs) delete pMob;
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
    m_Mobs.push_back(pMob);

    return Success;
}

void Player::tick(float deltaTSec)
{
    m_Elixir += deltaTSec * ELIXIR_PER_SECOND;

    if (m_pControl)
        m_pControl->tick(deltaTSec);

    for (Entity* pBuilding : m_Buildings) {
        if (!pBuilding->isDead()) {
            pBuilding->tick(deltaTSec);
        }
    }

    for (Entity* m : m_Mobs) {
        if (!m->isDead()) {
            m->tick(deltaTSec);
        }
    }

    // Move any mobs that died this tick into m_DeadMobs
    size_t newIndex = 0;
    for (size_t oldIndex = 0; oldIndex < m_Mobs.size(); ++oldIndex)
    {
        Entity* pMob = m_Mobs[oldIndex];
        if (!pMob->isDead())
        {
            Entity* pTemp = m_Mobs[newIndex];
            m_Mobs[newIndex] = m_Mobs[oldIndex];
            m_Mobs[oldIndex] = pTemp;
            ++newIndex;
        }
        else
        {
            m_DeadMobs.push_back(m_Mobs[oldIndex]);
        }
    }

    assert(newIndex <= m_Mobs.size());
    m_Mobs.resize(newIndex);
}

iPlayer::EntityData Player::getBuilding(unsigned int i) const
{
    if (i < m_Buildings.size())
    {
        return m_Buildings[i]->getData();
    }

    return EntityData();
}

iPlayer::EntityData Player::getMob(unsigned int i) const
{
    if (i < m_Mobs.size())
    {
        return m_Mobs[i]->getData();
    }

    return EntityData();
}

iPlayer::EntityData Player::getOpponentBuilding(unsigned int i) const
{
    if (i < GetOpponent().getBuildings().size())
    {
        return GetOpponent().getBuildings()[i]->getData();
    }

    return EntityData();
}

iPlayer::EntityData Player::getOpponentMob(unsigned int i) const
{
    if (i < GetOpponent().getMobs().size())
    {
        return GetOpponent().getMobs()[i]->getData();
    }

    return EntityData();
}

void Player::buildBuildings()
{
    const iEntityStats& kingStats = iEntityStats::getBuildingStats(iEntityStats::King);
    const iEntityStats& princessStats = iEntityStats::getBuildingStats(iEntityStats::Princess);

    if (m_bNorth)
    {
        m_Buildings.push_back(new Building(kingStats, Vec2(KingX, NorthKingY), true));
        m_Buildings.push_back(new Building(princessStats, Vec2(PrincessLeftX, NorthPrincessY), true));
        m_Buildings.push_back(new Building(princessStats, Vec2(PrincessRightX, NorthPrincessY), true));
    }
    else
    {
        m_Buildings.push_back(new Building(kingStats, Vec2(KingX, SouthKingY), false));
        m_Buildings.push_back(new Building(princessStats, Vec2(PrincessLeftX, SouthPrincessY), false));
        m_Buildings.push_back(new Building(princessStats, Vec2(PrincessRightX, SouthPrincessY), false));
    }
}

const Player& Player::GetOpponent() const
{
    const Player& opPlayer = Game::get().getPlayer(!m_bNorth);
    assert(&opPlayer != this);
    return opPlayer;
}

