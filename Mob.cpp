#include "Mob.h"

#include <limits>
#include <stdio.h>
#include "Waypoint.h"
#include "GameState.h"
#include "Point.h"

Mob::Mob(int x, int y, bool attackingNorth) {
	this->pos.x = x;
	this->pos.y = y;
	this->attackingNorth = attackingNorth;
	this->speed = 0.3;
	findClosestWaypoint();

	// TODO: change these vars
	size = 5.0;
}


void Mob::moveTowards() {
	this->pos.x += movementVector.x;
	this->pos.y += movementVector.y;
	//printf("Mob(%f, %f)  nextWaypoint(%f, %f)  \n", pos.x, pos.y, this->nextWaypoint->pos.x, this->nextWaypoint->pos.y);

	if (this->nextWaypoint->pos.insideOf(this->pos, this->size)) {
		Waypoint* trueNextWP = this->attackingNorth ?
			                   this->nextWaypoint->upNeighbor :
			                   this->nextWaypoint->downNeighbor;
		updateMoveTarget(trueNextWP);
	}

}

void Mob::findClosestWaypoint() {
	Waypoint* closestWP = GameState::waypoints[0];
	float smallestDist = std::numeric_limits<float>::max();

	for (int i = 0; i < GameState::WAYPOINT_COUNT; i++) {
		Waypoint* wp = GameState::waypoints[i];
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
	this->movementVector.x = target.x - this->pos.x;
	this->movementVector.y = target.y - this->pos.y;
	this->movementVector.normalize();
	this->movementVector.multiply(this->speed);
}