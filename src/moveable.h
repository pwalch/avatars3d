/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef MOVEABLE_H
#define MOVEABLE_H

#include <irrlicht.h>
#include <map>
#include <vector>
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

        /**
         * Must be called before using the object. Call this method after
         * having filled the positions with mapTime()
         * @param settings object settings
         */
        void prepareMove(const MoveableSettings& settings);

        /**
         * Maps frame times to positions
         * @param time time corresponding to position
         * @param position position corresponding to time value
         * @param rotation rotation corresponding to time value
         */
        void mapTime(int time, vector3df position, vector3df rotation);

        /**
         * Smooths the values using a n-points averager
         * @param values values to smooth
         * @param nbPoints number of points to use for N-points averager
         */
        void smooth(std::map < int, vector3df > & values, int nbPoints);

        /**
         * Returns speed map of given trajectory using frame number
         * and framerate
         * @param trajectory trajectory from which to get the speed
         * @param interval speed interval for derivative computation
         * @return speedInterval map
         */
        std::map < int, vector3df > computeSpeed(
                std::map < int, vector3df > & trajectory,
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
        ColorCurveNode* trajectoryNode;
        MoveableSettings moveableSettings;

        // Movement attributes
        std::map < int, vector3df > virtualTrajectory;
        std::map < int, vector3df > rotationAngle;
};

#endif // MOVEABLE_H
