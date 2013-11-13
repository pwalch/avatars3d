/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef TRAJECTORYDATA_H
#define TRAJECTORYDATA_H

#include <irrlicht.h>
#include <map>

using namespace irr;
using namespace irr::core;
using namespace irr::video;

class TrajectoryData {

    public:
        TrajectoryData(std::map < int, vector3df > & virtualTrajectory,
                       std::map < int, vector3df > & virtualRotation);

        bool isPositionContained(int time) const;
        bool isRotationContained(int time) const;

        vector3df getPositionAt(int time) const;
        vector3df getRotationAt(int time) const;

        const std::map < int, vector3df > & getVirtualTrajectory() const;
        const std::map < int, vector3df > & getVirtualRotation() const;

        void setPositionAt(int time, const vector3df& value);
        void setRotationAt(int time, const vector3df& value);

        int getBeginIndex() const;
        int getEndIndex() const;

    private:
        std::map < int, vector3df > mVirtualTrajectory;
        std::map < int, vector3df > mVirtualRotation;
};

#endif // TRAJECTORYDATA_H
