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

#include "EntityStats.h"

#include "Vec2.h"

class Entity
{

public:
    Entity(const iEntityStats& stats, const Vec2& pos, bool isNorth);

    const iEntityStats& getStats() const { return m_Stats; }

    bool isNorth() const { return m_bIsNorth; }

    bool isDead() const { return m_Health <= 0; }
    int getHealth() const { return m_Health; }
    void takeDamage(int dmg) { m_Health -= dmg; }

    const Vec2& getPosition() const { return m_Pos; }

protected:
    const iEntityStats& m_Stats;
    bool m_bIsNorth;
    int m_Health;
    Vec2 m_Pos;
};




