#ifndef __MOB_H__
#define __MOB_H__

#include "Waypoint.h"
#include "Point.h"


class Mob {

public:

	// Stats related
	bool attackingNorth;
	float speed; // Represents how many pixels this mob moves each frame
	float size; // Represents the radius of the circle that is this mob's hitbox
				// Also is the side-length of the square that draws this mob to the screen
				// Units are in pixels
	            // TODO: currently Mob-to-Mob collision is not implimented

	// Movement related
	Point pos;
	Waypoint* nextWaypoint;
	Point movementVector;

	Mob(int x, int y, bool attackingNorth);

	// TODO: Destructor

	// Call this function once per frame to update the Mob's position.
	// If this mob reaches it's target waypoint "nextWaypoint" it will
	// read the next waypoint and path towards it.
	void moveTowards();

	// Find the closest Waypoint to the mob and start moving towards it
	void findClosestWaypoint();


private: 

	// Have this mob start moving towards the provided target
	// TODO: Impliment true pathfinding instead of just walking straight
	//       Consider storing a list of Point objects that is refreshed in the event of a collision
	void updateMoveTarget(Point target);

	// Have this move start moving towards the provided target
	void updateMoveTarget(Waypoint* wp) { 
		this->updateMoveTarget(wp->pos); 
		this->nextWaypoint = wp;
	}

};

#endif
