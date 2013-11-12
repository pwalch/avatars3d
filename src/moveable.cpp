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
    delete mTrajectoryData;
}


Moveable::Moveable(TrajectoryData *trajectoryData, const MoveableSettings &moveableSettings, bool hasColorCurve)
{
    this->mTrajectoryData = trajectoryData;
    this->mMoveableSettings = moveableSettings;

    if(hasColorCurve) {
        CameraWindow* cam = CameraWindow::getInstance();
        ISceneManager* sceneManager = cam->getSceneManager();

        // Create virtualTrajectory color curve
        mTrajectoryNode= new ColorCurveNode(moveableSettings.mTrajColor,
                                            sceneManager->getRootSceneNode(),
                                            sceneManager);
    }
}

std::vector< vector2d < vector3df > > Moveable::lastMoves(int from ,
                                                          int samples)
{
    std::vector< vector2d<vector3df > > lines;
    for(int i = 0; i < samples; ++i) {
        int index = from - i;
        if(index - 1 >= 0) {
            vector3df start, end;
            start = mTrajectoryData->getPositionAt(index);
            end = mTrajectoryData->getPositionAt(index - 1);
            // Add each position pair to the list
            vector2d<vector3df> singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

void Moveable::setTime(int time)
{
    if(mMoveableSettings.mTrajVisible
        && mTrajectoryData->isPositionContained(time))
    {
        mTrajectoryNode->setLines(lastMoves(time,
                                           mMoveableSettings.mTrajNbPoints));
        mTrajectoryNode->setVisible(true);
    }
    else {
        mTrajectoryNode->setVisible(false);
    }
}

std::map < int, vector3df > Moveable::computeSpeed(
        const TrajectoryData& trajectoryData,
        int interval)
{
    std::map < int, vector3df > speed;
    // Compute speed and take account of framerate
    const int framerate = Engine::getInstance().getSequenceSettings().mFramerate;
    for(int f = interval;
        f <= trajectoryData.getEndIndex();
        ++f) {
        speed[f] = ((float)framerate) *
                (trajectoryData.getPositionAt(f)
                    - trajectoryData.getPositionAt(f - interval))
                / ((float)interval);
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
            f <= Engine::getInstance().getSequenceSettings().mFrameNumber;
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
