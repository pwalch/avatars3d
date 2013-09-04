/**
  * 3D Avatars
  * Pierre Walch
  */

#include "camerawindow.h"
#include "moveable.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;


void Moveable::prepareMove(bool trajVisible, const SColor& trajColor, int frameNumber, int framerate)
{
    CameraWindow& cam = CameraWindow::getInstance();
    ISceneManager* sceneManager = cam.getSceneManager();

    isTrajectoryVisible = trajVisible;

    // Create virtualTrajectory color curve
    trajectoryNode = new ColorCurveNode(trajColor, sceneManager->getRootSceneNode(), sceneManager);

    process(frameNumber, framerate);
}

std::vector< vector2d < vector3df > > Moveable::lastMoves(int from , int samples)
{
    std::vector< vector2d<vector3df > > lines;
    for(int i = 0; i < samples; ++i) {
        int index = from - i;
        if(index - 1 >= 0) {
            vector3df start, end;
            start = virtualTrajectory[index];
            end = virtualTrajectory[index - 1];
            // Add each position pair to the list
            vector2d<vector3df> singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

void Moveable::mapTime(int time, vector3df position, vector3df rotation)
{
    virtualTrajectory[time] = position;
    rotationAngle[time] = rotation;
}

void Moveable::setTime(int time)
{
    if(isTrajectoryVisible && virtualTrajectory.find(time) != virtualTrajectory.end())
    {
        const int nbPoints = 100;
        trajectoryNode->setLines(lastMoves(time, nbPoints));
        trajectoryNode->setVisible(true);
    }
    else {
        trajectoryNode->setVisible(false);
    }
}

void Moveable::process(int frameNumber, int framerate)
{
    // Nothing here for now, but can be used to process input trajectories
}

std::map< int, vector3df> Moveable::computeSpeed(std::map< int, vector3df> & trajectory, int frameNumber, int framerate)
{
    // Choose an interval for derivative
    const int speedInterval = 20;

    std::map < int, vector3df > speed;
    // Compute speed and take account of framerate
    for(int f = speedInterval; f <= frameNumber; ++f) {
        speed[f] = framerate * (trajectory[f] - trajectory[f - speedInterval]) / speedInterval;
    }

    // Compute first speeds that were not computable before
    for(int f = 0; f < speedInterval; ++f) {
        speed[f] = speed[speedInterval];
    }

    return speed;
}


void Moveable::smooth(std::map < int, vector3df > & values, int frameNumber)
{
    // Computing n-points average
    const int nbPoints = 15;
    std::map<int, vector3df> smoothed;
    for(int f = nbPoints; f <= frameNumber; ++f) {
        vector3df sum(0, 0, 0);
        for(int n = 1; n <= nbPoints; ++n) {
            sum += values[f - n];
        }

        smoothed[f] = sum / ((float)nbPoints);
    }

    // Applying the averager on the positions
    for(std::map<int, vector3df>::iterator t = smoothed.begin(); t != smoothed.end(); ++t) {
        values[t->first] = t->second;
    }
}
