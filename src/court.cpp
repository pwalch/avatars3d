/**
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

Court::Court(const io::path& scenePath, float scale,
             const std::map<int, Player*>& playerMap, MovingBody* ballInit)
{
    mPlayers = playerMap;
    mBall = ballInit;

    CameraWindow& cam = CameraWindow::getInstance();
    ISceneManager* sceneManager = cam.getSceneManager();

    Engine& engine = Engine::getInstance();

    // Load Irrlicht scene and apply scaling on the actual court node
    if(sceneManager->loadScene(scenePath) == false)
        engine.throwError("Scene file could not be loaded");

    mNode = sceneManager->getSceneNodeFromName("court");
    if(mNode == NULL)
        engine.throwError("Scene file does not contain court node");

    mNode->setVisible(true);
    mNode->setScale(vector3df(scale, scale, scale));
    // Activate smooth functions
    mNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    mNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    mNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
}

Court::~Court()
{
    for(std::map<int, Player*>::iterator i = mPlayers.begin();
            i != mPlayers.end(); ++i) {
        Player* p = i->second;
        delete p;
    }
    delete mBall;
}

void Court::setTime(int time)
{
    // Update each component of the scene

    for(std::map<int, Player*>::iterator i = mPlayers.begin();
            i != mPlayers.end(); ++i) {
        Player* p = i->second;
        p->setTime(time);
    }

    mBall->setTime(time);
}

const std::map<int, Player *>& Court::getPlayers() const
{
    return mPlayers;
}

