#ifndef __GAMESTATE__
#define __GAMESTATE__

#include <memory>
#include <vector>;
#include <unordered_set>

class Mob;
class Building;

#include "Waypoint.h";
#include "Point.h";


//Screen dimension constants
const int GAME_GRID_SIZE = 10; // A single in game grid is a square with this side length
const int GAME_GRID_WIDTH = 30; // How many tiles wide the game grid is. Should be an even number
const int GAME_GRID_HEIGHT = 50; // How many tiles tall the game grid is. 
const int SCREEN_WIDTH = GAME_GRID_SIZE * GAME_GRID_WIDTH;   // The total number of pixels wide the window is
const int SCREEN_HEIGHT = GAME_GRID_SIZE * GAME_GRID_HEIGHT; // The total number of pixels tall the window is

struct GameState
{
	
public:
	const static int WAYPOINT_COUNT = 22;
	static std::vector< std::shared_ptr<Waypoint>> waypoints;

	static std::unordered_set< std::shared_ptr<Mob>> mobs;

	static std::unordered_set< std::shared_ptr<Building>> buildings;

	static bool removeMob(Mob* m);
	static bool removeBuilding(Building* m);

private :

	// Initialize the waypoints list.
	static std::vector<std::shared_ptr<Waypoint>> buildWaypoints();

};

#endif
