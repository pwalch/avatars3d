#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <irrlicht.h>
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
        ~CameraWindow();

        /**
         * Initializes window and camera
         * @param initialWindowSize dimensions of window
         * @param initialPosition camera position vector
         * @param initialRotation camera rotation vector
         */
        void init(const irr::core::dimension2d<irr::u32>& initialWindowSize, const irr::core::vector3df& initialPosition, const irr::core::vector3df& initialRotation, const char* fontPath, int initialSpeed);

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
         * Returns screenshot of current display
         * @return pointer to Irrlicht image
         */
        irr::video::IImage* getScreenshot();

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

        irr::gui::IGUIEnvironment* getGUI();

        irr::gui::IGUIFont* getFont();

        int getSpeed() const;

        void setFrameCount(int frameCountNew);

        void takeScreenshot(int time);

private:
        CameraWindow() {}
        CameraWindow& operator= (const CameraWindow&) { }
        CameraWindow(const CameraWindow&) {}

        irr::core::dimension2di windowSize;
        irr::IrrlichtDevice *device;
        irr::video::IVideoDriver* driver;

        irr::gui::IGUIEnvironment* gui;
        irr::gui::IGUIFont* font;
        irr::core::stringw frameText;
        irr::gui::IGUIStaticText* frameCount;

        EventManager* eventManager;

        irr::scene::ISceneManager *sceneManager;
        irr::scene::ICameraSceneNode* staticCamera;
        int speed;

};

#endif // CAMERAWINDOW_H
