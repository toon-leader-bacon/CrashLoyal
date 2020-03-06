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

#include <float.h>

//Screen dimension constants
const int PIXELS_PER_METER = 10; // There are 10 pixels per meter in this game

const int GAME_GRID_WIDTH = 60; // How many tiles wide the game grid is. Should be an even number
const int GAME_GRID_HEIGHT = 100; // How many tiles tall the game grid is. 

const int SCREEN_WIDTH_PIXELS = PIXELS_PER_METER * GAME_GRID_WIDTH;   // The total number of pixels wide the window is
const int SCREEN_HEIGHT_PIXELS = PIXELS_PER_METER * GAME_GRID_HEIGHT; // The total number of pixels tall the window is

// Definig sizes of the river rectangle
const float RIVER_LEFT_X = 0;
const float RIVER_RIGHT_X = GAME_GRID_WIDTH;
const float RIVER_TOP_Y = ((float)GAME_GRID_HEIGHT / 2.f) - 1.5f;
const float RIVER_BOT_Y = ((float)GAME_GRID_HEIGHT / 2.f) + 1.5f;

// Defining sizes of the bridges
const float BRIDGE_WIDTH = 3.f;
const float BRIDGE_HEIGHT = 3.5f;

const float LEFT_BRIDGE_CENTER_X = (float)GAME_GRID_WIDTH / 4.0f;
const float LEFT_BRIDGE_CENTER_Y = (float)GAME_GRID_HEIGHT / 2.0f;

const float RIGHT_BRIDGE_CENTER_X = (float)GAME_GRID_WIDTH - LEFT_BRIDGE_CENTER_X;
const float RIGHT_BRIDGE_CENTER_Y = LEFT_BRIDGE_CENTER_Y;

// Defining the tower positions
const float KingX = (float)GAME_GRID_WIDTH / 2.f;
const float PrincessLeftX = (float)GAME_GRID_WIDTH / 4.f;
const float PrincessRightX = (float)GAME_GRID_WIDTH - PrincessLeftX;

const float NorthPrincessY = (float)GAME_GRID_HEIGHT * (3.f / 20.f);
const float NorthKingY = (float)GAME_GRID_HEIGHT * (1.f / 20.f);
const float SouthPrincessY = (float)GAME_GRID_HEIGHT - NorthPrincessY;
const float SouthKingY = (float)GAME_GRID_HEIGHT - NorthKingY;

// Waypoints
const int NORTH_KING_WAYPOINT_INDEX = 0;
const int NORTH_RIGHT_TOWER_WAYPOINT_INDEX = 2;
const int NORTH_LEFT_TOWER_WAYPOINT_INDEX = 20;

const int SOUTH_KING_WAYPOINT_INDEX = 11;
const int SOUTH_RIGHT_TOWER_WAYPOINT_INDEX = 9;
const int SOUTH_LEFT_TOWER_WAYPOINT_INDEX = 13;

const float WAYPOINT_SIZE = 1.f;

// Tick limitations
const float TICK_MIN = 0.05f;
const float TICK_MAX = 0.2f;

// Elixir

const float STARTING_ELIXIR = 8.f;
const float ELIXIR_PER_SECOND = 0.33333f;
const float MAX_ELIXIR = 10.01f; // a little bit more than 10, to avoid float error
