/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <irrlicht.h>
#include "player.h"
#include "eventmanager.h"

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
        void init(const irr::core::dimension2d<irr::u32>& initialWindowSize, const irr::video::SColor& bgColor, const irr::video::SColor& jTextColor, const irr::core::vector3df& initialPosition, const irr::core::vector3df& initialRotation, const char* fontGUIPath, const char* fontJerseyPath, int initialSpeed);

        /**
         * Updates scene with current camera and court
         */
        void updateScene();

        /**
         * Returns Irrlicht device
         * @return Irrlicht device
         */
        irr::IrrlichtDevice* getDevice() const;

        /**
         * Returns scene manager
         * @return scene manager
         */
        irr::scene::ISceneManager* getSceneManager() const;

        /**
         * Returns Irrlicht driver
         * @return Irrlicht driver
         */
        irr::video::IVideoDriver* getDriver() const;

        /**
         * Returns Irrlicht window dimensions
         * @return Irrlicht window dimensions
         */
        const irr::core::dimension2di& getWindowSize() const;

        /**
         * Creates a screenshot of current display and returns it
         * @return pointer to Irrlicht image
         */
        irr::video::IImage* createScreenshot();


        /**
         * Takes a screenshot and saves it in screenshot folder
         * @param time instant when the screenshot must be taken
         */
        void takeScreenshot(int time);

        /**
         * Returns camera position
         * @return camera position
         */
        const irr::core::vector3df& getCameraPosition() const;

        /**
         * Returns camera rotation
         * @return camera rotation
         */
        const irr::core::vector3df& getCameraRotation() const;

        /**
         * Sets camera position
         * @param position new position of the camera
         */
        void setPosition(const irr::core::vector3df& position);

        /**
         * Sets camera rotation
         * @param rotation new rotation of the camera
         */
        void setRotation(const irr::core::vector3df& rotation);

        /**
         * Moves camera using movement vector
         * @param moveVector movement to perform
         */
        void move(const irr::core::vector3df& moveVector);

        /**
         * Rotates camera using rotation vector
         * @param rotationVector rotation to perform
         */
        void rotate(const irr::core::vector3df& rotationVector);

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
        irr::gui::IGUIEnvironment* getGUI() const;

        /**
         * Returns font used in Irrlicht user interface
         * @return Irrlicht GUI font
         */
        irr::gui::IGUIFont* getGuiFont() const;

        /**
         * Returns font of the jersey text
         * @return jersey text font
         */
        irr::gui::IGUIFont* getJerseyFont() const;

    private:
        // Singleton functions
        CameraWindow() {}
        CameraWindow& operator= (const CameraWindow&) { }
        CameraWindow(const CameraWindow&) {}

        // Window
        irr::core::dimension2di windowSize;
        irr::video::SColor backgroundColor;

        // Irrlicht components
        irr::IrrlichtDevice *device;
        irr::video::IVideoDriver* driver;
        irr::scene::ISceneManager *sceneManager;
        EventManager* eventManager;

        // Camera
        irr::scene::ICameraSceneNode* staticCamera;
        int speed;

        // Irrlicht GUI
        irr::gui::IGUIEnvironment* gui;
        irr::gui::IGUIFont* guiFont;
        irr::gui::IGUIFont* jerseyFont;
        irr::core::stringw frameText;
        irr::gui::IGUIStaticText* frameCount;

        // Player jerseys
        irr::video::SColor jerseyTextColor;


};

#endif // CAMERAWINDOW_H
