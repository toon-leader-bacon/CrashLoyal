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

#include "GameState.h"

const float KingTowerSize = 5.0f;
const int KingTowerHealth = 100;

const float SmallTowerSize = 3.f;
const int SmallTowerHealth = 50;

const float KingTowerAttackRadius = 20.f;
const float SmallTowerAttackRadius = 10.f;

Building::Building(float x, float y, BuildingType type)
 : lastAttackTime(0)
 , state(BuildingState::Scaning)  
{
	Vec2 p = *(new Vec2(x, y));
	this->m_Pos = p;
	this->type = type;

	if (this->type == BuildingType::NorthKing || this->type == BuildingType::SouthKing) {
		this->size = KingTowerSize;
		m_Health = KingTowerHealth;
		this->attackRadius = KingTowerAttackRadius;
	} else {
		this->size = SmallTowerSize;
		m_Health = SmallTowerHealth;
		this->attackRadius = SmallTowerAttackRadius;
	}

	m_MaxHealth = m_Health;
}

void Building::attackProcedure(double elapsedTime) {
	if (this->target == nullptr || this->target->isDead()) {
		this->target = nullptr;
		this->state = BuildingState::Scaning;
		return;
	}

	if (inAttackRange(this->target->getPosition())) {
		if (this->lastAttackTime >= this->getAttackTime()) {
			// If our last attack was longer ago than our cooldown

			this->target->takeDamage(this->getDamage());
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
	return dist < this->attackRadius;
}

Entity* Building::findTargetInRange() {
	for (Mob* mob : GameState::get().getMobs()) {
		if (mob->isNorth() != isNorth()) {
			if (inAttackRange(mob->getPosition())) {
				return mob;
			}
		}
	}
	return nullptr;
}

void Building::scanProcedure(double elapsedTime) {
	// Look for an enemy mob in range
	Entity* possibleTarget = findTargetInRange();
	if (possibleTarget != NULL) {
		this->target = possibleTarget;
		this->state = BuildingState::Attacking;
	}
}

void Building::update(double elapsedTime) {
	switch (this->state) {
	case BuildingState::Scaning:
		this->scanProcedure(elapsedTime);
		break;
	case BuildingState::Attacking:
	default:
		this->attackProcedure(elapsedTime);
		break;
	}

	this->lastAttackTime += (float)elapsedTime;
}