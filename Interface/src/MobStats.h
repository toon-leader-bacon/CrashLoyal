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

enum MobType
{
    Swordsman,
    Archer,

    numMobTypes
};

// Stats that each mob needs to have.  
class iMobStats
{
public:
    // Final Project: call this function to get the stats for any type of mob.
    static const iMobStats& getStats(MobType t);

    virtual MobType getType() const = 0;
    virtual float getElixirCost() const = 0;
    virtual int getMaxHealth() const = 0;
    virtual float getSpeed() const = 0;
    virtual float getSize() const = 0;
    virtual float getMass() const = 0;
    virtual int getDamage() const = 0;
    virtual float getAttackRange() const = 0;
    virtual float getAttackTime() const = 0;
    virtual const char* getDisplayLetter() const = 0;
};

class MobStats_Swordsman : public iMobStats
{
public:
    virtual MobType getType() const { return Swordsman; }
    virtual float getElixirCost() const { return 5.f; }
    virtual int getMaxHealth() const { return 10; }
    virtual float getSpeed() const { return 3.f; }
    virtual float getSize() const { return 1.f; }
    virtual float getMass() const { return 3.f; }
    virtual int getDamage() const { return 3; }
    virtual float getAttackRange() const { return 1.f; }
    virtual float getAttackTime() const { return 2.f; }
    const char* getDisplayLetter() const { return "S"; }
};


class MobStats_Archer : public iMobStats
{
public:
    virtual MobType getType() const { return Archer; }
    virtual float getElixirCost() const { return 3.f; }
    virtual int getMaxHealth() const { return 4; }
    virtual float getSpeed() const { return 5.0f; }
    virtual float getSize() const { return 0.5f; }
    virtual float getMass() const { return 1.f; }
    virtual int getDamage() const { return 1; }
    virtual float getAttackRange() const { return 5.f; }
    virtual float getAttackTime() const { return 1.0f; }
    const char* getDisplayLetter() const { return "A"; }
};