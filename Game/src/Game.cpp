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
    for (Waypoint* pWaypoint : m_Waypoints) delete pWaypoint;
    for (Building* pBuilding : m_Buildings) delete pBuilding;
    for (Mob* pMob : m_Mobs) delete pMob;
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
            m->update(deltaTSec);
        }
    }

    // Clean up dead mobs
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
            delete m_Mobs[oldIndex];
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
    /*
      The structure of the returned Waypoint array is as follows. The numbers indicate the position the
      waypoint exists in the list. The location on the grid represents the position the waypoint exists
      in the game world.
     +----------------+
     |    21  0  1    |
     |  20         2  |
     |                |
     |  19         3  |
     |  18         4  |
     |  17         5  |
     |  16         6  |
     |  15         7  |
     |  14         8  |
     |                |
     |  13         9  |
     |   12  11  10   |
     +----------------+

     Each waypoint is linked such that the upNeighbor always points towards waypoint 0 (the enemy king)
     while the downNeighbor always points towards waypoint 11 (the friendly king).
     The enemy king (node 0) has an upNeighbor of 21 and a downNeighbor of 1.
     Likewise the friendly king (node 11) has an upNeighbor of 12 and a downNeighbor of 10.

     Waypoints 0 and 11 represent king towers
     Waypoints 20, 2, 10 and 12 represent Princess towers.

     */

    m_Waypoints.resize(22);

    float X_InitialPos = (float)GAME_GRID_WIDTH / 4.0f;

    float Y_InitialPos = (float)GAME_GRID_HEIGHT / 4.0f;
    float Y_Increment = (float)GAME_GRID_HEIGHT / 10.0f; // Split the middle 50% into 5 equal sections, waypoint before+after each slice

    // Build the waypoints for a lane
    for (int i = 0; i < 6; i++) {
        Vec2 ptLeft;
        ptLeft.x = X_InitialPos;
        ptLeft.y = Y_InitialPos + (i * Y_Increment);
        Waypoint* wpLeft = new Waypoint();
        wpLeft->m_Pos = ptLeft;
        m_Waypoints[19 - i] = wpLeft;

        Vec2 ptRight;
        ptRight.x = GAME_GRID_WIDTH - X_InitialPos;
        ptRight.y = Y_InitialPos + (i * Y_Increment);
        Waypoint* wpRight = new Waypoint();
        wpRight->m_Pos = ptRight;
        m_Waypoints[3 + i] = wpRight;
    }

    // Build the waypoints for the tower rows
    // Top Row
    Vec2 princessTLpt;
    princessTLpt.x = PrincessLeftX;
    princessTLpt.y = NorthPrincessY;
    Waypoint* princessTL = new Waypoint();
    princessTL->m_Pos = princessTLpt;
    m_Waypoints[20] = princessTL;

    Vec2 princessTRpt;
    princessTRpt.x = PrincessRightX;
    princessTRpt.y = NorthPrincessY;
    Waypoint* princessTR = new Waypoint();
    princessTR->m_Pos = princessTRpt;
    m_Waypoints[2] = princessTR;

    Vec2 kingTopPt;
    kingTopPt.x = KingX;
    kingTopPt.y = NorthKingY;
    Waypoint* kingTop = new Waypoint();
    kingTop->m_Pos = kingTopPt;
    m_Waypoints[0] = kingTop;

    Waypoint* topLeftMid = new Waypoint();
    topLeftMid->m_Pos = (kingTopPt + princessTLpt) / 2.f;
    m_Waypoints[21] = topLeftMid;

    Waypoint* topRightMid = new Waypoint();
    topRightMid->m_Pos = (kingTopPt + princessTRpt) / 2.f;
    m_Waypoints[1] = topRightMid;

    // Bot Row
    Vec2 princessBLpt;
    princessBLpt.x = PrincessLeftX;
    princessBLpt.y = SouthPrincessY;
    Waypoint* princessBL = new Waypoint();
    princessBL->m_Pos = princessBLpt;
    m_Waypoints[13] = princessBL;

    Vec2 princessBRpt;
    princessBRpt.x = PrincessRightX;
    princessBRpt.y = SouthPrincessY;
    Waypoint* princessBR = new Waypoint();
    princessBR->m_Pos = princessBRpt;
    m_Waypoints[9] = princessBR;

    Vec2 kingBotPt;
    kingBotPt.x = KingX;
    kingBotPt.y = SouthKingY;
    Waypoint* kingBot = new Waypoint();
    kingBot->m_Pos = kingBotPt;
    m_Waypoints[11] = kingBot;

    Waypoint* botLeftMid = new Waypoint();
    botLeftMid->m_Pos = (kingBotPt + princessBLpt) / 2.f;
    m_Waypoints[12] = botLeftMid;

    Waypoint* botRightMid = new Waypoint();
    botRightMid->m_Pos = (kingBotPt + princessBRpt) / 2.f;
    m_Waypoints[10] = botRightMid;


    // Link the waypoints correctly
    for (int i = 0; i < 11; i++) {
        // Right side of the map
        m_Waypoints[i]->m_UpNeighbor = m_Waypoints[abs((i - 1) % 22)];
        m_Waypoints[i]->m_DownNeighbor = m_Waypoints[i + 1];
        // Left side of the map
        int mirroredIndex = 21 - i;
        m_Waypoints[mirroredIndex]->m_UpNeighbor = m_Waypoints[(mirroredIndex + 1) % 22];
        m_Waypoints[mirroredIndex]->m_DownNeighbor = m_Waypoints[mirroredIndex - 1];
    }
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
