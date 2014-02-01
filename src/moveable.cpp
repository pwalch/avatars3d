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

Moveable::Moveable()
{
    mTrajectoryData = new TrajectoryData();
}

void Moveable::updateWith(TrajectoryChunk *chunk)
{
    mTrajectoryData->updateWith(chunk);

    delete chunk;
}


