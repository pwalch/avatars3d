/**
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <QTime>

#include "engine.h"
#include "camerawindow.h"

using namespace irr;
using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;

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

void CameraWindow::init(bool isConsole, const dimension2d<u32>& initialWindowSize, const SColor& bgColor, const SColor& guiTextColor, const SColor& jTextColor,
                        const char* fontGUIPath, const char* fontJerseyPath, float initialScale, float fieldOfView, const std::vector<vector3df>& initialTransformation, bool dspAxes)
{
    SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
    // Multisampling with 64 samples
    params.AntiAlias = 64;
    params.Bits = 32;
    // Using OpenGL for rendering
    params.DriverType = EDT_OPENGL;
    params.Doublebuffer = true;
    params.Fullscreen = false;
    params.HighPrecisionFPU = false;
    params.IgnoreInput = false;
    // Display no log entry
    params.LoggingLevel = ELL_NONE;
    params.Stencilbuffer = false;
    params.Stereobuffer = false;
    // We disable vertical synchronization to avoid performance clamping
    params.Vsync = false;
    params.WindowId = 0;
    params.WindowSize = initialWindowSize;
    params.WithAlphaChannel = false;
    params.ZBufferBits = 16;
    device = createDeviceEx(params);
    driver = device->getVideoDriver();
    sceneManager = device->getSceneManager();

//    X11 window handling
//    const SExposedVideoData& vData = driver->getExposedVideoData();
//    void* X11Display = vData.OpenGLLinux.X11Display;
//    unsigned long X11Window = vData.OpenGLLinux.X11Window;
//    XUnmapWindow((Display*)X11Display, X11Window);


    if(isConsole) {
        device->minimizeWindow();
    } else {
         device->setResizable(false);
    }

    device->setWindowCaption(L"3D View");
    windowSize = initialWindowSize;
    displayAxes = dspAxes;

    // Set background color, gui text color and jersey text color
    backgroundColor = bgColor;
    guiColor = guiTextColor;
    jerseyTextColor = jTextColor;
    // Stop device timer because we do not use it
    device->getTimer()->stop();


    transformation = initialTransformation;
    // Add camera and link rotation with target (rotation affects target)
    staticCamera = sceneManager->addCameraSceneNode();
    staticCamera->bindTargetAndRotation(true);
    staticCamera->setFarValue(30000);
    staticCamera->setFOV(fieldOfView);
    // Set FPS camera speed (for user interface)
    fpsScale = initialScale;

    // Create event manager to handle keyboard and mouse inputs from Irrlicht
    eventManager = new EventManager();
    device->setEventReceiver(eventManager);

    // Create GUI environment to use fonts and display 2D texts
    gui = device->getGUIEnvironment();
    guiFont = gui->getFont(fontGUIPath);
    jerseyFont = NULL;
    jerseyFont = gui->getFont(fontJerseyPath);

    // Set default font
    IGUISkin* skin = gui->getSkin();
    skin->setFont(guiFont);

    // Display frame count on top left corner
    dimension2d<u32> dimension(windowSize.Width, windowSize.Height / 15);
    stringw initialFrameText("Frame count");
    frameCount = gui->addStaticText(initialFrameText.c_str(), recti(0, 0, dimension.Width, dimension.Height));
    frameCount->setOverrideColor(SColor(255, 255, 255, 255));
}

const vector3df& CameraWindow::getPosition() const
{
    return staticCamera->getPosition();
}

vector3df CameraWindow::getRealPosition()
{
    return convertToReal(staticCamera->getPosition());
}

const vector3df& CameraWindow::getRotation() const
{
    return staticCamera->getRotation();
}

void CameraWindow::setPosition(const vector3df& position)
{
    vector3df rotation = getRotation();
    staticCamera->setPosition(position);
    // setTarget uses absolute position member so we need to update it every time position is changed
    staticCamera->updateAbsolutePosition();
    // Call setRotation to trigger setTarget
    staticCamera->setRotation(rotation);
}

void CameraWindow::setRealPosition(const vector3df &position)
{
    setPosition(convertToVirtual(position));
}

void CameraWindow::setRotation(const vector3df& rotation)
{
    staticCamera->setRotation(rotation);
}

void CameraWindow::move(const vector3df& moveVector)
{
    vector3df pos = staticCamera->getPosition();
    vector3df target = (staticCamera->getTarget() - staticCamera->getAbsolutePosition());

    // Forward direction is the target direction
    vector3df forwardDirection = target;
    forwardDirection.normalize();
    pos += forwardDirection * moveVector.X;

    // Up vector is given in camera definition
    vector3df upDirection = staticCamera->getUpVector();
    upDirection.normalize();
    pos += upDirection * moveVector.Z;

    // Left direction is cross product of forward and up vectors
    vector3df leftDirection = target.crossProduct(upDirection);
    leftDirection.normalize();
    pos += leftDirection * moveVector.Y;

    // Apply rotation to keep camera in the same relative direction
    vector3df relativeRotation = target.getHorizontalAngle();
    target.set(0,0, max_(1.f, pos.getLength()));
    matrix4 mat;
    mat.setRotationDegrees(vector3df(relativeRotation.X, relativeRotation.Y, 0));
    mat.transformVect(target);

    // Moving camera to its new position and adapt target
    setPosition(pos);
    target += pos;

    staticCamera->setTarget(target);
}

void CameraWindow::rotate(const vector3df& rotationVector)
{
    vector3df target = (staticCamera->getTarget() - staticCamera->getAbsolutePosition());
    vector3df relativeRotation = target.getHorizontalAngle();
    relativeRotation.Y -= rotationVector.Y;
    relativeRotation.X -= rotationVector.X;

    const f32 MaxVerticalAngle = 88.0f;
    // X < MaxVerticalAngle or X > 360-MaxVerticalAngle
    if (relativeRotation.X > MaxVerticalAngle*2 && relativeRotation.X < 360.0f-MaxVerticalAngle) {
        relativeRotation.X = 360.0f-MaxVerticalAngle;
    } else if (relativeRotation.X > MaxVerticalAngle && relativeRotation.X < 360.0f-MaxVerticalAngle) {
        relativeRotation.X = MaxVerticalAngle;
    }
    target.set(0,0, max_(1.f, staticCamera->getPosition().getLength()));
    matrix4 mat;
    mat.setRotationDegrees(vector3df(relativeRotation.X, relativeRotation.Y, 0));
    mat.transformVect(target);

    target += staticCamera->getPosition();

    staticCamera->setTarget(target);
}

IGUIEnvironment *CameraWindow::getGUI() const
{
    return gui;
}

IGUIFont* CameraWindow::getGuiFont() const
{
    return guiFont;
}

void CameraWindow::updateScene()
{
    driver->beginScene(
                true, // clear back-buffer
                true, // clear z-buffer
                backgroundColor);

    Engine& engine = Engine::getInstance();
    std::map<int, Player*> players = engine.getCourt()->getPlayers();
    // Render jersey number on player texture
    for(std::map<int, Player*>::iterator i = players.begin(); i != players.end(); ++i) {
        Player* p = i->second;

        ITexture* rt = p->getRenderTexture();
        ITexture* texture = p->getTexture();
        // Now we draw on texture instead of window
        driver->setRenderTarget(rt);
        // Solving OpenGL issue by resetting material
        driver->setMaterial(driver->getMaterial2D());
        driver->draw2DImage(texture, vector2di(0, 0));
        jerseyFont->draw(p->getJerseyText(), p->getJerseyTextRect(), jerseyTextColor, true, true);

        // We go back to window (necessary to be able to switch, see API)
        driver->setRenderTarget(0, true, true, backgroundColor);
    }

    sceneManager->drawAll();

    if(displayAxes) {
        float scaleAxes = 100;
        vector3df o(0, 0, 0);
        vector3df x(scaleAxes, 0, 0);
        SColor colorX(255, 255, 0, 0);
        vector3df y(0, scaleAxes, 0);
        SColor colorY(255, 0, 255, 0);
        vector3df z(0, 0, scaleAxes);
        SColor colorZ(255, 0, 0, 255);
        driver->draw3DLine(o, x, colorX);
        driver->draw3DLine(o, y, colorY);
        driver->draw3DLine(o, z, colorZ);
    }

//        // Testing coordinates
//        vector3df posBegin(6100, 4300, 0);
//        const float rd = 10;
//        vector3df posEnd(posBegin.X + rd, posBegin.Y, posBegin.Z);
//        driver->draw3DLine(convertToVirtual(posBegin), convertToVirtual(posEnd), SColor(255, 0, 255, 0));

    // Solve another OpenGL issue by resetting material
    driver->setMaterial(driver->getMaterial2D());
    gui->drawAll();

    driver->endScene();
}

IrrlichtDevice* CameraWindow::getDevice() const
{
    return device;
}

const dimension2di& CameraWindow::getWindowSize() const
{
    return windowSize;
}

ISceneManager* CameraWindow::getSceneManager() const
{
    return sceneManager;
}

IVideoDriver* CameraWindow::getDriver() const
{
    return driver;
}

IImage* CameraWindow::createScreenshot()
{
    IImage* screenshot = driver->createScreenShot();
    return screenshot;
}

float CameraWindow::getFpsScale() const
{
    return fpsScale;
}

void CameraWindow::setFrameCount(int frameCountNew)
{
    frameText = stringw("");
    frameText += frameCountNew;
    frameCount->setText(frameText.c_str());
}

void CameraWindow::takeScreenshot(int systemTime)
{
    stringw str = "screenshot_";
    str += systemTime;
    str += ".png";
    IImage* scr = createScreenshot();
    driver->writeImageToFile(scr, str);
}

IGUIFont* CameraWindow::getJerseyFont() const
{
    return jerseyFont;
}

void CameraWindow::setTime(int time)
{
    // We do not display camera trajectory because it obscures the view
    // Moveable::setTime(time);

    if(virtualTrajectory.find(time) != virtualTrajectory.end())
    {
        setPosition(virtualTrajectory[time]);
        setRotation(rotationAngle[time]);
    }

    setFrameCount(time);
}

vector3df CameraWindow::convertToVirtual(vector3df real)
{
    return vector3df(real.X * transformation[0].X + transformation[1].X, real.Z * transformation[0].Z + transformation[1].Z, real.Y * transformation[0].Y + transformation[1].Y);
}

vector3df CameraWindow::convertToReal(vector3df vrtl)
{
    return vector3df((vrtl.X - transformation[1].X)/transformation[0].X, (vrtl.Z - transformation[1].Y)/transformation[0].Y, (vrtl.Y - transformation[1].Z) / transformation[0].Z);
}
