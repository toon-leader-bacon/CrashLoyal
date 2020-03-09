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

#include <cmath>
#include <iostream>
#include <stdio.h>

class Vec2 {
public:
    float x;
    float y;

    Vec2() : x(-FLT_MAX), y(-FLT_MAX) {}
    Vec2(int inX, int inY) : x((float)inX), y((float)inY) {}
    Vec2(float inX, float inY) : x(inX), y(inY) {}
    Vec2(const Vec2& rhs) : x(rhs.x), y(rhs.y) {}

    bool operator==(const Vec2& rhs) const { return (x == rhs.x) && (y == rhs.y); }
    bool operator!=(const Vec2& rhs) const { return (x != rhs.x) || (y != rhs.y); }

    Vec2& operator=(const Vec2& rhs) { x = rhs.x; y = rhs.y; return *this; }

    Vec2 operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }

    Vec2 operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    Vec2& operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

    Vec2 operator*(const float f) const { return Vec2(x * f, y * f); }
    Vec2& operator*=(const float f) { x *= f; y *= f; return *this; }

    Vec2 operator/(const float f) const { return Vec2(x / f, y / f); }
    Vec2& operator/=(const float f) { x /= f; y /= f; return *this; }

    float lengthSqr() const { return x * x + y * y; }
    float length() const { return sqrt(lengthSqr()); }

    float distSqr(Vec2 other) const { return (other - *this).lengthSqr(); }
    float dist(Vec2 other) const { return (other - *this).length(); }

    // Returns the previous length.  If the length is too short to normalize,
    //  sets the vector to (0,0) and returns 0.
    float normalize();

    // Final Project: These helper functions can convert between Player coordinates
    // (which have the player's towers at the y=0 end of the field) and Game 
    // coordinates (with the north player on the y = 0 side of the field) 
    Vec2 Player2Game(bool bPlayerIsNorth) const;

private:
    // DELIBERATELY UNDEFINED
    bool operator<(const Vec2& rhs) const;
};

