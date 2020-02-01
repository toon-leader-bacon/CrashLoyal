#pragma once

#include "GameState.h"
#include "Attackable.h"
#include "Point.h"

const int KingTowerSize = 50;
const int KingTowerHealth = 100;

const float SmallTowerSize = 30.0f;
const float smallTowerHalth = 50.0f;

const float KingX = GAME_GRID_WIDTH / 2;
const float PrincessLeftX = GAME_GRID_WIDTH / 4;
const float PrincessRightX = GAME_GRID_WIDTH - PrincessLeftX;

const float NorthPrincessY = GAME_GRID_HEIGHT * (3 / 20.0);
const float NorthKingY = GAME_GRID_HEIGHT * (1 / 20.0);
const float SouthPrincessY = GAME_GRID_HEIGHT - NorthPrincessY;
const float SouthKingY = GAME_GRID_HEIGHT - NorthKingY;

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

