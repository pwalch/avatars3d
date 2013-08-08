/**
  * 3D Avatars
  * Pierre Walch
  */

#include "camerawindow.h"
#include "movingbody.h"

MovingBody::MovingBody()
{

}

void MovingBody::smoothTrajectory(int frameNumber)
{
    // Computing n-points average
    const int nbPoints = 15;
    std::map<int, irr::core::vector3df> smoothed;
    for(int f = nbPoints; f <= frameNumber; ++f) {
        irr::core::vector3df sum(0, 0, 0);
        for(int n = 1; n <= nbPoints; ++n) {
            sum += trajectory[f - n];
        }

        sum /= (float)nbPoints;
        smoothed[f] = sum;
    }

    // Applying the averager on the positions
    for(std::map<int, irr::core::vector3df>::iterator t = smoothed.begin(); t != smoothed.end(); ++t) {
        trajectory[t->first] = t->second;
    }
}


irr::video::ITexture* MovingBody::getTexture()
{
    return texture;
}

void MovingBody::init(const irr::core::stringw& nameInit, const irr::io::path &modelPath, const irr::io::path &texturePath, float scale, const irr::video::SColor& trajColor, const int frameNumber)
{
    CameraWindow& cam = CameraWindow::getInstance();
    irr::scene::ISceneManager* sceneManager = cam.getSceneManager();
    irr::video::IVideoDriver* driver = cam.getDriver();

    // Load player model and apply texture if necessary
    node = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh(modelPath));
    node->setScale(irr::core::vector3df(scale, scale, scale));
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    // If texture name is "none" we don't apply a texture
    if(strcmp(texturePath.c_str(), "none") != 0) {
        texture = driver->getTexture(texturePath);
        node->setMaterialTexture(0, texture);
    }

    // Set material settings
    node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true);
    node->setMaterialFlag(irr::video::EMF_FRONT_FACE_CULLING, false);
    node->setMaterialFlag(irr::video::EMF_TRILINEAR_FILTER, true);
    node->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true);
    node->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);

//    // Color the vertices
//    sceneManager->getMeshManipulator()->setVertexColors(node->getMesh(), irr::video::SColor(255, 0, 0, 255));

    // Add Irrlicht GUI text scene node containing the name of the body
    name = nameInit;
    textNode = sceneManager->addTextSceneNode(cam.getGuiFont(), name.c_str(), irr::video::SColor(255, 0, 255, 255), node);
    textNode->setVisible(false);

    // Create trajectory color curve
    trajectoryNode = new ColorCurveNode(trajColor, sceneManager->getRootSceneNode(), sceneManager);

    smoothTrajectory(frameNumber);

    // Initialize position
    setTime(0);
}

std::vector< irr::core::vector2d < irr::core::vector3df > > MovingBody::lastMoves(int from , int samples)
{
    std::vector< irr::core::vector2d<irr::core::vector3df > > lines;
    for(int i = 0; i < samples; ++i) {
        int index = from - i;
        if(index - 1 >= 0) {
            irr::core::vector3df start, end;
            start = trajectory[index];
            end = trajectory[index - 1];
            // Add each position pair to the list
            irr::core::vector2d<irr::core::vector3df> singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

void MovingBody::mapTime(int time, irr::core::vector3df position)
{
    trajectory[time] = position;
}

void MovingBody::setTime(int time)
{
    // If the index is found we display it, else we hide it
    if(trajectory.find(time) != trajectory.end())
    {
        node->setVisible(true);
        node->setPosition(trajectory[time]);
        irr::core::vector3df rotation = node->getRotation();
        node->setRotation(irr::core::vector3df(rotation.X, rotationAngle[time] + 180, rotation.Z));
        trajectoryNode->setLines(lastMoves(time, 200));
    }
    else
        node->setVisible(false);
}

void MovingBody::computeSpeed(int frameNumber)
{
    // Choose an interval for trajectory derivative
    const int speedInterval = 20;
    // Compute all the speeds
    for(int f = speedInterval; f <= frameNumber; ++f) {
        speed[f] = (trajectory[f] - trajectory[f - speedInterval]) / speedInterval;
    }
    // Compute first speeds that were not computable before
    for(int f = 0; f < speedInterval; ++f)
        speed[f] = speed[speedInterval];
}
