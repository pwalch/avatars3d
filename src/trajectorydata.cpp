/*
  * 3D Avatars
  * Pierre Walch
  */

#include "trajectorychunk.h"
#include "trajectorydata.h"


TrajectoryData::TrajectoryData() {

}

void TrajectoryData::updateWith(TrajectoryChunk *chunk)
{
    const std::map< int, vector3df >& positions = chunk->getPositions();
    for(std::map< int, vector3df >::const_iterator i = positions.begin();
        i != positions.end();
        ++i) {
        mVirtualPositions[i->first] = i->second;
    }

    const std::map< int, vector3df >& rotations = chunk->getRotations();
    for(std::map< int, vector3df >::const_iterator i = rotations.begin();
        i != rotations.end();
        ++i) {
        mVirtualRotation[i->first] = i->second;
    }
}

bool TrajectoryData::containsPositionFrame(int time) const {
    return mVirtualPositions.find(time) != mVirtualPositions.end();
}

bool TrajectoryData::containsRotationFrame(int time) const {
    return mVirtualRotation.find(time) != mVirtualRotation.end();
}

vector3df TrajectoryData::getPositionAt(int time) const {
    if(time < 0) {
        return vector3df(0, 0, 0);
    }

    if(containsPositionFrame(time))
        return mVirtualPositions.at(time);
    else
        return getPositionAt(time - 1);
}

vector3df TrajectoryData::getRotationAt(int time) const {
    if(time < 0) {
        return vector3df(0, 0, 0);
    }

    // If the frame is not available we try the previous one
    if(containsRotationFrame(time))
        return mVirtualRotation.at(time);
    else
        return getRotationAt(time - 1);
}

const std::map < int, vector3df > & TrajectoryData::getPositions() const {
    return mVirtualPositions;
}

const std::map < int, vector3df > & TrajectoryData::getRotations() const {
    return mVirtualRotation;
}

