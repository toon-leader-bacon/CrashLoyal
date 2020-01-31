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

	this->targetPosition = std::shared_ptr<Point>(new Point());
	this->state = MobState::Moving;
	findClosestWaypoint();


	// TODO: change these vars
	size = 5.0;
	targetLocked = false;
}

std::shared_ptr<Point> Mob::getPosition() {
	return std::shared_ptr<Point>(&(this->pos));
}

int Mob::getSize() {
	return this->size;
}

bool Mob::findClosestWaypoint() {
	std::shared_ptr<Waypoint> closestWP = GameState::waypoints[0];
	float smallestDist = std::numeric_limits<float>::max();

	for (std::shared_ptr<Waypoint> wp : GameState::waypoints) {
		//std::shared_ptr<Waypoint> wp = GameState::waypoints[i];
		// Filter out any waypoints that are "behind" us (behind is relative to attack dir
		// Remember y=0 is in the top left
		if (attackingNorth && wp->pos.y > this->pos.y) {
			continue;
		}
		else if ((!attackingNorth) && wp->pos.y < this->pos.y) {
			continue;
		}

		float dist = this->pos.dist(wp->pos);
		if (dist < smallestDist) {
			printf("found closer\n");
			smallestDist = dist;
			closestWP = wp;
		}
	}
	std::shared_ptr<Point> newTarget = std::shared_ptr<Point>(new Point);
	this->targetPosition->x = closestWP->pos.x;
	this->targetPosition->y = closestWP->pos.y;
	this->nextWaypoint = closestWP;
	
	return true;
}

void Mob::moveTowards(std::shared_ptr<Point> moveTarget) {
	Point movementVector;
	movementVector.x = moveTarget->x - this->pos.x;
	movementVector.y = moveTarget->y - this->pos.y;
	movementVector.normalize();
	movementVector.multiply(this->speed);
	this->pos.x += movementVector.x;
	this->pos.y += movementVector.y;
}


void Mob::findNewTarget() {
	// Find a new valid target to move towards and update this mob
	// to start pathing towards it

	if (!findAndSetAttackableMob()) { findClosestWaypoint(); }
}

// Have this mob start aiming towards the provided target
// TODO: impliment true pathfinding here
void Mob::updateMoveTarget(std::shared_ptr<Point> target) {
	this->targetPosition->x = target->x;
	this->targetPosition->y = target->y;
}

void Mob::updateMoveTarget(Point target) {
	this->targetPosition->x = target.x;
	this->targetPosition->y = target.y;
}


// Movement related
//////////////////////////////////
// Combat related

int Mob::attack(int dmg) {
	this->currentHealth -= dmg;
	if (this->isDead()) {
		GameState::removeMob(this);
	}
	return currentHealth;
}

bool Mob::findAndSetAttackableMob() {
	// Find an attackable target that's in the same quardrant as this Mob
	// If a target is found, this function returns true
	// If a target is found then this Mob is updated to start attacking it
	for (std::shared_ptr<Mob> otherMob : GameState::mobs) {
		if (otherMob->attackingNorth == this->attackingNorth) { continue; }

		if (((this->pos.x < (SCREEN_WIDTH / 2)) != (otherMob->pos.x < (SCREEN_WIDTH / 2))) ||
			((this->pos.y < (SCREEN_HEIGHT / 2)) != (otherMob->pos.y < (SCREEN_HEIGHT / 2)))) {
			// If we're in the same quardrant as the otherMob
			// Mark it as the new target
			this->setAttackTarget(otherMob);
			return true;
		}
	}
	return false;
}

// TODO Move this somewhere better like a utility class
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

bool Mob::targetInRange(std::shared_ptr<Attackable> possibleTarget) {
	std::shared_ptr<Point> targetPos = possibleTarget->getPosition();

	int range = this->size; // TODO: change this for ranged units
	return this->pos.insideOf(*targetPos, (range + possibleTarget->getSize()));
}
// Combat related
////////////////////////////////////////////////////////////
// Collisions

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

// Collisions
///////////////////////////////////////////////
// Procedures

void Mob::attackProcedure() {
	if (this->target == nullptr || this->target->isDead()) {
		this->targetLocked = false;
		this->target = nullptr;
		this->state = Moving;
		return;
	}

	if (targetInRange(target)) {
		if (this->lastAttackTime >= this->attackCooldown) {
			// If our last attack was longer ago than our cooldown
			this->target->attack(this->dmg);
			this->lastAttackTime = 0;
			return;
		}

		this->lastAttackTime += 1;
	}
	else {
		// If the target is not in range
		moveTowards(target->getPosition());
	}

}

void Mob::moveProcedure() {
	if (targetPosition) {
		moveTowards(targetPosition);

		// Check for collisions
		if (this->nextWaypoint->pos.insideOf(this->pos, (this->size + WAYPOINT_SIZE))) {
			std::shared_ptr<Waypoint> trueNextWP = this->attackingNorth ?
												   this->nextWaypoint->upNeighbor :
												   this->nextWaypoint->downNeighbor;
			setNewWaypoint(trueNextWP);
		}

		// Fighting otherMob takes priority always
		findAndSetAttackableMob();

	} else {
		// if targetPosition is nullptr
		findNewTarget();
	}
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
