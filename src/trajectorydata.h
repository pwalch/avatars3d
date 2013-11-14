/*
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

/**
 * @brief Position and rotation data container
 *
 * Contains position and rotation values of a Moveable over time.
 * Positions are stored in virtual coordinates.
 *
 * @see Moveable
 */
class TrajectoryData {

    public:

        /**
         * Instanciates a trajectory data object with its trajectory
         * @param virtualTrajectory positions of the object
         * @param virtualRotation rotations of the object
         */
        TrajectoryData(const std::map < int, vector3df > & virtualTrajectory,
                       const std::map < int, vector3df > & virtualRotation);

        /**
         * Returns whether position map contains a position for a given time index
         * @param time frame index
         * @return true if time corresponds to a value, false else
         */
        bool containsPositionFrame(int time) const;

        /**
         * Returns whether rotation map contains a rotation for a given time index
         * @param time frame index
         * @return true if time corresponds to a value, false else
         */
        bool containsRotationFrame(int time) const;

        /**
         * Returns position of the object at a given time index
         * @param time frame index
         * @return position
         */
        vector3df getPositionAt(int time) const;

        /**
         * Returns rotation of the object at a given time index
         * @param time frame index
         * @return position
         */
        vector3df getRotationAt(int time) const;

        /**
         * Returns position map
         * @return position map
         */
        const std::map < int, vector3df > & getVirtualTrajectory() const;

        /**
         * Returns rotation map
         * @return rotation map
         */
        const std::map < int, vector3df > & getVirtualRotation() const;

        /**
         * Sets position for a given time index
         * @param time frame index
         * @param value position vector
         */
        void setPositionAt(int time, const vector3df& value);

        /**
         * Sets rotation for a given time index
         * @param time frame index
         * @param value rotation vector
         */
        void setRotationAt(int time, const vector3df& value);

        /**
         * Returns first valid time index for position
         * @return frame index
         */
        int getBeginIndex() const;

        /**
         * Returns last valid time index for position
         * @return frame index
         */
        int getEndIndex() const;

    private:
        std::map < int, vector3df > mVirtualTrajectory;
        std::map < int, vector3df > mVirtualRotation;
};

#endif // TRAJECTORYDATA_H
