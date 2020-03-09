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

#include "iPlayer.h"

const Vec2 ksInvalidPos;


iPlayer::EntityData::EntityData()
    : m_Stats(iEntityStats::getStats(iEntityStats::InvalidMobType))
    , m_Health(INT_MIN)
    , m_Position(ksInvalidPos)
{
}

iPlayer::EntityData::EntityData(const iEntityStats& stats, const int& health, const Vec2& pos)

    : m_Stats(stats)
    , m_Health(health)
    , m_Position(pos)
{
}

iPlayer::EntityData::EntityData(const EntityData& rhs)
    : m_Stats(rhs.m_Stats)
    , m_Health(rhs.m_Health)
    , m_Position(rhs.m_Position)
{
}


