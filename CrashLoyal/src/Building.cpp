
#include "GameState.h"
#include "Building.h"

Building::Building(float x, float y, BuildingType type)
 : lastAttackTime(0)
 , state(BuildingState::Scaning)
	  
{
	Point p = *(new Point(x, y));
	this->pos = p;
	this->type = type;

	if (this->type == BuildingType::NorthKing || this->type == BuildingType::SouthKing) {
		this->size = KingTowerSize;
		this->health = KingTowerHealth;
		this->attackRadius = KingTowerAttackRadius;
	} else {
		this->size = SmallTowerSize;
		this->health = SmallTowerHealth;
		this->attackRadius = SmallTowerAttackRadius;
	}

	this->isNorthBuilding = (this->type == BuildingType::NorthKing ||
							 this->type == BuildingType::NorthLeftTower ||
							 this->type == BuildingType::NorthRightTower);
}

int Building::attack(int dmg) {
	health -= dmg;
	if (this->isDead()) { GameState::removeBuilding(this); }
	return health;
}

std::shared_ptr<Point> Building::getPosition() {
	return std::shared_ptr<Point>(&(this->pos));
}

float Building::GetSize() const {
	return this->size;
}

void Building::attackProcedure(double elapsedTime) {
	if (this->lastAttackTime >= this->GetAttackTime()) {
		this->target->attack(this->GetDamage());
		this->lastAttackTime = 0; // lastAttackTime is incremented in the main update function
		return;
	}
}

Attackable* Building::findTargetInRange() {
	for (std::shared_ptr<Mob> mob : GameState::mobs) {
		if (mob->IsAttackingNorth() != this->isNorthBuilding) { 
			return mob.get();
		}
	}
	return nullptr;
}

void Building::scanProcedure(double elapsedTime) {
	// Look for an enemy mob in range
	Attackable* possibleTargete = findTargetInRange();
	if (possibleTargete != nullptr) {
		this->target = possibleTargete;
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