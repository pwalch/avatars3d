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

