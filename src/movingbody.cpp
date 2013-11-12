/**
  * 3D Avatars
  * Pierre Walch
  */


#include "movingbody.h"
#include "camerawindow.h"
#include "engine.h"

MovingBody::MovingBody(TrajectoryData* trajectoryData,
                const BodySettings& movingBodySettings)
    : Moveable(trajectoryData)
{
    this->mMovingBodySettings = movingBodySettings;

    CameraWindow* cam = CameraWindow::getInstance();
    IVideoDriver* driver = cam->getDriver();
    ISceneManager* sceneManager = cam->getSceneManager();

    // Create virtualTrajectory color curve
    mTrajectoryNode= new ColorCurveNode(movingBodySettings.mTrajColor,
                                        sceneManager->getRootSceneNode(),
                                        sceneManager);

    Engine& engine = Engine::getInstance();

    // Load player model and apply texture if necessary
    IAnimatedMesh* mesh = sceneManager->getMesh(movingBodySettings.mModelPath);
    std::string modelErrorMsg = "Mesh could not be loaded.";
    if(mesh == NULL)
        engine.throwError(modelErrorMsg);

    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    node->setScale(vector3df(movingBodySettings.mScale,
                             movingBodySettings.mScale,
                             movingBodySettings.mScale));
    node->setMaterialFlag(EMF_LIGHTING, false);

    // If texture name is "none" we don't apply a texture
    if(strcmp(movingBodySettings.mTexturePath.c_str(), "none") != 0) {
        texture = driver->getTexture(movingBodySettings.mTexturePath);
        std::string textureErrorMsg = "A texture could not be loaded";
        if(texture == NULL)
            engine.throwError(textureErrorMsg);
        node->setMaterialTexture(0, texture);
    }

    // Set material settings
    node->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
    node->setMaterialFlag(EMF_FRONT_FACE_CULLING, false);

    node->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    node->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    node->setMaterialFlag(EMF_ANTI_ALIASING, true);
    node->setLoopMode(false);
    node->setAnimationSpeed(0);

    node->setVisible(movingBodySettings.mVisible);

//    // Color the vertices
//    sceneManager->getMeshManipulator()->setVertexColors(node->getMesh(),
//            SColor(255, 0, 0, 255));

    // Add Irrlicht GUI text scene node containing the name of the body
//    textNode = sceneManager->addTextSceneNode(cam->getGuiFont(),
//                               name.c_str(),
//                               SColor(255, 0, 255, 255),
//                               node);
//    textNode->setVisible(false);
}

void MovingBody::setTime(int time)
{
    if(mMovingBodySettings.mVisible && mTrajectoryData->isPositionContained(time)) {
        node->setVisible(true);
        node->setPosition(mTrajectoryData->getPositionAt(time));
        node->setRotation(mTrajectoryData->getRotationAt(time));
    } else {
        node->setVisible(false);
    }

    if(mMovingBodySettings.mTrajVisible && mTrajectoryData->isPositionContained(time)) {
        mTrajectoryNode->setLines(lastMoves(time, mMovingBodySettings.mTrajNbPoints));
        mTrajectoryNode->setVisible(true);
    } else {
        mTrajectoryNode->setVisible(false);
    }
}



ITexture* MovingBody::getTexture()
{
    return texture;
}

std::vector< vector2d < vector3df > > MovingBody::lastMoves(int from, int samples)
{
    std::vector< vector2d<vector3df > > lines;
    for(int i = 0; i < samples; ++i) {
        int index = from - i;
        if(index - 1 >= 0) {
            vector3df start, end;
            start = mTrajectoryData->getPositionAt(index);
            end = mTrajectoryData->getPositionAt(index - 1);
            // Add each position pair to the list
            vector2d<vector3df> singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

