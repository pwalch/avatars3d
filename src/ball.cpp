#include "camerawindow.h"
#include "ball.h"

Ball::Ball()
{

}

void Ball::mapTime(int time, irr::core::vector3df position)
{
    trajectory[time] = position;
}


void Ball::init(const char *modelPath, const char *texturePath)
{
    CameraWindow& cam = CameraWindow::getInstance();
    irr::scene::ISceneManager* sceneManager = cam.getSceneManager();
    irr::video::IVideoDriver* driver = cam.getDriver();

    node = sceneManager->addMeshSceneNode(sceneManager->getMesh(modelPath));
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    if(strcmp(texturePath, "none") != 0) {
        texture = driver->getTexture(texturePath);
        node->setMaterialTexture(0, texture);
    }
    node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
    node->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, false);

    setTime(0);
}

void Ball::setTime(int time)
{
    if(trajectory.find(time) != trajectory.end())
    {
        node->setVisible(true);
        node->setPosition(trajectory[time]);
    }
    else
        node->setVisible(false);
}

void Ball::smoothTrajectory(int frameNumber)
{

}
