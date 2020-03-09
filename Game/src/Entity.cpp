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

#include "Entity.h"

#include "Building.h"
#include "Game.h"
#include "Mob.h"
#include "Player.h"

Entity::Entity(const iEntityStats& stats, const Vec2& pos, bool isNorth)
    : m_Stats(stats)
    , m_bNorth(isNorth)
    , m_Health(stats.getMaxHealth())
    , m_Pos(pos)
    , m_pTarget(NULL)
    , m_bTargetLock(NULL)
    , m_TimeSinceAttack(0.f)
{
}

void Entity::tick(float deltaTSec)
{
    pickTarget();
    m_TimeSinceAttack += deltaTSec;
    if (targetInRange() && (m_TimeSinceAttack > m_Stats.getAttackTime()))
    {
        char buff[200];
        snprintf(buff, 200, "%s %s attacks %s %s for %d damage.\n",
                 m_bNorth ? "North" : "South",
                 m_Stats.getName(),
                 m_pTarget->isNorth() ? "North" : "South",
                 m_pTarget->getStats().getName(),
                 m_Stats.getDamage());
        std::cout << buff;

        m_bTargetLock = true;
        m_pTarget->takeDamage(m_Stats.getDamage());
        m_TimeSinceAttack = 0.f;
    }
}

void Entity::pickTarget()
{
    assert(!m_bTargetLock || !!m_pTarget);
    if (m_bTargetLock && !m_pTarget->isDead())
    {
        return;
    }

    m_pTarget = NULL;
    m_bTargetLock = false;

    Game& game = Game::get();

    float closestDistSq = FLT_MAX;

    Player& opposingPlayer = Game::get().getPlayer(!m_bNorth);

    for (Entity* pEntity : opposingPlayer.getBuildings())
    {
        assert(pEntity->isNorth() != isNorth());
        if (!pEntity->isDead())
        {
            float distSq = m_Pos.distSqr(pEntity->getPosition());
            if (distSq < closestDistSq)
            {
                closestDistSq = distSq;
                m_pTarget = pEntity;
            }
        }
    }

    if (m_Stats.getTargetType() != iEntityStats::Building)
    {
        for (Entity* pEntity : opposingPlayer.getMobs())
        {
            assert(pEntity->isNorth() != isNorth());
            if (!pEntity->isDead())
            {
                float distSq = m_Pos.distSqr(pEntity->getPosition());
                if (distSq < closestDistSq)
                {
                    closestDistSq = distSq;
                    m_pTarget = pEntity;
                }
            }
        }
    }
}

bool Entity::targetInRange()
{
    if (!!m_pTarget)
    {
        float range = m_Stats.getAttackRange();

        if (m_Stats.getDamageType() == iEntityStats::Melee)
        {
            range += ((m_Stats.getSize() + m_pTarget->getStats().getSize()) / 2.f);
        }

        return m_Pos.distSqr(m_pTarget->getPosition()) <= (range * range);
    }

    return false;
}
