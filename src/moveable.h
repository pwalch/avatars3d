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
         * Smooths the values using a n-points averager
         * @param values values to smooth
         * @param nbPoints number of points to use for N-points averager
         */
        static std::map < int, vector3df > smooth(const std::map < int, vector3df > & values, int nbPoints);

        /**
         * Computes speed by derivating positions of given trajectory. Uses frame rate of the current sequence,
         * retrieved from SequenceSettings object of Engine.
         * @see SequenceSettings
         * @see Engine
         *
         * @param trajectoryData trajectory of which to compute speed
         * @param interval interval (in number of frames) for derivative computation
         * @return speed data
         */
        static std::map < int, vector3df > computeSpeed(const TrajectoryData& trajectoryData, int interval);

        /**
         * Positions and rotation over time
         */
        TrajectoryData* mTrajectoryData;
};

#endif // MOVEABLE_H
