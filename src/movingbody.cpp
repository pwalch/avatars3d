#include "movingbody.h"
#include "camerawindow.h"
#include "engine.h"

void MovingBody::init(const MoveableSettings& moveableSettings, const MovingBodySettings& movingBodySettings)
{
    Moveable::prepareMove(moveableSettings);

    this->movingBodySettings = movingBodySettings;

    CameraWindow& cam = CameraWindow::getInstance();
    IVideoDriver* driver = cam.getDriver();
    ISceneManager* sceneManager = cam.getSceneManager();

    Engine& engine = Engine::getInstance();

    // Load player model and apply texture if necessary
    IAnimatedMesh* mesh = sceneManager->getMesh(movingBodySettings.modelPath);
    std::string modelPathCpp = movingBodySettings.modelPath.c_str();
    std::string modelErrorMsg = "Mesh could not be loaded: " + modelPathCpp;
    if(mesh == NULL)
        engine.throwError(modelErrorMsg);
    node = sceneManager->addAnimatedMeshSceneNode(mesh);
    node->setScale(vector3df(movingBodySettings.scale, movingBodySettings.scale, movingBodySettings.scale));
    node->setMaterialFlag(EMF_LIGHTING, false);

    // If texture name is "none" we don't apply a texture
    if(strcmp(movingBodySettings.texturePath.c_str(), "none") != 0) {
        texture = driver->getTexture(movingBodySettings.texturePath);
        std::string texturePathCpp = movingBodySettings.texturePath.c_str();
        std::string textureErrorMsg = "Texture could not be loaded: " + texturePathCpp;
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

//    // Color the vertices
//    sceneManager->getMeshManipulator()->setVertexColors(node->getMesh(), SColor(255, 0, 0, 255));

    // Add Irrlicht GUI text scene node containing the name of the body
    name = movingBodySettings.name;
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
