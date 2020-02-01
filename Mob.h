#ifndef __MOB_H__
#define __MOB_H__

#include <memory>
#include "Attackable.h"
#include "Building.h"
#include "Waypoint.h"
#include "Point.h"

enum MobState
{
	Moving,
	Attacking
};

class Mob : public Attackable {

public:

	MobState state;

	// Stats related
	// TODO: Make these private
	bool attackingNorth;
	int maxHealth;
	int currentHealth;
	float speed; // Represents how many pixels this mob moves each frame
	float size; // Represents the radius of the circle that is this mob's hitbox
				// Also is the side-length of the square that draws this mob to the screen
				// Units are in pixels
	            // TODO: currently Mob-to-Mob collision is not implimented

	// Movement related
	Point pos; // The position of this mob
	std::shared_ptr<Waypoint> nextWaypoint; // The default movement target if there are no enemies nearby
	std::shared_ptr<Point> targetPosition; // The actual place this mob is moving towards

	// Combat related
	int dmg = 2;
	bool targetLocked;
	std::shared_ptr<Attackable> target;
	int const attackCooldown = 200; // Number of frames that must pass before another attack can happen
	int lastAttackTime;           // How many frames ago was the last attack? 

	Mob(int x, int y, bool attackingNorth);

	// TODO: Destructor

	void update();

	// Call this function once per frame to update the Mob's position.
	// If this mob reaches it's target waypoint "nextWaypoint" it will
	// read the next waypoint and path towards it.
	void moveTowards(std::shared_ptr<Point> moveTarget);

	// Find the closest Waypoint to the mob and start moving towards it
	bool findClosestWaypoint();
	bool findAndSetAttackableMob();
	void findNewTarget();

	bool isDead() { return this->currentHealth <= 0; }

	int attack(int dmg);

	bool sameMob(Mob* otherMob) { return this->uuid == otherMob->uuid; }
	bool sameMob(std::shared_ptr<Mob> otherMob) { return this->uuid == otherMob->uuid; }

	std::shared_ptr<Point> getPosition();
	int getSize();

private: 

	static int previousUUID;

	int uuid;

	// Have this mob start moving towards the provided target
	// TODO: Impliment true pathfinding instead of just walking straight
	//       Consider storing a list of Point objects that is refreshed in the event of a collision
	void updateMoveTarget(std::shared_ptr<Point> target);

	void updateMoveTarget(Point target);


	void setNewWaypoint(std::shared_ptr<Waypoint> newWaypoint) {
		this->nextWaypoint = newWaypoint;
		this->updateMoveTarget(newWaypoint->pos);
	}

	// Push this unit away from the provided point
	void pushAway(Point awayFrom);

	std::shared_ptr<Building> checkBuildingCollision();

	void processBuildingCollision(std::shared_ptr<Building> b);

	std::shared_ptr<Mob> checkMobCollision();

	void processMobCollision(std::shared_ptr<Mob> otherMob);

	void attackProcedure();

	void moveProcedure();

	void setAttackTarget(std::shared_ptr<Attackable> b);

	bool targetInRange();

};

#endif
