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

// Final Project:  A player is one of the two players in the game - either the 
// north or the south player.  We have two APIs - iPlayer, which is what you'll
// be able to access for your own player, and iOpposingPlayer which is what 
// you'll be able to access for the opposing player.

#include "EntityStats.h"
#include "Vec2.h"

#include <vector>

enum MobType;

class iPlayer {
public:
    iPlayer() {}
    virtual ~iPlayer() {}

    // Final Project: What side are you playing?  If true, you're the top
    // player (i.e. red).  You'll need to account for this when doing things
    // like placing units, so that you place them on the right side.
    virtual bool isNorth() const = 0;

    // Final Project: Call this to find out how much elixir you currently have.
    // You can look in Constants.h to see how much you start with, what the 
    // maximum is and how fast it accumulates.
    virtual float getElixir() const = 0;

    // Final Project: This vector tells you what types of mobs are currently 
    // available to be placed.
    // TODO: Implement this feature - right now it's all of them!
    virtual const std::vector<iEntityStats::MobType>& GetAvailableMobTypes() const = 0;

    enum PlacementResult
    {
        Success = 0,
        InsufficientElixir,
        InvalidX,
        InvalidY,
        MobTypeUnavailable,
    };

    // Final Project: Your AI will call this function to place its mobs.  The
    // return value will tell you whether it succeeded and why it failed.
    virtual PlacementResult placeMob(iEntityStats::MobType type, const Vec2& pos) = 0;

private:
    // DELIBERATELY UNDEFINED
    iPlayer(const iPlayer& rhs);
    iPlayer& operator=(const iPlayer& rhs);
    bool operator==(const iPlayer& rhs) const;
    bool operator<(const iPlayer& rhs) const;
};

