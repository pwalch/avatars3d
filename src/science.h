#ifndef SCIENCE_H
#define SCIENCE_H

#include <map>
#include "irrlicht.h"

using namespace irr;
using namespace irr::core;

class Science
{
    public:

        /**
         * Smooths the values using a n-points moving average
         * @param values values to smooth
         * @param nbPoints number of points to use for average
         */
        static std::map < int, vector3df > smooth(const std::map < int, vector3df > & values, int nbPoints);

        /**
         * Computes speed by derivating positions of given trajectory. Uses frame rate of the current sequence,
         * retrieved from SequenceSettings object of Engine.
         * @see SequenceSettings
         * @see Engine
         *
         * @param realPositions real trajectory of which to compute speed
         * @param interval interval (in number of frames) for derivative computation
         * @param framerate frame rate to use to compute speed
         * @return speed data
         */
        static std::map < int, vector3df > computeSpeed(const std::map < int, vector3df >& realPositions,
                                                        int interval,
                                                        int framerate = 1);

        static std::map< int, vector3df > computeRotationWithSpeed(
                                                       const std::map < int, vector3df > & virtualPositions,
                                                       int interval,
                                                       int averagerNbPoints);
};

#endif // SCIENCE_H
