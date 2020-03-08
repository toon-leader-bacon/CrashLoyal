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

#include "Building.h"

#include "Game.h"

Building::Building(const iEntityStats& stats, const Vec2& pos, bool isNorth)
    : Entity(stats, pos, isNorth)
    , lastAttackTime(0)
    , state(BuildingState::Scaning)
{
    assert(dynamic_cast<const iEntityStats_Building*>(&stats) != NULL);
}

void Building::attackProcedure(float deltaTSec) {
    if (this->target == nullptr || this->target->isDead()) {
        this->target = nullptr;
        this->state = BuildingState::Scaning;
        return;
    }

    if (inAttackRange(this->target->getPosition())) {
        if (this->lastAttackTime >= m_Stats.getAttackTime()) {
            // If our last attack was longer ago than our cooldown

            this->target->takeDamage(m_Stats.getDamage());
            if (this->target->isDead())
                this->target = NULL;

            this->lastAttackTime = 0; // lastAttackTime is incremented in the main update function
            return;
        }
    }
    else {
        // If the target is not in range
        this->state = BuildingState::Scaning;
    }
}

bool Building::inAttackRange(Vec2 p) {
    float dist = this->m_Pos.dist(p);
    return dist < m_Stats.getAttackRange();
}

Entity* Building::findTargetInRange() {
    for (Mob* mob : Game::get().getMobs()) {
        if (mob->isNorth() != isNorth()) {
            if (inAttackRange(mob->getPosition())) {
                return mob;
            }
        }
    }
    return nullptr;
}

void Building::scanProcedure(float deltaTSec) {
    // Look for an enemy mob in range
    Entity* possibleTarget = findTargetInRange();
    if (possibleTarget != NULL) {
        this->target = possibleTarget;
        this->state = BuildingState::Attacking;
    }
}

void Building::tick(float deltaTSec) {
    switch (this->state) {
        case BuildingState::Scaning:
            this->scanProcedure(deltaTSec);
            break;
        case BuildingState::Attacking:
        default:
            this->attackProcedure(deltaTSec);
            break;
    }

    this->lastAttackTime += (float)deltaTSec;
}