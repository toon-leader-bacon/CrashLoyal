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

// Final Project: The interfaces in this file let your AI determine what types 
// of units exist, and what the characteristics are for each one.  The actual
// values are in EntityStats.cpp. 

#include <assert.h>
#include <limits>

// Stats that each mob needs to have.  
class iEntityStats
{
public:
    // NOTE: This enum must be in synch with the vector (in 
    // iEntityStats::getStats())
    enum MobType
    {
        Swordsman,
        Archer,
        Giant,

        numMobTypes,

        InvalidMobType
    };

    // NOTE: This enum must be in synch with the vector (in 
    // iEntityStats::getBuildingStats())
    enum BuildingType
    {
        Princess,
        King,

        numBuildingTypes,

        InvalidBuildingType
    };

    enum TargetType
    {
        Any,
        Building,
    };

    enum DamageType
    {
        Melee,
        Ranged,
    };

    // Final Project: call these functions to get the stats for any type of 
    // mob/building.
    static const iEntityStats& getStats(MobType t);
    static const iEntityStats& getBuildingStats(BuildingType t);

    virtual MobType getMobType() const = 0;
    virtual BuildingType getBuildingType() const = 0;

    virtual float getElixirCost() const = 0;
    virtual int getMaxHealth() const = 0;
    virtual float getSpeed() const = 0;
    virtual float getSize() const = 0;
    virtual float getMass() const = 0;
    virtual TargetType getTargetType() const = 0;
    virtual float getAttackRange() const = 0;
    virtual DamageType getDamageType() const = 0;
    virtual int getDamage() const = 0;
    virtual float getAttackTime() const = 0;
    virtual const char* getName() const = 0;
    virtual const char* getDisplayLetter() const = 0;
};

class iEntityStats_Mob : public iEntityStats
{
public:
    // This should not be called
    virtual BuildingType getBuildingType() const { assert(false); return InvalidBuildingType; }
};

class iEntityStats_Building : public iEntityStats
{
public:
    // These should not be called
    virtual MobType getMobType() const { assert(false); return InvalidMobType; }
    virtual float getElixirCost() const { assert(false); return FLT_MAX; }
    virtual float getSpeed() const { assert(false); return FLT_MAX; }
    virtual float getMass() const { assert(false); return FLT_MAX; }
    virtual DamageType getDamageType() const { return Ranged; }
};
