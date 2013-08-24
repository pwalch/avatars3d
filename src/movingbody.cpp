/**
  * 3D Avatars
  * Pierre Walch
  */

#include "camerawindow.h"
#include "movingbody.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

MovingBody::MovingBody()
{

}

MovingBody::~MovingBody()
{

}


ITexture* MovingBody::getTexture()
{
    return texture;
}

void MovingBody::init(const stringw& nameInit, const io::path &modelPath, const io::path &texturePath, float scale, const SColor& trajColor, const int frameNumber)
{
    CameraWindow& cam = CameraWindow::getInstance();
    ISceneManager* sceneManager = cam.getSceneManager();
    IVideoDriver* driver = cam.getDriver();

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

    // Create virtualTrajectory color curve
    trajectoryNode = new ColorCurveNode(trajColor, sceneManager->getRootSceneNode(), sceneManager);

    // Initialize position
    setTime(0);
}

std::vector< vector2d < vector3df > > MovingBody::lastMoves(int from , int samples)
{
    std::vector< vector2d<vector3df > > lines;
    for(int i = 0; i < samples; ++i) {
        int index = from - i;
        if(index - 1 >= 0) {
            vector3df start, end;
            start = virtualTrajectory[index];
            end = virtualTrajectory[index - 1];
            // Add each position pair to the list
            vector2d<vector3df> singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

void MovingBody::mapTime(int time, vector3df position)
{
    virtualTrajectory[time] = position;
}

void MovingBody::setTime(int time)
{
    // If the index is found we display it, else we hide it
    if(virtualTrajectory.find(time) != virtualTrajectory.end())
    {
        node->setVisible(true);
        trajectoryNode->setVisible(true);
        node->setPosition(virtualTrajectory[time]);
        vector3df rotation = node->getRotation();
        node->setRotation(vector3df(rotation.X, rotationAngle[time] + 180, rotation.Z));
        trajectoryNode->setLines(lastMoves(time, 200));
    }
    else {
        node->setVisible(false);
        trajectoryNode->setVisible(false);
    }
}

void MovingBody::computeSpeed(int frameNumber, int framerate)
{
    // Choose an interval for virtualTrajectory derivative
    const int speedInterval = 20;

    CameraWindow& cam = CameraWindow::getInstance();

    // Compute all the speeds (virtual and real) and take account of framerate
    for(int f = speedInterval; f <= frameNumber; ++f) {
        virtualSpeed[f] = framerate * (virtualTrajectory[f] - virtualTrajectory[f - speedInterval]) / speedInterval;
        realSpeed[f] = framerate * (cam.convertToReal(virtualTrajectory[f]) - cam.convertToReal(virtualTrajectory[f - speedInterval])) / speedInterval;
    }

    // Compute first speeds that were not computable before
    for(int f = 0; f < speedInterval; ++f) {
        virtualSpeed[f] = virtualSpeed[speedInterval];
        realSpeed[f] = realSpeed[speedInterval];
    }

    smoothSpeed(frameNumber);
}


void MovingBody::smoothSpeed(int frameNumber)
{
    // Computing n-points average
    const int nbPoints = 15;
    std::map<int, vector3df> virtualSmoothed;
    std::map<int, vector3df> realSmoothed;
    for(int f = nbPoints; f <= frameNumber; ++f) {
        vector3df virtualSum(0, 0, 0);
        vector3df realSum(0, 0, 0);
        for(int n = 1; n <= nbPoints; ++n) {
            virtualSum += virtualSpeed[f - n];
            realSum += realSpeed[f-n];
        }

        virtualSmoothed[f] = virtualSum / ((float)nbPoints);
        realSmoothed[f] = realSum / ((float) nbPoints);
    }

    // Applying the averager on the positions
    for(std::map<int, vector3df>::iterator t = virtualSmoothed.begin(); t != virtualSmoothed.end(); ++t) {
        virtualSpeed[t->first] = t->second;
    }

    for(std::map<int, vector3df>::iterator t = realSmoothed.begin(); t != realSmoothed.end(); ++t) {
        realSpeed[t->first] = t->second;
    }
}
