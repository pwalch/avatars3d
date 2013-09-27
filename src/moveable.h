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

using namespace irr;
using namespace irr::core;
using namespace irr::video;

/**
 * @brief Abstract class representing a moving body on the court with its own trajectory and orientation.
 *
 * This class is a sub-part of the model in MVC pattern. It contains the actual trajectory data. mapTime() and prepareMove() must be called in this order before using other methods.
 */
class Moveable : public Timeable
{
    public:

        virtual ~Moveable();

        /**
         * Must be called before using the object. Call this method after having filled the positions with mapTime()
         * @param trajVisible initial visibility of trajectory curve
         * @param trajColor color of the trajectory
         * @param frameNumber number of frames in the tracking video
         * @param framerate framerate in tracking video
         * @param trajNbPoints number of points in trajectory color curve
         */
        void prepareMove(bool trajVisible, const SColor& trajColor, int frameNumber, int framerate, int trajNbPoints);

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
         * @param frameNumber number of frames in the tracking video
         * @param nbPoints number of points to use for N-points averager
         */
        void smooth(std::map < int, vector3df > & values, int frameNumber, int nbPoints);

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
         * @param interval speed interval for derivative computation
         * @return speedInterval map
         */
        std::map < int, vector3df > computeSpeed(std::map < int, vector3df > & trajectory, int frameNumber, int framerate, int speedInterval);

        /**
         * Returns a list of the last positions of the body, grouped by consecutive pair (move lines)
         * @param from starting index of the list
         * @param samples number of positions to add to the list
         * @return list of moves
         */
        std::vector< vector2d < vector3df > > lastMoves(int from, int samples);

        void setTime(int time);

        /**
         * Returns whether the trajectory color curve of this object is visible or not
         * @return visibility state
         */
        bool isTrajectoryVisible() const;

        /**
         * Sets whether the trajectory color curve of this object is visible or not
         * @param value new visibility state
         */
        void setTrajectoryVisible(bool value);

protected:
        // Side nodes
        ColorCurveNode* trajectoryNode;
        bool trjVisible;
        int trjNbPoints;

        // Movement attributes
        std::map < int, vector3df > virtualTrajectory;
        std::map < int, vector3df > rotationAngle;
};

#endif // MOVEABLE_H
