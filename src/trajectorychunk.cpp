#include <map>
#include "irrlicht.h"
#include "trajectorychunk.h"

using namespace irr::core;

TrajectoryChunk::TrajectoryChunk(const std::map<int, vector3df>& positions,
                                 const std::map<int, vector3df>& rotations)
{
    mPositions = positions;
    mRotations = rotations;
}

const std::map<int, vector3df> &TrajectoryChunk::getPositions() const
{
    return mPositions;
}

const std::map<int, vector3df> &TrajectoryChunk::getRotations() const
{
    return mRotations;
}

int TrajectoryChunk::getBeginIndex()
{
    return mPositions.begin()->first;
}
