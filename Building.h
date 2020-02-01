#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "GameState.h"
#include "Attackable.h"
#include "Point.h"

const int KingTowerSize = 30;
const int KingTowerHealth = 100;

const int SmallTowerSize = 20;
const int smallTowerHalth = 50;

const int KingX = SCREEN_WIDTH / 2;
const int PrincessLeftX = SCREEN_WIDTH / 4;
const int PrincessRightX = SCREEN_WIDTH - PrincessLeftX;

const int NorthPrincessY = SCREEN_HEIGHT * (3 / 20.0);
const int NorthKingY = SCREEN_HEIGHT * (1 / 20.0);
const int SouthPrincessY = SCREEN_HEIGHT - NorthPrincessY;
const int SouthKingY = SCREEN_HEIGHT - NorthKingY;

enum BuildingType {
	NorthKing,
	NorthRightTower,
	NorthLeftTower,

	SouthKing,
	SouthRightTower,
	SouthLeftTower
};

class Building : public Attackable {

public:
	int health;
	Point pos;
	float radius;

	BuildingType type;
	bool isNorthBuilding;

	Building(int x, int y,  BuildingType type);

	Building(Point p, BuildingType type) : Building(p.x, p.y, type) { }

	bool isDead() { return this->health >= 0; }

	int attack(int dmg);

	std::shared_ptr<Point> getPosition();

	int getSize();

private:


};

#endif
