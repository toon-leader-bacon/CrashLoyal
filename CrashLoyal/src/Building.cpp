
#include "GameState.h"
#include "Building.h"

Building::Building(float x, float y, BuildingType type) {
	Point p = *(new Point(x, y));
	this->pos = p;
	this->type = type;

	if (this->type == NorthKing || this->type == SouthKing) {
		this->radius = KingTowerSize;
		this->health = KingTowerHealth;
	} else {
		this->radius = SmallTowerSize;
		this->health = smallTowerHalth;

	}

	this->isNorthBuilding = (this->type == NorthKing || 
							 this->type == NorthLeftTower || 
							 this->type == NorthRightTower);
}

int Building::attack(int dmg) {
	health += dmg;
	if (this->isDead()) { GameState::removeBuilding(this); }
	return health;
}

std::shared_ptr<Point> Building::getPosition() {
	return std::shared_ptr<Point>(&(this->pos));
}

float Building::getSize() {
	return this->radius;
}