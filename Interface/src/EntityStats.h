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
    virtual int getDamage() const = 0;
    virtual float getAttackTime() const = 0;
    virtual const char* getDisplayLetter() const = 0;
};

class iEntityStats_Mob : public iEntityStats
{
public:
    // This should not be called
    virtual BuildingType getBuildingType() const { assert(false); return InvalidBuildingType; }
};

class EntityStats_Swordsman : public iEntityStats_Mob
{
public:
    virtual MobType getMobType() const { return Swordsman; }
    virtual float getElixirCost() const { return 3.f; }
    virtual int getMaxHealth() const { return 10; }
    virtual float getSpeed() const { return 3.f; }
    virtual float getSize() const { return 0.5f; }
    virtual float getMass() const { return 3.f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual float getAttackRange() const { return 1.f; }
    virtual int getDamage() const { return 3; }
    virtual float getAttackTime() const { return 1.4f; }
    const char* getDisplayLetter() const { return "S"; }
};

class EntityStats_Archer : public iEntityStats_Mob
{
public:
    virtual MobType getMobType() const { return Archer; }
    virtual float getElixirCost() const { return 2.f; }
    virtual int getMaxHealth() const { return 3; }
    virtual float getSpeed() const { return 5.0f; }
    virtual float getSize() const { return 0.4f; }
    virtual float getMass() const { return 2.f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual float getAttackRange() const { return 5.f; }
    virtual int getDamage() const { return 2; }
    virtual float getAttackTime() const { return 0.8f; }
    const char* getDisplayLetter() const { return "A"; }
};

class EntityStats_Giant : public iEntityStats_Mob
{
public:
    virtual MobType getMobType() const { return Giant; }
    virtual float getElixirCost() const { return 6.f; }
    virtual int getMaxHealth() const { return 30; }
    virtual float getSpeed() const { return 2.f; }
    virtual float getSize() const { return 0.9f; }
    virtual float getMass() const { return 8.f; }
    virtual TargetType getTargetType() const { return Building; }
    virtual float getAttackRange() const { return 1.5f; }
    virtual int getDamage() const { return 5; }
    virtual float getAttackTime() const { return 2.4f; }
    const char* getDisplayLetter() const { return "G"; }
};

class iEntityStats_Building : public iEntityStats
{
public:
    // These should not be called
    virtual MobType getMobType() const { assert(false); return InvalidMobType; }
    virtual float getElixirCost() const { assert(false); return FLT_MAX; }
    virtual float getSpeed() const { assert(false); return FLT_MAX; }
    virtual float getMass() const { assert(false); return FLT_MAX; }
};

class EntityStats_Princess : public iEntityStats_Building
{
public:
    virtual BuildingType getBuildingType() const { return Princess; }
    virtual int getMaxHealth() const { return 300; }
    virtual float getSize() const { return 2.f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual int getDamage() const { return 2; }
    virtual float getAttackRange() const { return 1.5f; }
    virtual float getAttackTime() const { return 2.4f; }
    const char* getDisplayLetter() const { return "P"; }
};

class EntityStats_King : public iEntityStats_Building
{
public:
    virtual BuildingType getBuildingType() const { return King; }
    virtual int getMaxHealth() const { return 500; }
    virtual float getSize() const { return 3.0f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual int getDamage() const { return 2; }
    virtual float getAttackRange() const { return 10.f; }
    virtual float getAttackTime() const { return 1.4f; }
    const char* getDisplayLetter() const { return "K"; }
};
