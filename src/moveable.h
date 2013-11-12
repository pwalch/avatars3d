/**
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
#include "moveablesettings.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;


/**
 * @brief Abstract class representing a moving body on the court with its
 * own trajectory and orientation.
 *
 * This class is a sub-part of the model in MVC pattern. It contains the
 * actual trajectory data.
 * mapTime() and prepareMove() must be called in this order
 * before using other methods.
 */
class Moveable : public Timeable
{
    public:

        virtual ~Moveable();

        Moveable(TrajectoryData* trajectoryData, const MoveableSettings& moveableSettings, bool hasColorCurve);

        /**
         * Smooths the values using a n-points averager
         * @param values values to smooth
         * @param nbPoints number of points to use for N-points averager
         */
        static void smooth(std::map < int, vector3df > & values, int nbPoints);

        /**
         * Returns speed map of given trajectory using frame number
         * and framerate
         * @param trajectory trajectory from which to get the speed
         * @param interval speed interval for derivative computation
         * @return speedInterval map
         */
        static std::map < int, vector3df > computeSpeed(
                const TrajectoryData& trajectoryData,
                int interval);

        /**
         * Returns a list of the last positions of the body,
         * grouped by consecutive pair (move lines)
         * @param from starting index of the list
         * @param samples number of positions to add to the list
         * @return list of moves
         */
        std::vector< vector2d < vector3df > > lastMoves(int from, int samples);

        void setTime(int time);

protected:
        // Side nodes
        ColorCurveNode* mTrajectoryNode;
        MoveableSettings mMoveableSettings;

        // Movement attributes
        TrajectoryData* mTrajectoryData;
};

#endif // MOVEABLE_H
