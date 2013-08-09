/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <irrlicht.h>
#include "eventmanager.h"

using namespace irr;
using namespace irr::core;
using namespace irr::gui;
using namespace irr::scene;
using namespace irr::video;

class EventManager;

/**
 * Irrlicht window singleton, handling scene and especially camera
 */
class CameraWindow
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
        ~CameraWindow();

        /**
         * Initializes the window
         * @param initialWindowSize window size
         * @param bgColor background color of the window
         * @param jTextColor jersey text color
         * @param initialPosition initial position of the camera
         * @param initialRotation initial rotation of the camera
         * @param fontGUIPath Irrlicht user interface font
         * @param fontJerseyPath jersey text font
         * @param initialSpeed initial speed for FPS camera
         */
        void init(const dimension2d<u32>& initialWindowSize, const SColor& bgColor, const SColor& jTextColor, const vector3df& initialPosition, const vector3df& initialRotation, const char* fontGUIPath, const char* fontJerseyPath, int initialSpeed);

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
         * Returns Irrlicht window dimensions
         * @return Irrlicht window dimensions
         */
        const dimension2di& getWindowSize() const;

        /**
         * Creates a screenshot of current display and returns it
         * @return pointer to Irrlicht image
         */
        IImage* createScreenshot();


        /**
         * Takes a screenshot and saves it in screenshot folder
         * @param time instant when the screenshot must be taken
         */
        void takeScreenshot(int time);

        /**
         * Returns camera position
         * @return camera position
         */
        const vector3df& getCameraPosition() const;

        /**
         * Returns camera rotation
         * @return camera rotation
         */
        const vector3df& getCameraRotation() const;

        /**
         * Sets camera position
         * @param position new position of the camera
         */
        void setPosition(const vector3df& position);

        /**
         * Sets camera rotation
         * @param rotation new rotation of the camera
         */
        void setRotation(const vector3df& rotation);

        /**
         * Moves camera using movement vector
         * @param moveVector movement to perform
         */
        void move(const vector3df& moveVector);

        /**
         * Rotates camera using rotation vector
         * @param rotationVector rotation to perform
         */
        void rotate(const vector3df& rotationVector);

        /**
         * Returns initial speed of FPS camera
         * @return FPS camera initial speed
         */
        int getSpeed() const;

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

    private:
        // Singleton functions
        CameraWindow() {}
        CameraWindow& operator= (const CameraWindow&) { }
        CameraWindow(const CameraWindow&) {}

        // Window
        dimension2di windowSize;
        SColor backgroundColor;

        // Irrlicht components
        IrrlichtDevice *device;
        IVideoDriver* driver;
        ISceneManager *sceneManager;
        EventManager* eventManager;

        // Camera
        ICameraSceneNode* staticCamera;
        int speed;

        // Irrlicht GUI
        IGUIEnvironment* gui;
        IGUIFont* guiFont;
        IGUIFont* jerseyFont;
        stringw frameText;
        IGUIStaticText* frameCount;

        // Player jerseys
        SColor jerseyTextColor;


};

#endif // CAMERAWINDOW_H
