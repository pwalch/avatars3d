/*
  * 3D Avatars
  * Pierre Walch
  */

#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <memory>
#include <irrlicht.h>
#include <vector>
#include "eventmanager.h"
#include "camerasettings.h"
#include "moveable.h"

using namespace irr;
using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;

class EventManager;

/**
 * @brief Irrlicht window singleton
 *
 * Represents Irrlicht window. You must call constructCamera() in AvatarsFactory before using getInstance().
 * @see AvatarsFactory::constructCamera()
 */
class CameraWindow : public Moveable
{

    public:

        /**
         * Constructs 3D view
         * @param trajectoryData camera trajectory
         * @param cameraSettings camera settings
         */
        CameraWindow(TrajectoryData *trajectoryData, const CameraSettings& cameraSettings);

        /**
         * Destroys Irrlicht view and event manager
         */
        virtual ~CameraWindow();

        /**
         * Displays current scene in the window. Draws the scene background, then the players with the jersey number
         * printed on their texture, then the axes (if settings allow it), then the frame index in top-left corner
         */
        void updateScene();

        /**
         * Returns Irrlicht device
         * @return device
         */
        IrrlichtDevice* getDevice() const;

        /**
         * Returns Irrlicht scene manager
         * @return scene manager
         */
        ISceneManager* getSceneManager() const;

        /**
         * Returns Irrlicht video driver
         * @return driver
         */
        IVideoDriver* getDriver() const;

        /**
         * Creates a screenshot of the window and returns it
         * @return pointer to Irrlicht image
         * @see takeScreenshot();
         */
        IImage* createScreenshot();

        /**
         * Takes a screenshot and saves it in screenshot folder
         * @param systemTime system time when the user takes the screenshot
         * @see createScreenshot()
         */
        void takeScreenshot(int systemTime);

        /**
         * Returns position of the camera in real coordinates
         * @return real camera position
         */
        vector3df getRealPosition();

        /**
         * Sets the position of the camera with real coordinates
         * @param realPosition real position
         */
        void setRealPosition(const vector3df& realPosition);

        /**
         * Returns camera rotation
         * @return camera rotation
         */
        const vector3df& getRotation() const;

        /**
         * Sets camera rotation
         * @param rotation new rotation of the camera
         */
        void setRotation(const vector3df& rotation);

        /**
         * Moves camera using movement vector in virtual coordinates
         * @param moveVirtualVector movement to perform
         * @see setPosition();
         */
        void moveVirtual(const vector3df& moveVirtualVector);

        /**
         * Rotates camera using rotation vector
         * @param rotationVector rotation to perform
         * @see setRotation();
         */
        void rotate(const vector3df& rotationVector);

        /**
         * Returns Irrlicht font of the jersey text
         * @return jersey text font
         */
        IGUIFont* getJerseyFont() const;

        /**
         * Moves the camera to the position and rotation corresponding to the given time value, and updates
         * frame count text on top-left corner of the window
         * @param time frame index
         */
        void setTime(int time);

        /**
         * Returns camera settings
         * @return camera settings
         * @see CameraSettings
         */
        const CameraSettings& getSettings() const;

        /**
         * Sets whether camera trajectory file must be followed. If so, setTime() will move the camera according
         * to trajectory file. If not, setTime() won't move the camera.
         * @param isFollowingTrajectoryFile following state
         */
        void setFollowTrajectoryFile(bool isFollowingTrajectoryFile);


    private:

        // Singleton must be unique
        CameraWindow& operator= (const CameraWindow&);
        CameraWindow(const CameraWindow&);

        /**
         * Sets camera virtual position
         * @param virtualPosition virtual position
         */
        void setVirtualPosition(const vector3df& virtualPosition);

        /**
         * Updates the frame count text drawn on top-left corner of the window
         * @param frameCountNew new frame count
         */
        void setFrameCount(int frameCountNew);

        CameraSettings mSettings;

        // Irrlicht scene and video components
        IrrlichtDevice *mDevice;
        IVideoDriver* mDriver;
        ISceneManager *mSceneManager;
        EventManager* mEventManager;
        ICameraSceneNode* mStaticCamera;

        // Irrlicht GUI components
        IGUIEnvironment* mGui;
        IGUIFont* mGuiFont;
        stringw mFrameText;
        IGUIStaticText* mFrameCount;
        IGUIFont* mJerseyFont;

};

#endif // CAMERAWINDOW_H
