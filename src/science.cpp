#include <map>
#include <irrlicht.h>
#include "science.h"

using namespace irr;
using namespace irr::core;

std::map < int, vector3df > Science::smooth(const std::map < int, vector3df > & values, int nbPoints)
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

std::map < int, vector3df > Science::computeSpeed(const std::map < int, vector3df >& realPositions,
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

std::map<int, vector3df> Science::computeRotationWithSpeed(
                                               const std::map<int, vector3df>& virtualPositions,
                                               int interval,
                                               int averagerNbPoints)
{
    // Compute virtual speed to compute angle
    std::map < int, vector3df > virtualSpeed = Science::computeSpeed(virtualPositions,
                                                                    interval);
    virtualSpeed = Science::smooth(virtualSpeed, averagerNbPoints);

    std::map<int, vector3df> rotations;
    // Deduce angle from virtual speed
    for(std::map<int, vector3df>::iterator t = virtualSpeed.begin(); t != virtualSpeed.end(); ++t) {
        int index = t->first;
        vector3df avSpeed = t->second;
        float angle = avSpeed.getHorizontalAngle().Y;
        rotations[index] = vector3df(0, angle + 180, 0);
    }

    return rotations;
}
