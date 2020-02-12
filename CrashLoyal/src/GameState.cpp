#include "GameState.h"

#include <memory>
#include <vector>
#include <cmath>
#include "Building.h"
#include "Mob.h"
#include "Waypoint.h"
#include "Point.h"

std::vector<std::shared_ptr<Mob>> GameState::mobs;

std::unordered_set<std::shared_ptr<Building>> GameState::buildBuildings() {
	std::unordered_set<std::shared_ptr<Building>> result;
	
	// Kings
	std::shared_ptr<Building> northKing = std::make_shared<Building>(
		Building( Point(KingX, NorthKingY), BuildingType::NorthKing));
	std::shared_ptr<Building> southKing = std::make_shared<Building>(
		Building( Point(KingX, SouthKingY), BuildingType::SouthKing));
	result.insert(northKing);
	result.insert(southKing);

	// Northern Princesses
	std::shared_ptr<Building> northLeftPrincess = std::make_shared<Building>(
		Building( Point(PrincessLeftX, NorthPrincessY), BuildingType::NorthLeftTower));
	std::shared_ptr<Building> northRightPrincess = std::make_shared<Building>(
		Building( Point(PrincessRightX, NorthPrincessY), BuildingType::NorthRightTower));
	result.insert(northLeftPrincess);
	result.insert(northRightPrincess);

	// Southern Princesses
	std::shared_ptr<Building> southLeftPrincess = std::make_shared<Building>(
		Building( Point(PrincessLeftX, SouthPrincessY), BuildingType::SouthLeftTower));
	std::shared_ptr<Building> southRightPrincess = std::make_shared<Building>(
		Building( Point(PrincessRightX, SouthPrincessY), BuildingType::SouthRightTower));
	result.insert(southLeftPrincess);
	result.insert(southRightPrincess);


	return result;
}

std::unordered_set<std::shared_ptr<Building>> GameState::buildings = GameState::buildBuildings();

bool GameState::removeBuilding(Building* buildingToRemove) {
	for (std::shared_ptr<Building> b : GameState::buildings) {
		if (buildingToRemove->getType() == b->getType()) {
			GameState::buildings.erase(b);
			return true;
		}
	}
	return false;
}


std::vector<std::shared_ptr<Waypoint>> GameState::buildWaypoints() {
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

	std::vector<std::shared_ptr<Waypoint>> waypoints(22);

	float X_InitialPos = GAME_GRID_WIDTH / 4.0;

	float Y_InitialPos = GAME_GRID_HEIGHT / 4.0;
	float Y_Increment = GAME_GRID_HEIGHT / 10.0; // Split the middle 50% into 5 equal sections, waypoint before+after each slice

	// Build the waypoints for a lane
	for (int i = 0; i < 6; i++) {
		Point ptLeft;
		ptLeft.x = X_InitialPos;
		ptLeft.y = Y_InitialPos + (i * Y_Increment);
		std::shared_ptr<Waypoint> wpLeft = std::shared_ptr<Waypoint>(new Waypoint());
		wpLeft->pos = ptLeft;
		waypoints[19 - i] = wpLeft;

		Point ptRight;
		ptRight.x = GAME_GRID_WIDTH - X_InitialPos;
		ptRight.y = Y_InitialPos + (i * Y_Increment);
		std::shared_ptr<Waypoint> wpRight = std::shared_ptr<Waypoint>(new Waypoint());
		wpRight->pos = ptRight;
		waypoints[3 + i] = wpRight;
	}

	// Build the waypoints for the tower rows
	// Top Row
	Point princessTLpt;
	princessTLpt.x = PrincessLeftX;
	princessTLpt.y = NorthPrincessY;
	std::shared_ptr<Waypoint> princessTL = std::shared_ptr<Waypoint>(new Waypoint());
	princessTL->pos = princessTLpt;
	waypoints[20] = princessTL;

	Point princessTRpt;
	princessTRpt.x = PrincessRightX;
	princessTRpt.y = NorthPrincessY;
	std::shared_ptr<Waypoint> princessTR = std::shared_ptr<Waypoint>(new Waypoint());
	princessTR->pos = princessTRpt;
	waypoints[2] = princessTR;

	Point kingTopPt;
	kingTopPt.x = KingX;
	kingTopPt.y = NorthKingY;
	std::shared_ptr<Waypoint> kingTop = std::shared_ptr<Waypoint>(new Waypoint());
	kingTop->pos = kingTopPt;
	waypoints[0] = kingTop;

	std::shared_ptr<Waypoint> topLeftMid = std::shared_ptr<Waypoint>(new Waypoint());
	topLeftMid->pos = Point::midpoint(kingTopPt, princessTLpt);
	waypoints[21] = topLeftMid;

	std::shared_ptr<Waypoint> topRightMid = std::shared_ptr<Waypoint>(new Waypoint());
	topRightMid->pos = Point::midpoint(kingTopPt, princessTRpt);
	waypoints[1] = topRightMid;

	// Bot Row
	Point princessBLpt;
	princessBLpt.x = PrincessLeftX;
	princessBLpt.y = SouthPrincessY;
	std::shared_ptr<Waypoint> princessBL = std::shared_ptr<Waypoint>(new Waypoint());
	princessBL->pos = princessBLpt;
	waypoints[13] = princessBL;

	Point princessBRpt;
	princessBRpt.x = PrincessRightX;
	princessBRpt.y = SouthPrincessY;
	std::shared_ptr<Waypoint> princessBR = std::shared_ptr<Waypoint>(new Waypoint());
	princessBR->pos = princessBRpt;
	waypoints[9] = princessBR;

	Point kingBotPt;
	kingBotPt.x = KingX;
	kingBotPt.y = SouthKingY;
	std::shared_ptr<Waypoint> kingBot = std::shared_ptr<Waypoint>(new Waypoint());
	kingBot->pos = kingBotPt;
	waypoints[11] = kingBot;

	std::shared_ptr<Waypoint> botLeftMid = std::shared_ptr<Waypoint>(new Waypoint());
	botLeftMid->pos = Point::midpoint(kingBotPt, princessBLpt);
	waypoints[12] = botLeftMid;

	std::shared_ptr<Waypoint> botRightMid = std::shared_ptr<Waypoint>(new Waypoint());
	botRightMid->pos = Point::midpoint(kingBotPt, princessBRpt);
	waypoints[10] = botRightMid;


	// Link the waypoints correctly
	for (int i = 0; i < 11; i++) {
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


std::vector< std::shared_ptr<Waypoint>> GameState::waypoints = GameState::buildWaypoints();
