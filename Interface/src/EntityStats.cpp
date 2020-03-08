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

#include "EntityStats.h"

#include <assert.h>
#include <unordered_map>

const iEntityStats& iEntityStats::getStats(MobType t)
{
    // NOTE: This vector must be in synch with the MobType enum (in the .h)
    static std::vector<const iEntityStats*> sStats = { 
        new EntityStats_Swordsman, 
        new EntityStats_Archer,
        new EntityStats_Giant
    };

    // If any of these fail, then your vector (above) is out of synch with the 
    //  MobType enum (in the .h)... and bad things may ensue!
    assert(sStats.size() == numMobTypes);
    assert(!!sStats[t]);
    assert(sStats[t]->getMobType() == t);

    return *sStats[t];
}


const iEntityStats& iEntityStats::getBuildingStats(BuildingType t)
{
    // NOTE: This vector must be in synch with the MobType enum (in the .h)
    static std::vector<const iEntityStats*> sStats = {
        new EntityStats_Princess,
        new EntityStats_King
    };

    // If any of these fail, then your vector (above) is out of synch with the 
    //  MobType enum (in the .h)... and bad things may ensue!
    assert(sStats.size() == numBuildingTypes);
    assert(!!sStats[t]);
    assert(sStats[t]->getBuildingType() == t);

    return *sStats[t];
}

