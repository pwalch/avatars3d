/**
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include "engine.h"
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

void CameraWindow::init(const irr::core::dimension2d<irr::u32>& initialWindowSize, const irr::video::SColor& bgColor, const irr::video::SColor& jTextColor, const irr::core::vector3df& initialPosition, const irr::core::vector3df& initialRotation, const char* fontGUIPath, const char* fontJerseyPath, int initialSpeed)
{
    irr::SIrrlichtCreationParameters params = irr::SIrrlichtCreationParameters();
    // Multisampling with 64 samples
    params.AntiAlias = 64;
    params.Bits = 32;
    // Using OpenGL for rendering
    params.DriverType = irr::video::EDT_OPENGL;
    params.Doublebuffer = true;
    params.Fullscreen = false;
    params.HighPrecisionFPU = false;
    params.IgnoreInput = false;
    // Display only important log entries
    params.LoggingLevel = irr::ELL_ERROR;
    params.Stencilbuffer = false;
    params.Stereobuffer = false;
    // We disable vertical synchronization to avoid performance clamping
    params.Vsync = false;
    params.WindowId = 0;
    params.WindowSize = initialWindowSize;
    params.WithAlphaChannel = false;
    params.ZBufferBits = 16;
    device = irr::createDeviceEx(params);

    device->setWindowCaption(L"3D View");
    windowSize = initialWindowSize;

    device->setResizable(false);

    // Set background color and jersey text color
    backgroundColor = bgColor;
    jerseyTextColor = jTextColor;
    // Stop device timer because we do not use it
    device->getTimer()->stop();
    driver = device->getVideoDriver();
    sceneManager = device->getSceneManager();

    // Add camera and link rotation with target (rotation affects target)
    staticCamera = sceneManager->addCameraSceneNode();
    staticCamera->bindTargetAndRotation(true);
    staticCamera->setFarValue(3000);
    // Set FPS camera speed (for user interface)
    speed = initialSpeed;

    // Initialize camera position and rotation
    setPosition(initialPosition);
    setRotation(initialRotation);

    // Create event manager to handle keyboard and mouse inputs from Irrlicht
    eventManager = new EventManager();
    device->setEventReceiver(eventManager);

    // Create GUI environment to use fonts and display 2D texts
    gui = device->getGUIEnvironment();
    guiFont = gui->getFont(fontGUIPath);
    jerseyFont = gui->getFont(fontJerseyPath);

    // Set default font
    irr::gui::IGUISkin* skin = gui->getSkin();
    skin->setFont(guiFont);

    // Display frame count on top left corner
    irr::core::dimension2d<irr::u32> dimension(windowSize.Width, windowSize.Height / 15);
    irr::core::stringw initialFrameText("Frame count");
    frameCount = gui->addStaticText(initialFrameText.c_str(), irr::core::recti(0, 0, dimension.Width, dimension.Height));
    setFrameCount(0);

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

irr::gui::IGUIEnvironment *CameraWindow::getGUI() const
{
    return gui;
}

irr::gui::IGUIFont* CameraWindow::getGuiFont() const
{
    return guiFont;
}

void CameraWindow::updateScene()
{
    if(device->run()) {
        driver->beginScene(
                    true, // clear back-buffer
                    true, // clear z-buffer
                    backgroundColor);

        Engine& engine = Engine::getInstance();
        std::map<int, Player*> players = engine.getCourt()->getPlayers();

        // Render jersey number on player texture
        for(std::map<int, Player*>::iterator i = players.begin(); i != players.end(); ++i) {
            Player* p = i->second;

            irr::video::ITexture* rt = p->getRenderTexture();
            irr::video::ITexture* texture = p->getTexture();
            // Now we draw on texture instead of window
            driver->setRenderTarget(rt);
            // Solving OpenGL issue by resetting material
            driver->setMaterial(driver->getMaterial2D());
            driver->draw2DImage(texture, irr::core::vector2di(0, 0));
            jerseyFont->draw(p->getJerseyText(), p->getJerseyTextRect(), jerseyTextColor, true, true);

            // We go back to window (necessary to be able to switch, see API)
            driver->setRenderTarget(0, true, true, backgroundColor);
        }

        sceneManager->drawAll();

        // Solve another OpenGL issue by resetting material
        driver->setMaterial(driver->getMaterial2D());
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

irr::video::IImage* CameraWindow::createScreenshot()
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
    frameText = irr::core::stringw("");
    frameText += frameCountNew;
    frameCount->setText(frameText.c_str());
}

void CameraWindow::takeScreenshot(int time)
{
    irr::core::stringw str = "screenshots/scr_";
    str += time;
    str += ".png";
    irr::video::IImage* scr = createScreenshot();
    driver->writeImageToFile(scr, str);
}

irr::gui::IGUIFont* CameraWindow::getJerseyFont() const
{
    return jerseyFont;
}
