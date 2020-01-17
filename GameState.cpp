#include "GameState.h"

#include <vector>
#include <cmath>
#include "Mob.h"
#include "Waypoint.h"
#include "Point.h"

std::unordered_set<Mob*> GameState::mobs;

std::vector<Waypoint*> GameState::buildWaypoints() {
	/*
	  The structure of the returned Waypoint array is as follows. The numbers indicate the position the 
	  waypoint exists in the list. The location on the grid represents the position the waypoint exists
	  in the game world.
	 +----------------+
	 |    21  0  1    |
	 |	20         2  |
	 |	              |
	 |  19         3  |
	 |	18         4  |
	 |	17         5  |
	 |	16         6  |
	 |	15         7  |
	 |	14         8  |
	 |	              |
	 |	13         9  |
	 |	 12  11  10   |
	 +----------------+

	 Each waypoint is linked such that the upNeighbor always points towards waypoint 0 (the enemy king)
	 while the downNeighbor always points towards waypoint 11 (the friendly king).
	 The enemy king (node 0) has an upNeighbor of 21 and a downNeighbor of 1.
	 Likewise the friendly king (node 11) has an upNeighbor of 12 and a downNeighbor of 10.

	 Waypoints 0 and 11 represent king towers
	 Waypoints 20, 2, 10 and 12 represent Princess towers.

	 */

	std::vector<Waypoint*> waypoints(22);

	float X_InitialPos = SCREEN_WIDTH / 4.0;

	float Y_InitialPos = SCREEN_HEIGHT / 4.0;
	float Y_Increment = SCREEN_HEIGHT / 10.0; // Split the middle 50% into 5 equal sections, waypoint before+after each slice

	// Build the waypoints for a lane
	for (int i = 0; i < 6; i++) {
		Point ptLeft;
		ptLeft.x = X_InitialPos;
		ptLeft.y = Y_InitialPos + (i * Y_Increment);
		Waypoint* wpLeft = new Waypoint();
		wpLeft->pos = ptLeft;
		waypoints[19 - i] = wpLeft;

		Point ptRight;
		ptRight.x = SCREEN_WIDTH - X_InitialPos;
		ptRight.y = (SCREEN_HEIGHT - Y_InitialPos) - (i * Y_Increment);
		Waypoint* wpRight = new Waypoint();
		wpRight->pos = ptRight;
		waypoints[3 + i] = wpRight;
	}

	// Build the waypoints for the tower rows
	int princessYValue = Y_InitialPos - Y_Increment;
	int kingYValue = princessYValue - Y_Increment;
	int X_Increment = SCREEN_WIDTH / 8.0; // Split the middle 50% into 4 equal parts, waypoints before + after each slice

	// Top Row
	Point princessTLpt;
	princessTLpt.x = X_InitialPos;
	princessTLpt.y = princessYValue;
	Waypoint* princessTL = new Waypoint();
	princessTL->pos = princessTLpt;
	waypoints[20] = princessTL;

	Point princessTRpt;
	princessTRpt.x = SCREEN_WIDTH - X_InitialPos;
	princessTRpt.y = princessYValue;
	Waypoint* princessTR = new Waypoint();
	princessTR->pos = princessTRpt;
	waypoints[2] = princessTR;

	Point kingTopPt;
	kingTopPt.x = SCREEN_WIDTH / 2;
	kingTopPt.y = kingYValue;
	Waypoint* kingTop = new Waypoint();
	kingTop->pos = kingTopPt;
	waypoints[0] = kingTop;

	Waypoint* topLeftMid = new Waypoint();
	topLeftMid->pos = Point::midpoint(kingTopPt, princessTLpt);
	waypoints[21] = topLeftMid;

	Waypoint* topRightMid = new Waypoint();
	topRightMid->pos = Point::midpoint(kingTopPt, princessTRpt);
	waypoints[1] = topRightMid;

	// Bot Row
	Point princessBLpt;
	princessBLpt.x = X_InitialPos;
	princessBLpt.y = SCREEN_HEIGHT - princessYValue;
	Waypoint* princessBL = new Waypoint();
	princessBL->pos = princessBLpt;
	waypoints[13] = princessBL;

	Point princessBRpt;
	princessBRpt.x = SCREEN_WIDTH - X_InitialPos;
	princessBRpt.y = SCREEN_HEIGHT - princessYValue;
	Waypoint* princessBR = new Waypoint();
	princessBR->pos = princessBRpt;
	waypoints[9] = princessBR;

	Point kingBotPt;
	kingBotPt.x = SCREEN_WIDTH / 2;
	kingBotPt.y = SCREEN_HEIGHT - kingYValue;
	Waypoint* kingBot = new Waypoint();
	kingBot->pos = kingBotPt;
	waypoints[11] = kingBot;

	Waypoint* botLeftMid = new Waypoint();
	botLeftMid->pos = Point::midpoint(kingBotPt, princessBLpt);
	waypoints[12] = botLeftMid;

	Waypoint* botRightMid = new Waypoint();
	botRightMid->pos = Point::midpoint(kingBotPt, princessBRpt);
	waypoints[10] = botRightMid;


	// Link the waypoints correctly
	for (int i = 0; i <= 11; i++) {
		// Right side of the map
		waypoints[i]->upNeighbor = waypoints[abs((i - 1) % 22)];
		waypoints[i]->downNeighbor = waypoints[i + 1];

		// Left side of the map
		int mirroredIndex = 21 - i;
		waypoints[mirroredIndex]->upNeighbor = waypoints[(mirroredIndex + 1) % 22];
		waypoints[mirroredIndex]->downNeighbor = waypoints[mirroredIndex - 1];
	}

	return waypoints;
}


std::vector<Waypoint*> GameState::waypoints = GameState::buildWaypoints();
