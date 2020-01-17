#ifndef __WAYPOINT_H__
#define __WAYPOINT_H__

#include "Point.h"

struct Waypoint
{
	Point pos;  // The coordinates (in pixels) this waypoint represents
	Waypoint* upNeighbor;
	Waypoint* downNeighbor;
};

#endif
