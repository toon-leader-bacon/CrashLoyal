#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

class Mob;
class Building;

#include "Waypoint.h"
#include "Point.h"

//Screen dimension constants
const int PIXELS_PER_METER = 10; // There are 10 pixels per meter in this game

const int GAME_GRID_WIDTH = 60; // How many tiles wide the game grid is. Should be an even number
const int GAME_GRID_HEIGHT = 100; // How many tiles tall the game grid is. 
const int SCREEN_WIDTH = PIXELS_PER_METER * GAME_GRID_WIDTH;   // The total number of pixels wide the window is
const int SCREEN_HEIGHT = PIXELS_PER_METER * GAME_GRID_HEIGHT; // The total number of pixels tall the window is

// Definig sizes of the river rectangle
const float RIVER_LEFT_X = 0;
const float RIVER_RIGHT_X = GAME_GRID_WIDTH;
const float RIVER_TOP_Y = (GAME_GRID_HEIGHT / 2) - 1.5;
const float RIVER_BOT_Y = (GAME_GRID_HEIGHT / 2) + 1.5;

// Defining sizes of the bridges
const float BRIDGE_WIDTH = 3;
const float BRIDGE_HEIGHT = 3.5;
const float LEFT_BRIDGE_CENTER_X = GAME_GRID_WIDTH / 4.0;
const float LEFT_BRIDGE_CENTER_Y = GAME_GRID_HEIGHT / 2.0;

const float RIGHT_BRIDGE_CENTER_X = GAME_GRID_WIDTH - LEFT_BRIDGE_CENTER_X;
const float RIGHT_BRIDGE_CENTER_Y = LEFT_BRIDGE_CENTER_Y;

struct GameState
{
	
public:
	const static int WAYPOINT_COUNT = 22;
	static std::vector<std::shared_ptr<Waypoint>> waypoints;

	static std::vector<std::shared_ptr<Mob>> mobs;

	static std::unordered_set< std::shared_ptr<Building>> buildings;

	static bool removeBuilding(Building* m);

private :

	// Initialize the waypoints list.
	static std::vector<std::shared_ptr<Waypoint>> buildWaypoints();

	// Initialize the building lists
	static std::unordered_set<std::shared_ptr<Building>> buildBuildings();

};

