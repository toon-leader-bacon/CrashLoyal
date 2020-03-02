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

#include "Vec2.h"

float Vec2::distSqr(Vec2 a, Vec2 b) {
	const float xDist = b.x - a.x;
	const float yDist = b.y - a.y;
	return xDist * xDist + yDist * yDist;
}

float Vec2::dist(Vec2 a, Vec2 b) {
	return sqrt(distSqr(a, b));
}

float Vec2::distSqr(Vec2 other) const {
	return Vec2::distSqr(*this, other);
}

float Vec2::dist(Vec2 other) const {
	return Vec2::dist(*this, other);
}

void Vec2::normalize() {
	float mag = sqrt((x * x) + (y * y));

	float nearZero = 0.00001f;
	if (mag <= nearZero) { return; }

	this->x = this->x / mag;
	this->y = this->y / mag;
}

bool Vec2::insideOf(const Vec2& a, float aRadius, Vec2 b) {
	return dist(a, b) < aRadius;
}

bool Vec2::insideOf(const Vec2& other, float radius) const {
	return insideOf(*this, radius, other);
}