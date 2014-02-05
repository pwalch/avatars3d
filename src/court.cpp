/*
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <map>
#include <irrlicht.h>
#include "camerawindow.h"
#include "player.h"
#include "court.h"
#include "engine.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

Court::Court(const CourtSettings& courtSettings,
             const std::map<int, Player*>& playerMap,
             MovingBody* ball)
{
    mPlayers = playerMap;
    mBall = ball;

    CameraWindow* cam = Engine::getInstance().getCameraWindow();
    ISceneManager* sceneManager = cam->getSceneManager();

    Engine& engine = Engine::getInstance();

    if(sceneManager->loadScene(courtSettings.mScenePath) == false) {
        engine.throwError(L"Scene file could not be loaded");
    }

    mNode = sceneManager->getSceneNodeFromName("court");
    if(mNode == NULL) {
        engine.throwError(L"Scene file does not contain court node");
    }

    mNode->setVisible(true);
    const float scale = courtSettings.mScale;
    mNode->setScale(vector3df(scale, scale, scale));

    // Activate smoothing functions
    mNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    mNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    mNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
}

Court::~Court()
{
    for(std::map<int, Player*>::iterator i = mPlayers.begin(); i != mPlayers.end(); ++i) {
        Player* p = i->second;
        delete p;
    }
    delete mBall;
}

void Court::updateTrajectories(const std::map<int, VectorSequence>& playerChunk,
                               const VectorSequence& ballChunk)
{
    // Update positions of each player
    for(std::map<int, VectorSequence>::const_iterator i = playerChunk.begin();
        i != playerChunk.end();
        ++i) {
        Player* p = mPlayers[i->first];
        p->updatePositions(i->second);
    }

    mBall->updatePositions(ballChunk);
}

void Court::setTime(int time)
{
    for(std::map<int, Player*>::iterator i = mPlayers.begin(); i != mPlayers.end(); ++i) {
        Player* p = i->second;
        p->setTime(time);
    }

    mBall->setTime(time);
}

const std::map<int, Player *>& Court::getPlayers() const
{
    return mPlayers;
}

