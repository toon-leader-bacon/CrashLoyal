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

#include "iPlayer.h"

#include "Constants.h"
#include <algorithm>
#include <assert.h>

class iController;
class Entity;

class Player : public iPlayer {
public:
    // NOTE: we take ownership of the controller
    explicit Player(iController* pControl, bool bNorth);
    virtual ~Player();

    virtual bool isNorth() const { return m_bNorth; }

    virtual float getElixir() const { return (float)m_Elixir; }
    virtual const std::vector<iEntityStats::MobType>& GetAvailableMobTypes() const { return m_AvailableMobs; }
    virtual PlacementResult placeMob(iEntityStats::MobType type, const Vec2& pos);

    void tick(float deltaTSec);

    const std::vector<Entity*>& getBuildings() const { return m_Buildings; }
    const std::vector<Entity*>& getMobs() const { return m_Mobs; }

    virtual unsigned int getNumBuildings() const { return m_Buildings.size(); }
    virtual EntityData getBuilding(unsigned int i) const;

    virtual unsigned int getNumMobs() const { return m_Mobs.size(); }
    virtual EntityData getMob(unsigned int i) const;

    virtual unsigned int getNumOpponentBuildings() const { return GetOpponent().getNumBuildings(); }
    virtual EntityData getOpponentBuilding(unsigned int i) const;

    virtual unsigned int getNumOpponentMobs() const { return GetOpponent().getNumMobs(); }
    virtual EntityData getOpponentMob(unsigned int i) const;

private:
    void buildBuildings();

    const Player& GetOpponent() const;

    float capElixir(float e) const { return std::max(e, MAX_ELIXIR); }

private:
    iController* m_pControl;                // owned, may be NULL

    bool m_bNorth;
    float m_Elixir;

    std::vector<iEntityStats::MobType> m_AvailableMobs;

    std::vector<Entity*> m_Buildings;       // owned
    std::vector<Entity*> m_Mobs;            // owned

    // When mobs die, we move them to this vector.  For now we just hang on to 
    // them forever - we never delete them - so as to avoid memory issues.
    std::vector<Entity*> m_DeadMobs;        // owned

};
