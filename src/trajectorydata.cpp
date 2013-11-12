/**
  * 3D Avatars
  * Pierre Walch
  */


#include "trajectorydata.h"

TrajectoryData::TrajectoryData(std::map < int, vector3df > & virtualTrajectory,
               std::map < int, vector3df > & virtualRotation) {
    this->mVirtualTrajectory = virtualTrajectory;
    this->mVirtualRotation = virtualRotation;
}

bool TrajectoryData::isPositionContained(int time) const {
    return mVirtualTrajectory.find(time) != mVirtualTrajectory.end();
}

bool TrajectoryData::isRotationContained(int time) const {
    return mVirtualRotation.find(time) != mVirtualRotation.end();
}

vector3df TrajectoryData::getPositionAt(int time) const {
    if(isPositionContained(time))
        return mVirtualTrajectory.at(time);
    else
        return vector3df(0, 0, 0);
}

vector3df TrajectoryData::getRotationAt(int time) const {
    if(isRotationContained(time))
        return mVirtualRotation.at(time);
    else
        return vector3df(0, 0, 0);
}

const std::map < int, vector3df > & TrajectoryData::getVirtualTrajectory() const {
    return mVirtualTrajectory;
}

const std::map < int, vector3df > & TrajectoryData::getVirtualRotation() const {
    return mVirtualRotation;
}

void TrajectoryData::setPositionAt(int time, const vector3df& value) {
    mVirtualTrajectory[time] = value;
}

void TrajectoryData::setRotationAt(int time, const vector3df& value) {
    mVirtualRotation[time] = value;
}

int TrajectoryData::getBeginIndex() const {
    return mVirtualTrajectory.begin()->first;
}

int TrajectoryData::getEndIndex() const {
    return mVirtualTrajectory.end()->first;
}
