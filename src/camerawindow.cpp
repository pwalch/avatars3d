#include <iostream>
#include "camerawindow.h"

CameraWindow::~CameraWindow()
{
    delete eventManager;
    device->drop();
}

CameraWindow& CameraWindow::getInstance()
{
    static CameraWindow instance;
    return instance;
}

void CameraWindow::init(const irr::core::dimension2d<irr::u32>& initialWindowSize, const irr::core::vector3df& initialPosition, const irr::core::vector3df& initialRotation, const char* fontPath, int initialSpeed)
{
    irr::SIrrlichtCreationParameters params = irr::SIrrlichtCreationParameters();
    params.AntiAlias = 64;
    params.Bits = 32;
    params.DriverType = irr::video::EDT_OPENGL;
    params.Doublebuffer = true;
    params.Fullscreen = false;
    params.HighPrecisionFPU = false;
    params.IgnoreInput = false;
    params.LoggingLevel = irr::ELL_ERROR;
    params.Stencilbuffer = false;
    params.Stereobuffer = false;
    params.Vsync = false;
    params.WindowId = 0;
    params.WindowSize = initialWindowSize;
    params.WithAlphaChannel = false;
    params.ZBufferBits = 16;
    device = irr::createDeviceEx(params);

    // Create Irrlicht window without vertical synchronization to avoid performance limitation
//    device = irr::createDevice(
//                irr::video::EDT_OPENGL,
//                initialWindowSize,
//                32, false, false, false, 0);

    device->setWindowCaption(L"3D View");
    windowSize = initialWindowSize;
    // Stop device timer because we handle it manually during execution
    device->getTimer()->stop();
    driver = device->getVideoDriver();
    sceneManager = device->getSceneManager();

    // Add camera and link rotation with target (rotation affects target)
    staticCamera = sceneManager->addCameraSceneNode();
    staticCamera->bindTargetAndRotation(true);
    staticCamera->setFarValue(3000);

    speed = initialSpeed;

    // Initialize camera position and rotation
    setPosition(initialPosition);
    setRotation(initialRotation);

    // Create event manager to handle keyboard and mouse
    eventManager = new EventManager();
    device->setEventReceiver(eventManager);

    gui = device->getGUIEnvironment();
    font = gui->getFont(fontPath);
    frameText = "000000000000";
    irr::core::dimension2d<irr::u32> dimension = font->getDimension(frameText.c_str());
    frameCount = gui->addStaticText(frameText.c_str(), irr::core::recti(0, 0, dimension.Width, dimension.Height));
}

const irr::core::vector3df& CameraWindow::getCameraPosition() const
{
    return staticCamera->getPosition();
}

const irr::core::vector3df& CameraWindow::getCameraRotation() const
{
    return staticCamera->getRotation();
}

void CameraWindow::setPosition(const irr::core::vector3df& position)
{
    staticCamera->setPosition(position);
}

void CameraWindow::setRotation(const irr::core::vector3df& rotation)
{
    staticCamera->updateAbsolutePosition();
    staticCamera->setRotation(rotation);
}

void CameraWindow::move(const irr::core::vector3df& moveVector)
{
    irr::core::vector3df pos = staticCamera->getPosition();
    irr::core::vector3df target = (staticCamera->getTarget() - staticCamera->getAbsolutePosition());

    // Forward direction is the target direction
    irr::core::vector3df forwardDirection = target;
    forwardDirection.normalize();
    pos += forwardDirection * moveVector.X;

    // Up vector is given in camera definition
    irr::core::vector3df upDirection = staticCamera->getUpVector();
    upDirection.normalize();
    pos += upDirection * moveVector.Z;

    // Left direction is cross product of forward and up vectors
    irr::core::vector3df leftDirection = target.crossProduct(upDirection);
    leftDirection.normalize();
    pos += leftDirection * moveVector.Y;

    // Apply rotation to keep camera in the same relative direction
    irr::core::vector3df relativeRotation = target.getHorizontalAngle();
    target.set(0,0, irr::core::max_(1.f, pos.getLength()));
    irr::core::matrix4 mat;
    mat.setRotationDegrees(irr::core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
    mat.transformVect(target);

    // Moving camera to its new position and adapt target
    staticCamera->setPosition(pos);
    target += pos;

    staticCamera->updateAbsolutePosition();
    staticCamera->setTarget(target);
}

void CameraWindow::rotate(const irr::core::vector3df& rotationVector)
{
    irr::core::vector3df target = (staticCamera->getTarget() - staticCamera->getAbsolutePosition());
    irr::core::vector3df relativeRotation = target.getHorizontalAngle();
    relativeRotation.Y -= rotationVector.Y;
    relativeRotation.X -= rotationVector.X;

    const irr::f32 MaxVerticalAngle = 88.0f;
    // X < MaxVerticalAngle or X > 360-MaxVerticalAngle
    if (relativeRotation.X > MaxVerticalAngle*2 && relativeRotation.X < 360.0f-MaxVerticalAngle) {
        relativeRotation.X = 360.0f-MaxVerticalAngle;
    } else if (relativeRotation.X > MaxVerticalAngle && relativeRotation.X < 360.0f-MaxVerticalAngle) {
        relativeRotation.X = MaxVerticalAngle;
    }
    target.set(0,0, irr::core::max_(1.f, staticCamera->getPosition().getLength()));
    irr::core::matrix4 mat;
    mat.setRotationDegrees(irr::core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
    mat.transformVect(target);

    target += staticCamera->getPosition();

    // setTarget uses absolute position member so we need to update it (see Irrlicht doc)
    staticCamera->updateAbsolutePosition();
    staticCamera->setTarget(target);
}

irr::gui::IGUIEnvironment *CameraWindow::getGUI()
{
    return gui;
}

irr::gui::IGUIFont* CameraWindow::getFont()
{
    return font;
}

void CameraWindow::updateScene()
{
    if(device->run()) {
        driver->beginScene(
                    true, // clear back-buffer
                    true, // clear z-buffer
                    irr::video::SColor(255, 196, 194, 199));
        sceneManager->drawAll();
        gui->drawAll();
        driver->endScene();
    }
}

irr::IrrlichtDevice* CameraWindow::getDevice() const
{
    return device;
}

const irr::core::dimension2di& CameraWindow::getWindowSize() const
{
    return windowSize;
}

irr::scene::ISceneManager* CameraWindow::getSceneManager() const
{
    return sceneManager;
}

irr::video::IVideoDriver* CameraWindow::getDriver() const
{
    return driver;
}

irr::video::IImage* CameraWindow::getScreenshot()
{
    irr::video::IImage* screenshot = driver->createScreenShot();
    return screenshot;
}

int CameraWindow::getSpeed() const
{
    return speed;
}

void CameraWindow::setFrameCount(int frameCountNew)
{
    irr::core::stringw txt;
    txt += frameCountNew;
    frameCount->setText(txt.c_str());
}

void CameraWindow::takeScreenshot(int time)
{
    irr::core::stringw str = "screenshots/scr_";
    str += time;
    str += ".png";
    irr::video::IImage* scr = getScreenshot();
    driver->writeImageToFile(scr, str);
}
