/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <irrlicht.h>
#include <vector>
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
         * @param inConsole sets if window must be minimized or not
         * @param initialWindowSize window size
         * @param bgColor background color of the window
         * @param jTextColor jersey text color
         * @param initialPosition initial position of the camera
         * @param initialRotation initial rotation of the camera
         * @param fontGUIPath Irrlicht user interface font
         * @param fontJerseyPath jersey text font
         * @param initialSpeed initial speed for FPS camera
         * @param fieldOfView angle of view of CameraWindow
         * @param initialTransformation array containing the scaling transformation in the first row and the offset transformation in the second one
         */
        void init(bool inConsole, const dimension2d<u32>& initialWindowSize, const SColor& bgColor, const SColor& jTextColor,
                const vector3df& initialPosition, const vector3df& initialRotation, const char* fontGUIPath, const char* fontJerseyPath, int initialSpeed,
                  float fieldOfView, const std::vector<vector3df>& initialTransformation);

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
         * Converts real coordinates in meters of a point to virtual Irrlicht coordinates
         * @param real position in meters
         * @return virtual position of the input point
         */
        vector3df convertToVirtual(vector3df real);

        /**
         * Converts virtual Irrlicht coordinates to real coordinates in meters
         * @param vrtl virtual position
         * @return real position of the input point
         */
        vector3df convertToReal(vector3df vrtl);

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
        std::vector<vector3df> transformation;

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
