/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <irrlicht.h>
#include <vector>
#include "eventmanager.h"
#include "moveable.h"
#include "camerasettings.h"

using namespace irr;
using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;

class EventManager;

/**
 * @brief Irrlicht window singleton (view)
 *
 * This class represents view in MVC pattern. It handles scene display and
 * camera. Call init(), mapTime() and prepareMove() in
 * this order before using its methods.
 */
class CameraWindow : public Moveable
{
    public:

        /**
         * Returns instance of singleton class
         * @return camera window instance
         */
        static CameraWindow& getInstance();

        /**
         * Destroys Irrlicht view
         */
        virtual ~CameraWindow();

        /**
         * Initializes the window
         */
        void init(const CameraSettings& settings);

        /**
         * Updates scene with current camera and court
         */
        void updateScene();

        /**
         * Returns Irrlicht device
         * @return Irrlicht device
         */
        IrrlichtDevice* getDevice() const;

        /**
         * Returns scene manager
         * @return scene manager
         */
        ISceneManager* getSceneManager() const;

        /**
         * Returns Irrlicht driver
         * @return Irrlicht driver
         */
        IVideoDriver* getDriver() const;

        /**
         * Creates a screenshot of current display and returns it
         * @return pointer to Irrlicht image
         */
        IImage* createScreenshot();

        /**
         * Takes a screenshot and saves it in screenshot folder
         * @param systemTime system time when the user takes the screenshot
         */
        void takeScreenshot(int systemTime);

        /**
         * Returns camera position
         * @return camera position
         */
        const vector3df& getPosition() const;

        /**
         * Returns real position in meters of the camera
         * @return real position of the camera
         */
        vector3df getRealPosition();

        /**
         * Returns camera rotation
         * @return camera rotation
         */
        const vector3df& getRotation() const;

        /**
         * Sets camera virtual position
         * @param position new virtual position of the camera
         */
        void setPosition(const vector3df& position);

        /**
         * Sets the position of the camera with real coordinates in meters
         * @param position new real position in meters
         */
        void setRealPosition(const vector3df& position);

        /**
         * Sets camera rotation
         * @param rotation new rotation of the camera
         */
        void setRotation(const vector3df& rotation);

        /**
         * Moves camera using movement vector in virtual coordinates
         * @param moveVector movement to perform
         */
        void move(const vector3df& moveVector);

        /**
         * Rotates camera using rotation vector
         * @param rotationVector rotation to perform
         */
        void rotate(const vector3df& rotationVector);

        /**
         * Sets the frame count and updates the text in the GUI
         * @param frameCountNew new frame count
         */
        void setFrameCount(int frameCountNew);

        /**
         * Returns GUI environment
         * @return GUI environment
         */
        IGUIEnvironment* getGUI() const;

        /**
         * Returns font used in Irrlicht user interface
         * @return Irrlicht GUI font
         */
        IGUIFont* getGuiFont() const;

        /**
         * Returns font of the jersey text
         * @return jersey text font
         */
        IGUIFont* getJerseyFont() const;

        void setTime(int time);

        /**
         * Returns camera settings
         * @return camera settings
         */
        const CameraSettings& getSettings() const;

    private:
        // Singleton functions
        CameraWindow() {}
        CameraWindow& operator= (const CameraWindow&) { }
        CameraWindow(const CameraWindow&) {}

        CameraSettings settings;

        // Irrlicht components
        IrrlichtDevice *device;
        IVideoDriver* driver;
        ISceneManager *sceneManager;
        EventManager* eventManager;

        // Camera
        ICameraSceneNode* staticCamera;

        // Irrlicht GUI
        IGUIEnvironment* gui;
        IGUIFont* guiFont;
        IGUIFont* jerseyFont;
        stringw frameText;
        IGUIStaticText* frameCount;

};

#endif // CAMERAWINDOW_H
