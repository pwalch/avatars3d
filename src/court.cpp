#include <iostream>
#include <map>
#include <irrlicht.h>
#include "camerawindow.h"
#include "player.h"
#include "court.h"

Court::Court(const irr::io::path& scenePath, float scale,
             const std::map<int, Player*>& playerMap, Ball* ballInit,
             int frameNumber, int framerate, int animFramerate,
             std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold)
{
    players = playerMap;
    ball = ballInit;

    CameraWindow& cam = CameraWindow::getInstance();
    irr::scene::ISceneManager* sceneManager = cam.getSceneManager();

    sceneManager->loadScene(scenePath);
    node = sceneManager->getSceneNodeFromName("court");
    node->setMaterialFlag(irr::video::EMF_TRILINEAR_FILTER, true);
    node->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true);
    node->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
    node->setScale(irr::core::vector3df(scale, scale, scale));

    processPlayers(frameNumber, framerate, animFramerate, stateDates, stateThreshold);
    processBall(frameNumber);

    //    // Loading model from Blender
//    irr::scene::IMesh* mesh = sceneManager->getMesh(modelPath);
//    node = sceneManager->addMeshSceneNode(mesh);
//    node->setScale(irr::core::vector3df(scale, scale, scale));
//    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
//    node->setMaterialTexture(0, driver->getTexture(texturePath));
//    node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
//    node->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, false);
}

Court::~Court()
{
    for(std::map<int, Player*>::iterator i = players.begin(); i != players.end(); ++i) {
        Player* p = i->second;
        delete p;
    }
    delete ball;
}

void Court::setTime(const int time)
{
    for(std::map<int, Player*>::iterator i = players.begin(); i != players.end(); ++i) {
        Player* p = i->second;
        p->setTime(time);
    }

    ball->setTime(time);
}

void Court::processPlayers(int frameNumber, int framerate, int animFramerate,
        std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold)
{
    for(std::map<int, Player*>::iterator i = players.begin(); i != players.end(); ++i) {
        Player* p = i->second;

        p->smoothTrajectory(frameNumber);
        p->computeSpeed(frameNumber, framerate, animFramerate, stateDates, stateThreshold);
    }
}

void Court::processBall(int frameNumber)
{
    ball->smoothTrajectory(frameNumber);
}

std::map<int, Player *> Court::getPlayers() const
{
    return players;
}
