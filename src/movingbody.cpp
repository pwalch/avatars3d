/*
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
    mColorCurveNode= new ColorCurveNode(movingBodySettings.mTrajColor,
                                        sceneManager->getRootSceneNode(),
                                        sceneManager);

    Engine& engine = Engine::getInstance();

    // Load player model and apply texture if necessary
    IAnimatedMesh* mesh = sceneManager->getMesh(movingBodySettings.mModelPath);
    stringw modelErrorMsg = "Mesh could not be loaded: ";
    modelErrorMsg += movingBodySettings.mModelPath;
    if(mesh == NULL)
        engine.throwError(modelErrorMsg);

    mNode = sceneManager->addAnimatedMeshSceneNode(mesh);
    mNode->setScale(vector3df(movingBodySettings.mScale,
                             movingBodySettings.mScale,
                             movingBodySettings.mScale));
    mNode->setMaterialFlag(EMF_LIGHTING, false);

    // If texture name is "none" we don't apply a texture
    if(strcmp(movingBodySettings.mTexturePath.c_str(), "none") != 0) {
        mTexture = driver->getTexture(movingBodySettings.mTexturePath);
        stringw textureErrorMsg = "Texture could not be loaded: ";
        textureErrorMsg += movingBodySettings.mTexturePath;
        if(mTexture == NULL)
            engine.throwError(textureErrorMsg);
        mNode->setMaterialTexture(0, mTexture);
    }

    // Set material settings
    mNode->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
    mNode->setMaterialFlag(EMF_FRONT_FACE_CULLING, false);

    mNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    mNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    mNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
    mNode->setLoopMode(false);
    mNode->setAnimationSpeed(0);

    mNode->setVisible(movingBodySettings.mVisible);

    // Irrlicht 3D text node snippet
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
    // Displaying or hiding 3D model
    if(mMovingBodySettings.mVisible && mTrajectoryData->containsPositionFrame(time)) {
        mNode->setVisible(true);
        mNode->setPosition(mTrajectoryData->getPositionAt(time));
        mNode->setRotation(mTrajectoryData->getRotationAt(time));
    } else {
        mNode->setVisible(false);
    }

    // Displaying or hiding trajectory color curve
    if(mMovingBodySettings.mTrajVisible && mTrajectoryData->containsPositionFrame(time)) {
        mColorCurveNode->setLines(lastMoves(time, mMovingBodySettings.mTrajNbPoints));
        mColorCurveNode->setVisible(true);
    } else {
        mColorCurveNode->setVisible(false);
    }
}

std::vector< std::pair<vector3df, vector3df > > MovingBody::lastMoves(int from, int samples)
{
    std::vector< std::pair<vector3df, vector3df > > lines;
    for(int i = 0; i < samples; ++i) {
        int index = from - i;
        if(index - 1 >= 0) {
            // Find both ends of the line
            vector3df start, end;
            start = mTrajectoryData->getPositionAt(index);
            end = mTrajectoryData->getPositionAt(index - 1);
            // Add each position pair to the list
            std::pair<vector3df, vector3df > singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

