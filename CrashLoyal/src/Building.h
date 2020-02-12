#pragma once

#include "GameState.h"
#include "Attackable.h"
#include "Point.h"
#include "Mob.h"

const float KingTowerSize = 5.0f;
const int KingTowerHealth = 100;

const float SmallTowerSize = 3.f;
const int SmallTowerHealth = 50.0f;

const float KingTowerAttackRadius = 20.f;
const float SmallTowerAttackRadius = 20.f;

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

class Building {

public:

	bool isNorthBuilding;


	Building(float x, float y,  BuildingType type);

	Building(Point p, BuildingType type) : Building(p.x, p.y, type) { }

	bool isDead() { return this->health >= 0; }

	int attack(int dmg);

	std::shared_ptr<Point> getPosition();

	float GetSize() const;

	Point getPoint() const { return this->pos; }

	BuildingType getType() { return this->type; }

	void update(double elapsedTime);

private:
	int health;
	Point pos;
	float size;
	float attackRadius;

	BuildingType type;

	Attackable* target;  // May be null => no current target
	float lastAttackTime;


	enum class BuildingState {
		Attacking,
		Scaning
	};
	BuildingState state;

	void attackProcedure(double elapsedTime);

	virtual int GetDamage() const { return 2; }
	virtual float GetAttackTime() const { return 0.15f; }

	Attackable* findTargetInRange();

	void scanProcedure(double elapsedTime);




};

