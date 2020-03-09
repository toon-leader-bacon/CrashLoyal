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

#include "EntityStats.h"

#include <assert.h>
#include <unordered_map>

class EntityStats_Swordsman : public iEntityStats_Mob
{
public:
    virtual MobType getMobType() const { return Swordsman; }
    virtual float getElixirCost() const { return 3.f; }
    virtual int getMaxHealth() const { return 1452; }
    virtual float getSpeed() const { return 3.f; }
    virtual float getSize() const { return 0.5f; }
    virtual float getMass() const { return 3.f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual float getAttackRange() const { return 0.2f; }
    virtual DamageType getDamageType() const { return Melee; }
    virtual int getDamage() const { return 167; }
    virtual float getAttackTime() const { return 1.2f; }
    virtual const char* getName() const { return "Swordsman"; }
    virtual const char* getDisplayLetter() const { return "S"; }
};

class EntityStats_Archer : public iEntityStats_Mob
{
public:
    virtual MobType getMobType() const { return Archer; }
    virtual float getElixirCost() const { return 2.f; }
    virtual int getMaxHealth() const { return 216; }
    virtual float getSpeed() const { return 5.0f; }
    virtual float getSize() const { return 0.4f; }
    virtual float getMass() const { return 2.f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual float getAttackRange() const { return 6.5f; }
    virtual DamageType getDamageType() const { return Ranged; }
    virtual int getDamage() const { return 100; }
    virtual float getAttackTime() const { return 0.7f; }
    virtual const char* getName() const { return "Archer"; }
    virtual const char* getDisplayLetter() const { return "A"; }
};

class EntityStats_Giant : public iEntityStats_Mob
{
public:
    virtual MobType getMobType() const { return Giant; }
    virtual float getElixirCost() const { return 5.f; }
    virtual int getMaxHealth() const { return 3275; }
    virtual float getSpeed() const { return 2.f; }
    virtual float getSize() const { return 0.9f; }
    virtual float getMass() const { return 8.f; }
    virtual TargetType getTargetType() const { return Building; }
    virtual float getAttackRange() const { return .5f; }
    virtual DamageType getDamageType() const { return Melee; }
    virtual int getDamage() const { return 211; }
    virtual float getAttackTime() const { return 1.5f; }
    virtual const char* getName() const { return "Giant"; }
    virtual const char* getDisplayLetter() const { return "G"; }
};

class EntityStats_Princess : public iEntityStats_Building
{
public:
    virtual BuildingType getBuildingType() const { return Princess; }
    virtual int getMaxHealth() const { return 2534; }
    virtual float getSize() const { return 2.5f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual int getDamage() const { return 90; }
    virtual float getAttackRange() const { return 7.5f; }
    virtual float getAttackTime() const { return 0.8f; }
    virtual const char* getName() const { return "Princess Tower"; }
    virtual const char* getDisplayLetter() const { return "P"; }
};

class EntityStats_King : public iEntityStats_Building
{
public:
    virtual BuildingType getBuildingType() const { return King; }
    virtual int getMaxHealth() const { return 4008; }
    virtual float getSize() const { return 3.5f; }
    virtual TargetType getTargetType() const { return Any; }
    virtual int getDamage() const { return 90; }
    virtual float getAttackRange() const { return 7.f; }
    virtual float getAttackTime() const { return 1.f; }
    virtual const char* getName() const { return "King Tower"; }
    virtual const char* getDisplayLetter() const { return "K"; }
};

class EntityStats_Invalid : public iEntityStats_Mob
{
public:
    virtual MobType getMobType() const { return InvalidMobType; }
    virtual BuildingType getBuildingType() const { return InvalidBuildingType; }
    virtual float getElixirCost() const { return FLT_MAX; }
    virtual int getMaxHealth() const { return INT_MAX; }
    virtual float getSpeed() const { return FLT_MAX; }
    virtual float getSize() const { return FLT_MAX; }
    virtual float getMass() const { return FLT_MAX; }
    virtual TargetType getTargetType() const { return Any; }
    virtual float getAttackRange() const { return FLT_MAX; }
    virtual DamageType getDamageType() const { return Melee; }
    virtual int getDamage() const { return INT_MAX; }
    virtual float getAttackTime() const { return FLT_MAX; }
    virtual const char* getName() const { return "Invalid"; }
    virtual const char* getDisplayLetter() const { return ""; }
};

const iEntityStats& iEntityStats::getStats(MobType t)
{
    // NOTE: This vector must be in synch with the MobType enum (in the .h)
    static std::vector<const iEntityStats*> sStats = { 
        new EntityStats_Swordsman, 
        new EntityStats_Archer,
        new EntityStats_Giant
    };

    // If any of these fail, then your vector (above) is out of synch with the 
    //  MobType enum (in the .h)... and bad things may ensue!
    assert(sStats.size() == numMobTypes);
    assert(!!sStats[t]);
    assert(sStats[t]->getMobType() == t);

    if ((size_t)t < sStats.size())
    {
        return *sStats[t];
    }

    static const EntityStats_Invalid ksInvalidStats;
    return ksInvalidStats;
}


const iEntityStats& iEntityStats::getBuildingStats(BuildingType t)
{
    // NOTE: This vector must be in synch with the MobType enum (in the .h)
    static std::vector<const iEntityStats*> sStats = {
        new EntityStats_Princess,
        new EntityStats_King
    };

    // If any of these fail, then your vector (above) is out of synch with the 
    //  MobType enum (in the .h)... and bad things may ensue!
    assert(sStats.size() == numBuildingTypes);
    assert(!!sStats[t]);
    assert(sStats[t]->getBuildingType() == t);

    if ((size_t)t < sStats.size())
    {
        return *sStats[t];
    }

    static const EntityStats_Invalid ksInvalidStats;
    return ksInvalidStats;
}

