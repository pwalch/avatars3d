#include <map>
#include <irrlicht.h>

#include "trajectorychunk.h"
#include "trajectorydata.h"
#include "affinetransformation.h"
#include "science.h"

using namespace irr;
using namespace irr::core;

std::map < int, vector3df > Science::computeEverySmooth(const std::map < int, vector3df > & values, int nbPoints)
{
    // Computing n-points moving average: smoothed[f] = 1/nbPoints sum from i=0 to (nbPoints-1) of original[f-i]
    std::map<int, vector3df> smoothed;
    int firstFrame = values.begin()->first;
    int lastFrame = values.end()->first - 1;
    for(int f = firstFrame + nbPoints; f <= lastFrame; ++f) {
        vector3df sum(0, 0, 0);
        for(int i = 0; i <= (nbPoints - 1); ++i) {
            sum += values.at(f - i);
        }

        smoothed[f] = sum / ((float)nbPoints);
    }

    // We take the first computable mean for the very first values
    for(int f = firstFrame; f < firstFrame + nbPoints; ++f) {
        smoothed[f] = smoothed[nbPoints];
    }

    return smoothed;
}

std::map < int, vector3df > Science::computeEverySpeed(const std::map < int, vector3df >& values,
                                                   int interval,
                                                   int framerate)
{
    // Compute speed and take account of framerate
    std::map < int, vector3df > speed;
    int firstFrame = values.begin()->first;
    int lastFrame = values.end()->first - 1;
    for(int f = firstFrame + interval; f <= lastFrame; ++f) {
        // Speed = (pos[f] - pos[f-n]) / n
        speed[f] = ((float)framerate) * (values.at(f) - values.at(f - interval)) / ((float)interval);
    }

    // We take the first computable speed for the very first speeds
    for(int f = firstFrame; f < firstFrame + interval; ++f) {
        speed[f] = speed[interval];
    }

    return speed;
}

std::map<int, vector3df> Science::computeEveryVirtualSpeed(const std::map<int, vector3df> &position, int interval, int nbPointsAverager, int framerate)
{
    std::map<int, vector3df > smoothed = computeEverySmooth(position, nbPointsAverager);
    std::map<int, vector3df > speed = computeEverySpeed(smoothed, interval, framerate);

    return speed;
}

std::map<int, vector3df> Science::computeEveryRealSpeed(const std::map<int, vector3df> &virtualPosition, int interval, int nbPointsAverager, AffineTransformation* tfm, int framerate)
{
    std::map<int, vector3df > realPosition;
    for(std::map<int, vector3df >::const_iterator i = virtualPosition.begin(); i != virtualPosition.end(); ++i) {
        realPosition[i->first] = tfm->convertToReal(i->second);
    }

    std::map<int, vector3df > smoothed = computeEverySmooth(realPosition, nbPointsAverager);
    std::map<int, vector3df > speed = computeEverySpeed(smoothed, interval, framerate);

    return speed;
}

std::map<int, vector3df> Science::computeEveryVirtualRotation(const std::map<int, vector3df> &virtualPosition, int interval, int nbPointsAverager, int framerate)
{
    std::map<int, vector3df> smoothed = computeEverySmooth(virtualPosition, nbPointsAverager);
    std::map<int, vector3df> virtualSpeed = computeEverySpeed(smoothed, interval, framerate);

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
