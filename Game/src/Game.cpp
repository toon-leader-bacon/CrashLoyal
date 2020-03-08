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

#include "Game.h"

#include <cmath>
#include "Building.h"
#include "Constants.h"
#include "Controller_UI.h"
#include "Mob.h"
#include "Player.h"

Game* Singleton<Game>::s_Obj = NULL;

Game::Game()
    : gameOverState(0) // No winner at start of game
{
    // FinalProject: This is where you specify which controllers to use - for 
    // instance, if you make two instances of your AI then it will play 
    // itself, or if you make one the UI and one your AI then you can play
    // against your AI.  If you make the controller NULL then that player
    // will just passively sit there and let you kill it.
    buildPlayers(NULL, new Controller_UI);

    buildWaypoints();
    buildBuildings();
}

Game::~Game()
{
    for (Building* pBuilding : m_Buildings) delete pBuilding;
    for (Mob* pMob : m_Mobs) delete pMob;
    for (Mob* pMob : m_DeadMobs) delete pMob;
    delete m_pNorthPlayer;
    delete m_pSouthPlayer;
}

void Game::tick(float deltaTSec)
{
    m_pNorthPlayer->tick(deltaTSec);
    m_pSouthPlayer->tick(deltaTSec);

    for (Building* pBuilding : m_Buildings) {
        if (!pBuilding->isDead()) {
            pBuilding->tick(deltaTSec);
        }
    }

    for (Mob* m : m_Mobs) {
        if (!m->isDead()) {
            m->tick(deltaTSec);
        }
    }

    // Clean up the last tick's dead mobs
    for (Mob* pDeadMob : m_DeadMobs)
    {
        delete pDeadMob;
    }
    m_DeadMobs.resize(0);

    // Move any mobs that died this tick into m_DeadMobs
    size_t newIndex = 0;
    for (size_t oldIndex = 0; oldIndex < m_Mobs.size(); ++oldIndex)
    {
        Mob* pMob = m_Mobs[oldIndex];
        if (!pMob->isDead())
        {
            Mob* pTemp = m_Mobs[newIndex];
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

int Game::checkGameOver() {
    if (gameOverState == 0) {

        for (Building* pBuilding : Game::get().getBuildings()) {
            if ((pBuilding->getStats().getBuildingType() == iEntityStats::King) && pBuilding->isDead()) {
                gameOverState = pBuilding->isNorth() ? -1 : 1;
            }
        }
    }

    return gameOverState;
}

void Game::buildPlayers(iController* pNorthControl, iController* pSouthControl)
{
    m_pNorthPlayer = new Player(pNorthControl, true);
    m_pSouthPlayer = new Player(pSouthControl, false);
}

void Game::buildWaypoints()
{
    // The first waypoint is 1 tile toward the center of the princess tower
    const float princessSize = iEntityStats::getBuildingStats(iEntityStats::Princess).getSize();
    const Vec2 first(PrincessLeftX + (princessSize / 2.f) + 1.f, NorthPrincessY);
    addFourWaypoints(first);

    for (float y = first.y + WAYPOINT_Y_INCREMENT; y < RIVER_TOP_Y; y += WAYPOINT_Y_INCREMENT)
    {
        addFourWaypoints(Vec2(LEFT_BRIDGE_CENTER_X, y));
    }
}

void Game::addFourWaypoints(Vec2 pt)
{
    const float rightX = GAME_GRID_WIDTH - pt.x;
    const float bottomY = GAME_GRID_HEIGHT - pt.y;

    m_Waypoints.push_back(pt);
    m_Waypoints.push_back(Vec2(rightX, pt.y));
    m_Waypoints.push_back(Vec2(pt.x, bottomY));
    m_Waypoints.push_back(Vec2(rightX, bottomY));
}

void Game::buildBuildings()
{
    const iEntityStats& kingStats = iEntityStats::getBuildingStats(iEntityStats::King);
    const iEntityStats& princessStats = iEntityStats::getBuildingStats(iEntityStats::Princess);

    m_Buildings.push_back(new Building(kingStats, Vec2(KingX, NorthKingY), true));
    m_Buildings.push_back(new Building(princessStats, Vec2(PrincessLeftX, NorthPrincessY), true));
    m_Buildings.push_back(new Building(princessStats, Vec2(PrincessRightX, NorthPrincessY), true));

    m_Buildings.push_back(new Building(kingStats, Vec2(KingX, SouthKingY), false));
    m_Buildings.push_back(new Building(princessStats, Vec2(PrincessLeftX, SouthPrincessY), false));
    m_Buildings.push_back(new Building(princessStats, Vec2(PrincessRightX, SouthPrincessY), false));
}
