#ifndef SCIENCE_H
#define SCIENCE_H

#include <map>
#include "irrlicht.h"
#include "affinetransformation.h"

using namespace irr;
using namespace irr::core;

/**
 * Class only with static methods, to compute various expressions.
 */
class Science
{
    public:

        /**
         * Smooths the values using a n-points moving average
         * @param values values to smooth
         * @param nbPoints number of points to use for average
         */
        static std::map < int, vector3df > computeEverySmooth(const std::map < int, vector3df > & values, int nbPoints);

        /**
         * Computes speed by derivating positions of given trajectory.
         * @see SequenceSettings
         * @see Engine
         *
         * @param values positions of which to compute speed
         * @param interval interval (in number of frames) for derivative computation
         * @param framerate frame rate to use to compute speed
         * @return speed map
         */
        static std::map < int, vector3df > computeEverySpeed(const std::map < int, vector3df >& values,
                                                        int interval,
                                                        int framerate);

        /**
         * Computes and smoothes a virtual speed.
         * @see computeEverySpeed()
         * @param virtualPosition real position of which to compute speed
         * @param interval interval (in number of frames) for derivative computation
         * @param nbPointsAverager number of points for moving average
         * @param framerate frame rate to use to compute speed
         * @return virtual speed map
         */
        static std::map < int, vector3df > computeEveryVirtualSpeed(const std::map < int, vector3df >& virtualPosition,
                                                                    int interval,
                                                                    int nbPointsAverager,
                                                                    int framerate);

        /**
         * Computes and smoothes a real speed
         * @param virtualPosition virtual position of which to compute speed
         * @param interval interval (in number of frames) for derivative computation
         * @param nbPointsAverager number of points for moving average
         * @param tfm affine transformation to convert virtual positions to real positions
         * @param framerate frame rate to use to compute speed
         * @return real speed map
         */
        static std::map < int, vector3df > computeEveryRealSpeed(const std::map < int, vector3df >& virtualPosition,
                                                                    int interval,
                                                                    int nbPointsAverager,
                                                                    AffineTransformation* tfm,
                                                                    int framerate);

        /**
         * Computes rotation using speeds deduced from positions
         * @param virtualPosition virtual position of which to compute speed
         * @param interval interval (in number of frames) for derivative computation
         * @param nbPointsAverager number of points for moving average
         * @param framerate frame rate to use to compute speed
         * @return rotation map
         */
        static std::map < int, vector3df > computeEveryVirtualRotation(
                                                                    const std::map < int, vector3df >& virtualPosition,
                                                                    int interval,
                                                                    int nbPointsAverager,
                                                                    int framerate);

};

#endif // SCIENCE_H
