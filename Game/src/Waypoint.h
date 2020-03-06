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

#include <memory>
#include "Constants.h"
#include "Vec2.h"

/*
The structure of the returned Waypoint array is as follows. The numbers indicate the position the
waypoint exists in the list. The location on the grid represents the position the waypoint exists
in the game world.
 +---------------+
|     21  0  1    |
|    20         2  |
|                  |
|   19         3  |
|    18         4  |
|    17         5  |
|    16         6  |
|    15         7  |
|    14         8  |
|                  |
|    13         9  |
|     12  11  10   |
 +---------------+

Each waypoint is linked such that the upNeighbor always points towards waypoint 0 (the enemy king)
while the downNeighbor always points towards waypoint 11 (the friendly king).
The enemy king (node 0) has an upNeighbor of 21 and a downNeighbor of 1.
Likewise the friendly king (node 11) has an upNeighbor of 12 and a downNeighbor of 10.

Waypoints 0 and 11 represent king towers
Waypoints 20, 2, 10 and 12 represent Princess towers.

*/

struct Waypoint {
    Vec2 m_Pos;  // The coordinates (in pixels) this waypoint represents
    const Waypoint* m_UpNeighbor;
    const Waypoint* m_DownNeighbor;
};

