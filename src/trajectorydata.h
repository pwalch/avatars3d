/*
  * 3D Avatars
  * Pierre Walch
  */


#ifndef TRAJECTORYDATA_H
#define TRAJECTORYDATA_H

#include <irrlicht.h>
#include <map>

#include "trajectorychunk.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;

/**
 * @brief Position and rotation data container
 *
 * Contains position and rotation values of a Moveable over time.
 * Positions are stored in virtual coordinates. We add positions and rotation by appending TrajectoryChunks.
 *
 * @see Moveable
 */
class TrajectoryData {

    public:

        /**
         * Instanciates a trajectory data object (empty before we add chunks)
         */
        TrajectoryData();

        /**
         * Appends a chunk of trajectory to the current data
         * @param chunk chunk to add
         */
        void updateWith(TrajectoryChunk* chunk);

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
        const std::map < int, vector3df > & getPositions() const;

        /**
         * Returns rotation map
         * @return rotation map
         */
        const std::map < int, vector3df > & getRotations() const;

    private:
        std::map < int, vector3df > mVirtualPositions;
        std::map < int, vector3df > mVirtualRotation;
};

#endif // TRAJECTORYDATA_H
