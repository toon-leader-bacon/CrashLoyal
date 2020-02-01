#pragma once

#include "GameState.h"
#include "Attackable.h"
#include "Point.h"

const int KingTowerSize = 30;
const int KingTowerHealth = 100;

const float SmallTowerSize = 20.0f;
const float smallTowerHalth = 50.0f;

const float KingX = SCREEN_WIDTH / 2;
const float PrincessLeftX = SCREEN_WIDTH / 4;
const float PrincessRightX = SCREEN_WIDTH - PrincessLeftX;

const float NorthPrincessY = SCREEN_HEIGHT * (3 / 20.0);
const float NorthKingY = SCREEN_HEIGHT * (1 / 20.0);
const float SouthPrincessY = SCREEN_HEIGHT - NorthPrincessY;
const float SouthKingY = SCREEN_HEIGHT - NorthKingY;

enum class BuildingType {
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

	Building(float x, float y,  BuildingType type);

	Building(Point p, BuildingType type) : Building(p.x, p.y, type) { }

	bool isDead() { return this->health >= 0; }

	int attack(int dmg);

	std::shared_ptr<Point> getPosition();

	float GetSize() const;

private:


};

