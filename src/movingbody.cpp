#include "movingbody.h"
#include "camerawindow.h"

void MovingBody::init(bool trajVisible, const SColor& trajColor, int frameNumber, int framerate, const stringw &nameInit, const io::path &modelPath, const io::path &texturePath, float scale)
{
    Moveable::prepareMove(trajVisible, trajColor, frameNumber, framerate);

    CameraWindow& cam = CameraWindow::getInstance();
    IVideoDriver* driver = cam.getDriver();
    ISceneManager* sceneManager = cam.getSceneManager();

    // Load player model and apply texture if necessary
    node = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh(modelPath));
    node->setScale(vector3df(scale, scale, scale));
    node->setMaterialFlag(EMF_LIGHTING, false);

    // If texture name is "none" we don't apply a texture
    if(strcmp(texturePath.c_str(), "none") != 0) {
        texture = driver->getTexture(texturePath);
        node->setMaterialTexture(0, texture);
    }

    // Set material settings
    node->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
    node->setMaterialFlag(EMF_FRONT_FACE_CULLING, false);
    node->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    node->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    node->setMaterialFlag(EMF_ANTI_ALIASING, true);

//    // Color the vertices
//    sceneManager->getMeshManipulator()->setVertexColors(node->getMesh(), SColor(255, 0, 0, 255));

    // Add Irrlicht GUI text scene node containing the name of the body
    name = nameInit;
    textNode = sceneManager->addTextSceneNode(cam.getGuiFont(), name.c_str(), SColor(255, 0, 255, 255), node);
    textNode->setVisible(false);
}

void MovingBody::setTime(int time)
{
    Moveable::setTime(time);

    if(virtualTrajectory.find(time) != virtualTrajectory.end())
    {
        node->setVisible(true);
        node->setPosition(virtualTrajectory[time]);
        node->setRotation(rotationAngle[time]);
    }
    else {
        node->setVisible(false);
    }
}



ITexture* MovingBody::getTexture()
{
    return texture;
}
