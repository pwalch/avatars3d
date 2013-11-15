/*
  * 3D Avatars
  * Pierre Walch
  */

#include "camerawindow.h"
#include "moveable.h"
#include "engine.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

Moveable::~Moveable()
{
    delete mTrajectoryData;
}


Moveable::Moveable(TrajectoryData *trajectoryData)
{
    this->mTrajectoryData = trajectoryData;
}



std::map < int, vector3df > Moveable::smooth(const std::map < int, vector3df > & values, int nbPoints)
{
    // Computing n-points moving average: smoothed[f] = 1/nbPoints sum from i=0 to (nbPoints-1) of original[f-i]
    std::map<int, vector3df> smoothed;
    int lastFrame = values.end()->first - 1;
    for(int f = nbPoints; f <= lastFrame; ++f) {
        vector3df sum(0, 0, 0);
        for(int i = 0; i <= (nbPoints - 1); ++i) {
            sum += values.at(f - i);
        }

        smoothed[f] = sum / ((float)nbPoints);
    }

    // We take the first computable mean for the very first values
    for(int f = 0; f < nbPoints; ++f) {
        smoothed[f] = smoothed[nbPoints];
    }

    return smoothed;
}

std::map < int, vector3df > Moveable::computeSpeed(const std::map < int, vector3df >& realPositions,
                                                   int interval,
                                                   int framerate)
{
    // Compute speed and take account of framerate
    std::map < int, vector3df > speed;
    int lastFrame = realPositions.end()->first - 1;
    for(int f = interval; f <= lastFrame; ++f) {
        // Speed = (pos[f] - pos[f-n]) / n
        speed[f] = ((float)framerate) * (realPositions.at(f) - realPositions.at(f - interval)) / ((float)interval);
    }

    // We take the first computable speed for the very first speeds
    for(int f = 0; f < interval; ++f) {
        speed[f] = speed[interval];
    }

    return speed;
}

