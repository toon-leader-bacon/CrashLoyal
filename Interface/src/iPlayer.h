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

// Final Project:  A player is one of the two players in the game - either the 
// north or the south player.  This API defines the interface which controllers 
// (e.g. the UI, a student's AI, etc.) use to query the state of the player
// and to control what that player does (i.e. place mobs).

#pragma once

#include "EntityStats.h"
#include "Vec2.h"

enum MobType;

class iPlayer {
public:
    iPlayer() {}
    virtual ~iPlayer() {}

    // Final Project: Call this to find out how much elixir you currently have.
    // You can look in Constants.h to see how much you start with, what the 
    // maximum is and how fast it accumulates.
    virtual float getElixir() const = 0;

    enum PlacementResult
    {
        Success = 0,
        InsufficientElixir,
        InvalidX,
        InvalidY,
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

//class iOpposingPlayer {
//public:
//    iOpposingPlayer() {}
//    virtual ~iOpposingPlayer() {}
//
//    // Final Project: Call this to find out how much elixir you currently have.
//    // You can look in Constants.h to see how much you start with, what the 
//    // maximum is and how fast it accumulates.
//    virtual float getElixir() const = 0;
//
//    size_t getNumMobs() const;
//    const iMob* getMob(size_t index) const;
//    const iBuilding* getBuilding(size_t index) const;
//
//private:
//    // DELIBERATELY UNDEFINED
//    iOpposingPlayer(const iOpposingPlayer& rhs);
//    iOpposingPlayer& operator=(const iOpposingPlayer& rhs);
//    bool operator==(const iOpposingPlayer& rhs) const;
//    bool operator<(const iOpposingPlayer& rhs) const;
//};
