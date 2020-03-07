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

#include "Mob.h"

#include "Building.h"
#include "Constants.h"
#include "Game.h"
#include "Vec2.h"
#include "Waypoint.h"

int Mob::s_PreviousUID = 0;

Mob::Mob(const iMobStats& stats, const Vec2& pos, bool isNorth)
    : m_Uid(++s_PreviousUID)
    , m_bIsNorth(isNorth)
    , m_Stats(stats)
    , m_pAttackTarget(NULL)
    , m_pMoveTarget(NULL)
    , m_LastAttackTime(0)
{
    m_Pos = pos;
    m_Health = stats.getMaxHealth();
}

const Waypoint* Mob::findClosestWaypoint() {
    const std::vector<Waypoint*>& waypoints = Game::get().getWaypoints();

    const Waypoint* pClosest = NULL;

    float smallestDist = std::numeric_limits<float>::max();
    for (const Waypoint* wp : waypoints) {
        // Filter out any waypoints that are "behind" us (behind is relative to attack dir
        // Remember y=0 is in the top left
        if (m_bIsNorth == (wp->m_Pos.y < m_Pos.y)) {
            continue;
        }

        float dist = m_Pos.dist(wp->m_Pos);
        if (dist < smallestDist) {
            smallestDist = dist;
            pClosest = wp;
        }
    }

    return pClosest;
}

void Mob::moveTowards(const Vec2& moveTarget, float elapsedTime) {
    Vec2 movementVector = moveTarget - m_Pos;
    movementVector.normalize();
    movementVector *= (float)m_Stats.getSpeed();
    movementVector *= (float)elapsedTime;
    
    m_Pos += movementVector;
}

void Mob::pickTargets() {
    // if we already have an attack target, make sure that it's not dead.
    if (m_pAttackTarget && m_pAttackTarget->isDead())
    {
        m_pAttackTarget = NULL;
    }

    // If we don't have a target, look for something to attack
    if (!m_pAttackTarget)
    {
        Game& game = Game::get();

        float closestDistSq = FLT_MAX;

        const BuildingType firstTower = m_bIsNorth
            ? BuildingType::FirstSouthTower
            : BuildingType::FirstNorthTower;
        const BuildingType lastTower = m_bIsNorth
            ? BuildingType::LastSouthTower
            : BuildingType::LastNorthTower;

        for (int i = firstTower; i <= lastTower; ++i)
        {
            Building* pTower = game.getBuilding((BuildingType)i);
            if (!pTower->isDead())
            {
                float distSq = Vec2::distSqr(m_Pos, pTower->getPosition());
                if (distSq < closestDistSq)
                {
                    closestDistSq = distSq;
                    m_pAttackTarget = pTower;
                }
            }
        }

        for (Mob* pOtherMob : Game::get().getMobs()) {
            if ((pOtherMob->m_bIsNorth != m_bIsNorth) && !pOtherMob->isDead())
            {
                float distSq = Vec2::distSqr(m_Pos, pOtherMob->getPosition());
                if (distSq < closestDistSq)
                {
                    closestDistSq = distSq;
                    m_pAttackTarget = pOtherMob;
                }
            }
        }
    }

    // If we have an attack target but it's on the other side of the river, we 
    // also pick a move target (to move towards the bridge).  Otherwise, we 
    // need to clear the move target.
    // TODO: Handle logic for ranged shooting across river
    if (m_pAttackTarget)
    {
        bool imTop = m_Pos.y < (GAME_GRID_HEIGHT / 2);
        bool otherTop = m_pAttackTarget->getPosition().y < (GAME_GRID_HEIGHT / 2);
        m_pMoveTarget = (imTop == otherTop)
            ? NULL
            : findClosestWaypoint();
    }
    // Otherwise, if we don't currently have a move target we need to pick one
    else if (!m_pMoveTarget)
    {
        m_pMoveTarget = findClosestWaypoint();
    }
}

// Movement related
//////////////////////////////////
// Combat related

// TODO: unify with similar functionality in the buildings
bool Mob::targetInRange() {
    float range = getSize(); // TODO: change this for ranged mobs
    float totalSize = range + m_pAttackTarget->getSize();
    return m_Pos.insideOf(m_pAttackTarget->getPosition(), totalSize);
}

// Combat related
////////////////////////////////////////////////////////////
// Collisions

// PROJECT 3: 
//  1) return a vector of mobs that we're colliding with
//  2) handle collision with towers & river 
Mob* Mob::checkCollision() {
    for (const Mob* pOtherMob : Game::get().getMobs()) {
        // don't collide with yourself
        if (*this == *pOtherMob) continue;

        // PROJECT 3: YOUR CODE CHECKING FOR A COLLISION GOES HERE
    }
    return NULL;
}

void Mob::processCollision(Mob* otherMob, float elapsedTime) {
    // PROJECT 3: YOUR COLLISION HANDLING CODE GOES HERE
}

// Collisions
///////////////////////////////////////////////
// Procedures

// TODO: unify with building
void Mob::attackProcedure(float elapsedTime) {
    assert(m_pAttackTarget && !m_pAttackTarget->isDead());

    if (targetInRange()) {
        if (m_LastAttackTime >= m_Stats.getAttackTime()) {
            m_LastAttackTime = 0; // lastAttackTime is incremented in the main update function

            std::cout << "Attack!" << std::endl;

            m_pAttackTarget->takeDamage(m_Stats.getDamage());

            if (m_pAttackTarget->isDead())
            {
                m_pAttackTarget = NULL;

            }
            return;
        }
    }
    else {
        // If the target is not in range
        moveTowards(m_pAttackTarget->getPosition(), elapsedTime);
    }
}

void Mob::moveProcedure(float elapsedTime) {
    assert(!!m_pMoveTarget);
    const Vec2& targetPos = m_pMoveTarget->m_Pos;

    moveTowards(targetPos, elapsedTime);

    // Check for collisions
    if (targetPos.insideOf(m_Pos, (getSize() + WAYPOINT_SIZE))) {
        m_pMoveTarget = m_bIsNorth
            ? m_pMoveTarget->m_DownNeighbor
            : m_pMoveTarget->m_UpNeighbor;
    }

    // PROJECT 3: You should not change this code very much, but this is where your 
    // collision code will be called from
    Mob* otherMob = checkCollision();
    if (otherMob) {
        processCollision(otherMob, elapsedTime);
    }
}

void Mob::update(float elapsedTime) {
    m_LastAttackTime += (float)elapsedTime;

    pickTargets();
    assert(!!m_pMoveTarget || !!m_pAttackTarget);

    // If we have a move target, move to it (if there's also an attack target, 
    // it's on the far side of the bridge).

    if (m_pMoveTarget)
        moveProcedure(elapsedTime);
    else if (m_pAttackTarget)
        attackProcedure(elapsedTime);
}
