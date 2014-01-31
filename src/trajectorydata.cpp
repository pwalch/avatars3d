/*
  * 3D Avatars
  * Pierre Walch
  */

#include "trajectorydata.h"


TrajectoryData::TrajectoryData(
               const std::map < int, vector3df > & virtualTrajectory,
               const std::map < int, vector3df > & virtualRotation) {
    this->mVirtualTrajectory = virtualTrajectory;
    this->mVirtualRotation = virtualRotation;
}

bool TrajectoryData::containsPositionFrame(int time) const {
    return mVirtualTrajectory.find(time) != mVirtualTrajectory.end();
}

bool TrajectoryData::containsRotationFrame(int time) const {
    return mVirtualRotation.find(time) != mVirtualRotation.end();
}

vector3df TrajectoryData::getPositionAt(int time) const {
    if(containsPositionFrame(time))
        return mVirtualTrajectory.at(time);
    else
        return vector3df(0, 0, 0);
}

vector3df TrajectoryData::getRotationAt(int time) const {
    if(containsRotationFrame(time))
        return mVirtualRotation.at(time);
    else
        return vector3df(0, 0, 0);
}

const std::map < int, vector3df > & TrajectoryData::getVirtualPositions() const {
    return mVirtualTrajectory;
}

const std::map < int, vector3df > & TrajectoryData::getVirtualRotations() const {
    return mVirtualRotation;
}

