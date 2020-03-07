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

#include "Entity.h"

#include "MobStats.h"

class Vec2;
struct Waypoint;

class Mob : public Entity {

public:
    Mob(const iMobStats& stats, const Vec2& pos, bool isNorth);
    virtual ~Mob() {}

    bool operator==(const Mob& rhs) const { return m_Uid == rhs.m_Uid; }

    virtual bool isNorth() const { return m_bIsNorth; }
    
    const iMobStats& getStats() const { return m_Stats; }
    virtual int getMaxHealth() const { return m_Stats.getMaxHealth(); }
    virtual float getSize() const  { return m_Stats.getSize(); }

    // The main function that drives this mob. Should be called once every game tick.
    void update(float elapsedTime);

protected:
    // pick the attack and move targets (we may have both).
    void pickTargets();

    const Waypoint* findClosestWaypoint();
    void moveTowards(const Vec2& moveTarget, float elapsedTime);

    bool targetInRange();


    Mob* checkCollision();
    void processCollision(Mob* otherMob, float elapsedTime);

    void attackProcedure(float elapsedTime);
    void moveProcedure(float elapsedTime);

protected:
    static int s_PreviousUID;
    int m_Uid;

    bool m_bIsNorth;
    const iMobStats& m_Stats;

    Entity* m_pAttackTarget;
    const Waypoint* m_pMoveTarget;

    float m_LastAttackTime;
};
