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

#include "Game.h"
#include "Entity.h"
#include "Vec2.h"
#include "Mob.h"

enum BuildingType {
    FirstNorthTower = 0,

    NorthKing = FirstNorthTower,
    NorthRightTower,
    NorthLeftTower,

    LastNorthTower = NorthLeftTower,

    FirstSouthTower,

    SouthKing = FirstSouthTower,
    SouthRightTower,
    SouthLeftTower,

    LastSouthTower = SouthLeftTower,

    NumBuildingTypes
};


class Building : public Entity {

public:

    Building(float x, float y,  BuildingType type);

    Building(Vec2 p, BuildingType type) : Building(p.x, p.y, type) { }

    virtual bool isNorth() const { return type <= BuildingType::LastNorthTower; }

    virtual float getSize() const { return size; }

    BuildingType getType() { return this->type; }

    void update(float elapsedTime);

    virtual int getMaxHealth() const { return m_MaxHealth; }

private:
    int m_MaxHealth;
    float size;
    float attackRadius;

    BuildingType type;

    Entity* target;  // Not owned, NULL => no current target
    float lastAttackTime;

    enum class BuildingState {
        Attacking,
        Scaning
    };
    BuildingState state;

    void attackProcedure(float elapsedTime);

    virtual int getDamage() const { return 2; }
    virtual float getAttackTime() const { return 2.f; }

    Entity* findTargetInRange();

    void scanProcedure(float elapsedTime);

    bool inAttackRange(Vec2 p);
};

