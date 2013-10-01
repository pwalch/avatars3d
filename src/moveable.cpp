/**
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

}

void Moveable::prepareMove(const MoveableSettings& moveableSettings)
{
    this->moveableSettings = moveableSettings;

    CameraWindow& cam = CameraWindow::getInstance();
    ISceneManager* sceneManager = cam.getSceneManager();

    // Create virtualTrajectory color curve
    trajectoryNode
        = new ColorCurveNode(moveableSettings.trajColor,
                             sceneManager->getRootSceneNode(),
                             sceneManager);
}

std::vector< vector2d < vector3df > > Moveable::lastMoves(int from ,
                                                          int samples)
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
    if(moveableSettings.trajVisible
        && virtualTrajectory.find(time) != virtualTrajectory.end())
    {
        trajectoryNode->setLines(lastMoves(time,
                                           moveableSettings.trajNbPoints));
        trajectoryNode->setVisible(true);
    }
    else {
        trajectoryNode->setVisible(false);
    }
}

std::map< int, vector3df> Moveable::computeSpeed(
                std::map< int, vector3df> & trajectory, int interval)
{
    std::map < int, vector3df > speed;
    // Compute speed and take account of framerate
    for(int f = interval;
        f <= Engine::getInstance().getSequenceSettings().frameNumber; ++f) {
        speed[f] = Engine::getInstance().getSequenceSettings().framerate
            * (trajectory[f] - trajectory[f - interval]) / interval;
    }

    // Compute first speeds that were not computable before
    for(int f = 0; f < interval; ++f) {
        speed[f] = speed[interval];
    }

    return speed;
}


void Moveable::smooth(std::map < int, vector3df > & values, int nbPoints)
{
    // Computing n-points average
    std::map<int, vector3df> smoothed;
    for(int f = nbPoints;
            f <= Engine::getInstance().getSequenceSettings().frameNumber;
            ++f) {
        vector3df sum(0, 0, 0);
        for(int n = 1; n <= nbPoints; ++n) {
            sum += values[f - n];
        }

        smoothed[f] = sum / ((float)nbPoints);
    }

    // Applying the averager on the positions
    for(std::map<int, vector3df>::iterator t = smoothed.begin();
            t != smoothed.end(); ++t) {
        values[t->first] = t->second;
    }
}
