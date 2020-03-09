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

#include "Singleton.h"
#include "Vec2.h"
#include <vector>

class Building;
class iController;
class Mob;
class Player;

class Game : public Singleton<Game>
{
public:
    explicit Game();
    virtual ~Game();

    void tick(float deltaTSec);

    Player& getPlayer(bool bNorth) { return bNorth ? *m_pNorthPlayer : *m_pSouthPlayer; }

    const std::vector<Vec2>& getWaypoints() const { return m_Waypoints; }

    int checkGameOver();

private:
    void buildPlayers(iController* pNorthControl, iController* pSouthControl);

    void buildWaypoints();
    void addFourWaypoints(Vec2 pt);

private:
    Player* m_pNorthPlayer;
    Player* m_pSouthPlayer;

    std::vector<Vec2> m_Waypoints;

    // Negative => South won, Positive => North won, 0 => no winner yet
    int gameOverState; 
};

