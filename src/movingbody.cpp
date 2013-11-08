#include "movingbody.h"
#include "camerawindow.h"
#include "engine.h"

void MovingBody::init(const MoveableSettings& moveableSettings,
                      const MovingBodySettings& movingBodySettings)
{
    Moveable::prepareMove(moveableSettings);

    this->mMovingBodySettings = movingBodySettings;

    CameraWindow& cam = CameraWindow::getInstance();
    IVideoDriver* driver = cam.getDriver();
    ISceneManager* sceneManager = cam.getSceneManager();

    Engine& engine = Engine::getInstance();

    // Load player model and apply texture if necessary
    IAnimatedMesh* mesh = sceneManager->getMesh(mMovingBodySettings.mModelPath);
    std::string modelPathCpp = mMovingBodySettings.mModelPath.c_str();
    std::string modelErrorMsg = "Mesh could not be loaded: " + modelPathCpp;
    if(mesh == NULL)
        engine.throwError(modelErrorMsg);
    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    node->setScale(vector3df(mMovingBodySettings.mScale,
                             mMovingBodySettings.mScale,
                             mMovingBodySettings.mScale));
    node->setMaterialFlag(EMF_LIGHTING, false);

    // If texture name is "none" we don't apply a texture
    if(strcmp(mMovingBodySettings.mTexturePath.c_str(), "none") != 0) {
        texture = driver->getTexture(mMovingBodySettings.mTexturePath);
        std::string texturePathCpp = mMovingBodySettings.mTexturePath.c_str();
        std::string textureErrorMsg
            = "Texture could not be loaded: " + texturePathCpp;
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

    node->setVisible(mMovingBodySettings.mVisible);

//    // Color the vertices
//    sceneManager->getMeshManipulator()->setVertexColors(node->getMesh(),
//            SColor(255, 0, 0, 255));

    // Add Irrlicht GUI text scene node containing the name of the body
    name = mMovingBodySettings.mName;
    textNode = sceneManager->addTextSceneNode(cam.getGuiFont(),
                               name.c_str(),
                               SColor(255, 0, 255, 255),
                               node);
    textNode->setVisible(false);
}

void MovingBody::setTime(int time)
{
    Moveable::setTime(time);

    if(mMovingBodySettings.mVisible && mVirtualTrajectory.find(time) != mVirtualTrajectory.end())
    {
        node->setVisible(true);
        node->setPosition(mVirtualTrajectory[time]);
        node->setRotation(mRotationAngle[time]);
    }
    else {
        node->setVisible(false);
    }
}



ITexture* MovingBody::getTexture()
{
    return texture;
}
