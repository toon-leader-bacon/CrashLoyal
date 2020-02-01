
#include "GameState.h"
#include "Building.h"

Building::Building(float x, float y, BuildingType type) {
	Point p = *(new Point(x, y));
	this->pos = p;
	this->type = type;

	if (this->type == BuildingType::NorthKing || this->type == BuildingType::SouthKing) {
		this->radius = KingTowerSize;
		this->health = KingTowerHealth;
	} else {
		this->radius = SmallTowerSize;
		this->health = smallTowerHalth;

	}

	this->isNorthBuilding = (this->type == BuildingType::NorthKing ||
							 this->type == BuildingType::NorthLeftTower ||
							 this->type == BuildingType::NorthRightTower);
}

int Building::attack(int dmg) {
	health += dmg;
	if (this->isDead()) { GameState::removeBuilding(this); }
	return health;
}

std::shared_ptr<Point> Building::getPosition() {
	return std::shared_ptr<Point>(&(this->pos));
}

float Building::GetSize() const
{
	return this->radius;
}