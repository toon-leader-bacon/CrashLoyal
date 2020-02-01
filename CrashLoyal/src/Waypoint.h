#pragma once

#include <memory>
#include "Point.h"

/*
The structure of the returned Waypoint array is as follows. The numbers indicate the position the
waypoint exists in the list. The location on the grid represents the position the waypoint exists
in the game world.
 +---------------+
|     21  0  1    |
|	20         2  |
|	              |
|   19         3  |
|	18         4  |
|	17         5  |
|	16         6  |
|	15         7  |
|	14         8  |
|	              |
|	13         9  |
|	 12  11  10   |
 +---------------+

Each waypoint is linked such that the upNeighbor always points towards waypoint 0 (the enemy king)
while the downNeighbor always points towards waypoint 11 (the friendly king).
The enemy king (node 0) has an upNeighbor of 21 and a downNeighbor of 1.
Likewise the friendly king (node 11) has an upNeighbor of 12 and a downNeighbor of 10.

Waypoints 0 and 11 represent king towers
Waypoints 20, 2, 10 and 12 represent Princess towers.

*/

const int NORTH_KING_WAYPOINT_INDEX = 0;
const int NORTH_RIGHT_TOWER_WAYPOINT_INDEX = 2;
const int NORTH_LEFT_TOWER_WAYPOINT_INDEX = 20;

const int SOUTH_KING_WAYPOINT_INDEX = 11;
const int SOUTH_RIGHT_TOWER_WAYPOINT_INDEX = 9;
const int SOUTH_LEFT_TOWER_WAYPOINT_INDEX = 13;

const float WAYPOINT_SIZE = 1;
struct Waypoint {
	Point pos;  // The coordinates (in pixels) this waypoint represents
	std::shared_ptr<Waypoint> upNeighbor;
	std::shared_ptr<Waypoint> downNeighbor;
};

