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

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

Court::Court(const io::path& scenePath, float scale,
             const std::map<int, Player*>& playerMap, Ball* ballInit)
{
    players = playerMap;
    ball = ballInit;

    CameraWindow& cam = CameraWindow::getInstance();
    ISceneManager* sceneManager = cam.getSceneManager();

    // Load Irrlicht scene and apply scaling on the actual court node
    sceneManager->loadScene(scenePath);
    node = sceneManager->getSceneNodeFromName("court");
    node->setScale(vector3df(scale, scale, scale));
    // Activate smooth functions
    node->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    node->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    node->setMaterialFlag(EMF_ANTI_ALIASING, true);
}

Court::~Court()
{
    for(std::map<int, Player*>::iterator i = players.begin(); i != players.end(); ++i) {
        Player* p = i->second;
        delete p;
    }
    delete ball;
}

void Court::setTime(int time)
{
    for(std::map<int, Player*>::iterator i = players.begin(); i != players.end(); ++i) {
        Player* p = i->second;
        p->setTime(time);
    }

    ball->setTime(time);
}

const std::map<int, Player *>& Court::getPlayers() const
{
    return players;
}

