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

void MovingBody::smoothSpeed(int frameNumber)
{
    // Computing n-points average
    const int nbPoints = 15;
    std::map<int, vector3df> smoothed;
    for(int f = nbPoints; f <= frameNumber; ++f) {
        vector3df sum(0, 0, 0);
        for(int n = 1; n <= nbPoints; ++n) {
            sum += speed[f - n];
        }

        sum /= (float)nbPoints;
        smoothed[f] = sum;
    }

    // Applying the averager on the positions
    for(std::map<int, vector3df>::iterator t = smoothed.begin(); t != smoothed.end(); ++t) {
        speed[t->first] = t->second;
    }
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

    // Create trajectory color curve
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
            start = trajectory[index];
            end = trajectory[index - 1];
            // Add each position pair to the list
            vector2d<vector3df> singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

void MovingBody::mapTime(int time, vector3df position)
{
    trajectory[time] = position;
}

void MovingBody::setTime(int time)
{
    // If the index is found we display it, else we hide it
    if(trajectory.find(time) != trajectory.end())
    {
        node->setVisible(true);
        trajectoryNode->setVisible(true);
        node->setPosition(trajectory[time]);
        vector3df rotation = node->getRotation();
        node->setRotation(vector3df(rotation.X, rotationAngle[time] + 180, rotation.Z));
        trajectoryNode->setLines(lastMoves(time, 200));
    }
    else {
        node->setVisible(false);
        trajectoryNode->setVisible(false);
    }

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

    smoothSpeed(frameNumber);
}
