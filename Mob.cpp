#include "Mob.h"

#include <memory>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include "Building.h"
#include "Waypoint.h"
#include "GameState.h"
#include "Point.h"

int Mob::previousUUID;

Mob::Mob(int x, int y, bool attackingNorth) {
	Mob::previousUUID = 1 + Mob::previousUUID;
	this->uuid = Mob::previousUUID;

	this->pos.x = x;
	this->pos.y = y;
	this->attackingNorth = attackingNorth;
	this->speed = 0.06;
	this->maxHealth = 10;
	this->currentHealth = 10;
	findClosestWaypoint();

	// TODO: change these vars
	size = 5.0;
	targetLocked = false;

}

int Mob::attack(int dmg) {
	this->currentHealth -= dmg;
	if (this->isDead()) {
		GameState::removeMob(this);
	}
	return currentHealth;
}

void Mob::moveTowards() {
	Point* movementVector = new Point();
	movementVector->x = this->targetPosition.x - this->pos.x;
	movementVector->y = this->targetPosition.y - this->pos.y;
	movementVector->normalize();
	movementVector->multiply(this->speed);

	pos.x += movementVector->x;
	pos.y += movementVector->y;
	delete movementVector;
}

void Mob::findClosestWaypoint() {
	std::shared_ptr<Waypoint> closestWP = GameState::waypoints[0];
	float smallestDist = std::numeric_limits<float>::max();

	for (int i = 0; i < GameState::WAYPOINT_COUNT; i++) {
		std::shared_ptr<Waypoint> wp = GameState::waypoints[i];
		float dist = this->pos.dist(wp->pos);
		if (dist < smallestDist) {
			smallestDist = dist;
			closestWP = wp;
		}
	}

	this->updateMoveTarget(closestWP);
}


// Have this mob start aiming towards the provided target
// TODO: impliment true pathfinding here
void Mob::updateMoveTarget(Point target) {
	this->targetPosition = target;
}

int randomNumber(int minValue, int maxValue) {
	// Retunrs a random number between [min, max). Min is inclusive, max is not.
	return (rand() % maxValue) + minValue;
}

void Mob::pushAway(Point awayFrom) {
	// TODO: Consider making a little random noise when pushing to avoid walking direcly into a push
	int deltaX = (awayFrom.x - this->pos.x) + randomNumber(0, 10) / 20;
	int deltaY = (awayFrom.y - this->pos.y) + randomNumber(0, 10) / 20;
	Point* p = new Point(deltaX, deltaY);
	p->normalize();
	p->multiply(this->speed * -1);
	this->pos.x += p->x;
	this->pos.y += p->y;
	delete p;
}

void Mob::setAttackTarget(std::shared_ptr<Attackable> newTarget) {
	this->state = MobState::Attacking;
	target = newTarget;
}

std::shared_ptr<Building> Mob::checkBuildingCollision() {
	// Returns a refrence to the building this mob is currently collided with
	// If the mob is not hitting a building, returns nullptr
	for (std::shared_ptr<Building> b : GameState::buildings) {
		if (this->pos.insideOf(b->pos, (b->radius + this->size))) {
			return b;
		}
	}
	return std::shared_ptr<Building>(nullptr);
}

void Mob::processBuildingCollision(std::shared_ptr<Building> b) {
	if (this->attackingNorth != b->isNorthBuilding) {
		// Mob collided with friendly building
		this->pushAway(b->pos);
	} else {
		// Mob collided with enemy building
		this->setAttackTarget(b);
	}
}

std::shared_ptr<Mob> Mob::checkMobCollision() {
	for (std::shared_ptr<Mob> otherMob : GameState::mobs) {
		if (this->sameMob(otherMob)) { continue; }
		if (this->pos.insideOf(otherMob->pos, (this->size + otherMob->size))) {
			return otherMob;
		}
	}
	return std::shared_ptr<Mob>(nullptr);
}

void Mob::processMobCollision(std::shared_ptr<Mob> otherMob) {
	if (otherMob->attackingNorth == this->attackingNorth) {
		// Mob collided with friendly mob
		otherMob->pushAway(this->pos);
	} else {
		// this mob collided with enemy Mob
		this->state = Attacking;
		this->setAttackTarget(otherMob);
	}
}

void Mob::attackProcedure() {
	if (this->target->isDead()) {
		this->targetLocked = false;
		this->target = nullptr;
		this->state = Moving;
	}

	if (this->lastAttackTime >= this->attackCooldown) {
		// If our last attack was longer ago than our cooldown
		printf("Attacking\n");
		this->target->attack(this->dmg);
		this->lastAttackTime = 0;
		return;
	}

	this->lastAttackTime += 1;
}

void Mob::moveProcedure() {
	std::shared_ptr<Building> b = checkBuildingCollision();
	if (b != nullptr) {
		// If this mob has hit a building
		processBuildingCollision(b);
		return;
	}

	std::shared_ptr<Mob> otherMob = checkMobCollision();
	if (otherMob != nullptr) {
		// If this mob has run into another mob
		processMobCollision(otherMob);
		return;
	}

	if (this->nextWaypoint->pos.insideOf(this->pos, (this->size + WAYPOINT_SIZE))) {
		std::shared_ptr<Waypoint> trueNextWP = this->attackingNorth ?
			this->nextWaypoint->upNeighbor :
			this->nextWaypoint->downNeighbor;
		updateMoveTarget(trueNextWP);
	}

	// Else no collisions => move in straight line towards target
	moveTowards();
}


void Mob::update() {

	switch (this->state) {
	case Attacking:
		this->attackProcedure();
		break;
	case Moving:
	default:
		this->moveProcedure();
		break;
	}
}
