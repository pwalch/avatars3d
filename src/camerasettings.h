#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include "affinetransformation.h"

/**
 * @brief Camera initializer settings
 *
 * Contains all the necessary properties to initialize the camera.
 */
class CameraSettings
{
    public:

        /**
         * True if console mode (only console and Irrlicht window),
         * false if Qt window enabled (console, Irrlicht window and Qt window)
         */
        bool inConsole;

        /**
         * Irrlicht window size
         */
        dimension2d<u32> windowSize;

        /**
         * Background color of 3D view
         */
        SColor bgColor;

        /**
         * Color of GUI text
         */
        SColor guiColor;

        /**
         * Color of jersey text
         */
        SColor jerseyTextColor;

        /**
         * Path to GUI text font
         */
        const char* fontGUIPath;

        /**
         * Path to jersey number font
         */
        const char* fontJerseyPath;

        /**
         * FPS camera speed scale
         */
        float fpsScale;

        /**
         * Camera field of view
         */
        float fieldOfView;

        /**
         * Specifies whether virtual Irrlicht axes have to be displayed or not
         */
        bool displayAxes;

        /**
         * Specifies whether fullscreen is enabled or not for Irrlicht window
         */
        bool fullScreen;
};

#endif // CAMERASETTINGS_H
