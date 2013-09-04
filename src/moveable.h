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

using namespace irr;
using namespace irr::core;
using namespace irr::video;

/**
 * Abstract class representing a moving body on the court with its own trajectory and orientation. mapTime() and prepareMove() must be called in this order before using other methods.
 */
class Moveable
{
    public:
        /**
         * Must be called before using the object. Call this method after having filled the positions with mapTime()
         * @param trajColor color of the trajectory
         * @param frameNumber number of frames in the tracking video
         * @param framerate framerate in tracking video
         */
        void prepareMove(bool trajVisible, const SColor& trajColor, int frameNumber, int framerate);

        /**
         * Maps frame times to positions
         * @param time time corresponding to position
         * @param position corresponding to time
         */
        void mapTime(int time, vector3df position, vector3df rotation);

        /**
         * Smooths the values using a n-points averager
         * @param values values to smooth
         * @param frameNumber number of frames in the tracking video
         */
        void smooth(std::map < int, vector3df > & values, int frameNumber);

        /**
         * Computes angle using trajectory
         * @param frameNumber frame number in the tracking video
         * @param framerate framerate in the tracking video
         */
        virtual void process(int frameNumber, int framerate);

        /**
         * Returns speed map of given trajectory using frame number and framerate
         * @param trajectory trajectory from which to get the speed
         * @param frameNumber number of frames in the tracking video
         * @param framerate framerate in the tracking video
         * @return speed map
         */
        std::map < int, vector3df > computeSpeed(std::map < int, vector3df > & trajectory, int frameNumber, int framerate);

        /**
         * Returns a list of the last positions of the body, grouped by consecutive pair (move lines)
         * @param from starting index of the list
         * @param samples number of positions to add to the list
         * @return list of moves
         */
        std::vector< vector2d < vector3df > > lastMoves(int from, int samples);

        /**
         * Adapts the position of the model to the wanted time value
         * @param time wanted frame time
         */
        virtual void setTime(int time) = 0;


    protected:
        // Side nodes
        ColorCurveNode* trajectoryNode;
        bool isTrajectoryVisible;

        // Movement attributes
        std::map < int, vector3df > virtualTrajectory;
        std::map < int, vector3df > rotationAngle;
};

#endif // MOVEABLE_H
