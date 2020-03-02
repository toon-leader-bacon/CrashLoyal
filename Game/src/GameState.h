// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "Singleton.h"

#include <vector>

#include "Vec2.h"
#include "Waypoint.h"

class Mob;

class Building;
enum BuildingType;


class GameState : public Singleton<GameState>
{
public:
	explicit GameState();
	virtual ~GameState();

	void tick(double deltaTSec);

	Building* getBuilding(BuildingType b) { return m_Buildings[(size_t)b]; }

	const std::vector<Waypoint*>& getWaypoints() const { return m_Waypoints; }
	const std::vector<Mob*>& getMobs() const { return m_Mobs; }

	void addMob(Mob* mob) { m_Mobs.push_back(mob); }	// takes ownership

private:
	// Helpers for the constructor
	void buildWaypoints();
	void buildBuildings();

private:
	std::vector<Waypoint*> m_Waypoints;			// owned
	std::vector<Building*> m_Buildings;			// owned
	std::vector<Mob*> m_Mobs;					// owned
};

