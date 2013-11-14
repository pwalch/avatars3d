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
    // Computing n-points moving average
    std::map<int, vector3df> smoothed;
    int frameNumber = Engine::getInstance().getSequenceSettings().mFrameNumber;
    for(int f = nbPoints; f <= frameNumber; ++f) {
        vector3df sum(0, 0, 0);
        for(int n = 1; n <= nbPoints; ++n) {
            sum += values.at(f - n);
        }

        smoothed[f] = sum / ((float)nbPoints);
    }

    return smoothed;
}

std::map < int, vector3df > Moveable::computeSpeed(const TrajectoryData& trajectoryData, int interval)
{
    // Compute speed and take account of framerate
    const int framerate = Engine::getInstance().getSequenceSettings().mFramerate;
    std::map < int, vector3df > speed;
    for(int f = interval; f <= trajectoryData.getEndIndex(); ++f) {
        // Speed = (pos[f] - pos[f-n]) / n
        speed[f] = ((float)framerate) *
                    (trajectoryData.getPositionAt(f) - trajectoryData.getPositionAt(f - interval))
                    / ((float)interval);
    }

    // Compute first speeds that were not computable before
    for(int f = 0; f < interval; ++f) {
        speed[f] = speed[interval];
    }

    return speed;
}

