/*
  * 3D Avatars
  * Pierre Walch
  */

#ifndef MOVEABLE_H
#define MOVEABLE_H

#include <irrlicht.h>
#include <map>
#include <vector>
#include "trajectorydata.h"
#include "colorcurvenode.h"
#include "timeable.h"
#include "moveable.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;


/**
 * @brief Abstract moveable object on the court with its own trajectory and orientation.
 *
 * Contains the actual trajectory data. Provides methods to compute speeds from trajectories, and to compute
 * smoothed trajectory.
 */
class Moveable : public Timeable
{
    public:

        /**
         * Constructs an object by giving it a trajectory
         * @param trajectoryData trajectory
         */
        Moveable(TrajectoryData* trajectoryData);

        /**
         * Releases memory of trajectory data
         */
        virtual ~Moveable();

    protected:

        /**
         * Positions and rotation over time
         */
        TrajectoryData* mTrajectoryData;
};

#endif // MOVEABLE_H
