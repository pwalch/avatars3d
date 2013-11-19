/*
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


// Singleton member declaration to avoid link error
#ifndef SINGLETON_COMPILE_TIME_CHECK
std::auto_ptr<CameraWindow> CameraWindow::sInstance;
#endif

CameraWindow::~CameraWindow()
{
    delete mEventManager;
    mDevice->drop();
}

CameraWindow* CameraWindow::getInstance()
{
    return sInstance.get();
}

vector3df CameraWindow::getRealPosition()
{
    return Engine::getInstance().getTransformation()->convertToReal(mStaticCamera->getPosition());
}

void CameraWindow::setRealPosition(const vector3df &position)
{
    setVirtualPosition(Engine::getInstance().getTransformation()->convertToVirtual(position));
}

void CameraWindow::setVirtualPosition(const vector3df& virtualPosition)
{
    vector3df rotation = getRotation();
    mStaticCamera->setPosition(virtualPosition);
    // setTarget uses absolute position member so we need to update it every time position is changed
    mStaticCamera->updateAbsolutePosition();
    // Call setRotation to trigger setTarget
    mStaticCamera->setRotation(rotation);
}


void CameraWindow::moveVirtual(const vector3df& moveVector)
{
    vector3df pos = mStaticCamera->getPosition();
    vector3df target = (mStaticCamera->getTarget() - mStaticCamera->getAbsolutePosition());

    // Forward direction is the target direction
    vector3df forwardDirection = target;
    forwardDirection.normalize();
    pos += forwardDirection * moveVector.X;

    // Up vector is given in camera definition
    vector3df upDirection = mStaticCamera->getUpVector();
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
    setVirtualPosition(pos);
    target += pos;

    mStaticCamera->setTarget(target);
}

const vector3df& CameraWindow::getRotation() const
{
    return mStaticCamera->getRotation();
}

void CameraWindow::setRotation(const vector3df& rotation)
{
    mStaticCamera->setRotation(rotation);
}

void CameraWindow::rotate(const vector3df& rotationVector)
{
    vector3df target = (mStaticCamera->getTarget() - mStaticCamera->getAbsolutePosition());
    vector3df relativeRotation = target.getHorizontalAngle();
    relativeRotation.Y -= rotationVector.Y;
    relativeRotation.X -= rotationVector.X;

    const f32 MaxVerticalAngle = 88.0f;
    // X < MaxVerticalAngle or X > 360-MaxVerticalAngle
    if (relativeRotation.X > MaxVerticalAngle*2 &&
        relativeRotation.X < 360.0f-MaxVerticalAngle) {
        relativeRotation.X = 360.0f-MaxVerticalAngle;
    } else if (relativeRotation.X > MaxVerticalAngle &&
               relativeRotation.X < 360.0f-MaxVerticalAngle) {
        relativeRotation.X = MaxVerticalAngle;
    }
    target.set(0,0, max_(1.f, mStaticCamera->getPosition().getLength()));
    matrix4 mat;
    mat.setRotationDegrees(vector3df(relativeRotation.X, relativeRotation.Y, 0));
    mat.transformVect(target);

    target += mStaticCamera->getPosition();

    mStaticCamera->setTarget(target);
}

void CameraWindow::updateScene()
{
    mDriver->beginScene(
                true, // clear back-buffer
                true, // clear z-buffer
                mSettings.mBgColor);

    Engine& engine = Engine::getInstance();


    static bool areJerseyNumbersGiven = false;

    if(!areJerseyNumbersGiven) {
        std::map<int, Player*> players = engine.getCourt()->getPlayers();
        // Render jersey number on player texture
        for(std::map<int, Player*>::iterator i = players.begin();
                i != players.end(); ++i) {
            Player* p = i->second;

            // Virtual surface where to draw
            ITexture* rt = p->getRenderTexture();
            // Actual player texture with its color but without jersey number
            ITexture* texture = p->getTexture();
            // Now we draw on texture instead of window
            mDriver->setRenderTarget(rt);

            // Draw actual player texture
            mDriver->setMaterial(mDriver->getMaterial2D());
            mDriver->draw2DImage(texture, vector2di(0, 0));

            // Draw jersey number over it
            mDriver->setMaterial(mDriver->getMaterial2D());
            mJerseyFont->draw(p->getJerseyText(),
                              p->getPlayerSettings().mJerseyTextRect,
                              mSettings.mJerseyTextColor, true, true);

            // We go back to window (necessary to be able to switch, see API)
            mDriver->setRenderTarget(0, true, true, mSettings.mBgColor);
        }
        areJerseyNumbersGiven = true;
    }

    mSceneManager->drawAll();

    if(mSettings.mDisplayAxes) {
        float scaleAxes = 100;
        vector3df o(0, 0, 0);
        vector3df x(scaleAxes, 0, 0);
        SColor colorX(255, 255, 0, 0);
        vector3df y(0, scaleAxes, 0);
        SColor colorY(255, 0, 255, 0);
        vector3df z(0, 0, scaleAxes);
        SColor colorZ(255, 0, 0, 255);
        mDriver->draw3DLine(o, x, colorX);
        mDriver->draw3DLine(o, y, colorY);
        mDriver->draw3DLine(o, z, colorZ);
    }

//        // Testing coordinates
//        vector3df posBegin(6100, 4300, 0);
//        const float rd = 10;
//        vector3df posEnd(posBegin.X + rd, posBegin.Y, posBegin.Z);
//        driver->draw3DLine(convertToVirtual(posBegin),
//                      convertToVirtual(posEnd), SColor(255, 0, 255, 0));

    // Solve another OpenGL issue by resetting material
    mDriver->setMaterial(mDriver->getMaterial2D());
    mGui->drawAll();

    mDriver->endScene();
}

IrrlichtDevice* CameraWindow::getDevice() const
{
    return mDevice;
}

ISceneManager* CameraWindow::getSceneManager() const
{
    return mSceneManager;
}

IVideoDriver* CameraWindow::getDriver() const
{
    return mDriver;
}

IImage* CameraWindow::createScreenshot()
{
    IImage* screenshot = mDriver->createScreenShot();
    return screenshot;
}

void CameraWindow::setFrameCount(int frameCountNew)
{
    mFrameText = stringw("");
    mFrameText += frameCountNew;
    mFrameCount->setText(mFrameText.c_str());
}

void CameraWindow::takeScreenshot(int systemTime)
{
    stringw str = "screenshot_";
    str += systemTime;
    str += ".png";
    IImage* scr = createScreenshot();
    mDriver->writeImageToFile(scr, str);
}

IGUIFont* CameraWindow::getJerseyFont() const
{
    return mJerseyFont;
}

void CameraWindow::setTime(int time)
{
    if(mSettings.mFollowTrajectoryFile && mTrajectoryData->containsPositionFrame(time))
    {
        setVirtualPosition(mTrajectoryData->getPositionAt(time));
        setRotation(mTrajectoryData->getRotationAt(time));
    }

    setFrameCount(time);
}

const CameraSettings &CameraWindow::getSettings() const
{
    return mSettings;
}

void CameraWindow::setFollowTrajectoryFile(bool isFollowingTrajectoryFile)
{
    mSettings.mFollowTrajectoryFile = isFollowingTrajectoryFile;
}

CameraWindow::CameraWindow(TrajectoryData *trajectoryData, const CameraSettings& cameraSettings)
    : Moveable(trajectoryData)
{

    this->mSettings = cameraSettings;

    SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
    // Multisampling with many samples
    params.AntiAlias = 16;
    params.Bits = 32;
    // Using OpenGL for rendering
    params.DriverType = EDT_OPENGL;
    params.Doublebuffer = true;
    params.Fullscreen = mSettings.mFullScreen;
    params.HighPrecisionFPU = false;
    params.IgnoreInput = false;
    // Display no log entry
    params.LoggingLevel = ELL_NONE;
    params.Stencilbuffer = false;
    params.Stereobuffer = false;
    // We disable vertical synchronization to avoid performance clamping
    params.Vsync = false;
    params.WindowId = 0;
    params.WindowSize = mSettings.mWindowSize;
    params.WithAlphaChannel = false;
    params.ZBufferBits = 16;
    mDevice = createDeviceEx(params);
    mDriver = mDevice->getVideoDriver();
    mSceneManager = mDevice->getSceneManager();

    mDevice->setResizable(false);

    if(mSettings.mInConsole) {
        // Minimize window with X11 directly. XUnmapWindow() can completely
        // remove the window
//        const SExposedVideoData& vData = driver->getExposedVideoData();
//        void* X11Display = vData.OpenGLLinux.X11Display;
//        unsigned long X11Window = vData.OpenGLLinux.X11Window;
//        int screen = 0;
//        XIconifyWindow((Display*)X11Display, X11Window, screen);

        // Minimize window with Irrlicht
//        device->minimizeWindow();
    }

    mDevice->setWindowCaption(L"3D View");
    // Stop device timer because we do not use it
    mDevice->getTimer()->stop();

    // Add camera and link rotation with target (rotation affects target)
    mStaticCamera = mSceneManager->addCameraSceneNode();
    mStaticCamera->bindTargetAndRotation(true);
    mStaticCamera->setFarValue(30000);
    mStaticCamera->setFOV(mSettings.mFieldOfView);

    // Create event manager to handle keyboard and mouse inputs from Irrlicht
    mEventManager = new EventManager();
    mDevice->setEventReceiver(mEventManager);

    Engine& engine = Engine::getInstance();

    // Create GUI environment to use fonts and display 2D texts
    mGui = mDevice->getGUIEnvironment();
    mGuiFont = mGui->getFont(mSettings.mFontGUIPath);
    if(mGuiFont == NULL) {
        engine.throwError(L"Gui font could not be loaded");
    }
    mJerseyFont = mGui->getFont(mSettings.mFontJerseyPath);
    if(mJerseyFont == NULL) {
        engine.throwError(L"Jersey font could not be loaded");
    }
    mJerseyFont->setKerningWidth(50);

    // Set default font
    IGUISkin* skin = mGui->getSkin();
    skin->setFont(mGuiFont);

    // Display frame count on top left corner
    dimension2d<u32> dimension(mSettings.mWindowSize.Width, mSettings.mWindowSize.Height / 15);
    stringw initialFrameText("Frame count");
    mFrameCount = mGui->addStaticText(initialFrameText.c_str(), recti(0, 0, dimension.Width, dimension.Height));
    mFrameCount->setOverrideColor(SColor(255, 255, 255, 255));
}
