#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <irrlicht.h>

using namespace irr;
using namespace irr::core;
using namespace irr::video;

/**
 * @brief Camera initializer settings
 *
 * Contains all the necessary properties to initialize the camera.
 */
class CameraSettings
{
    public:

        CameraSettings() {
            mInConsole = false;
            mUseTrajectoryFile = false;
            mWindowSize = dimension2d<u32>(0, 0);
            mBgColor = SColor(0, 0, 0, 0);
            mGuiColor = SColor(0, 0, 0, 0);
            mJerseyTextColor = SColor(0, 0, 0, 0);
            mFontGUIPath = "";
            mFontJerseyPath = "";
            mFpsScale = 0.0;
            mFieldOfView = 0.0;
            mDisplayAxes = false;
            mFullScreen = false;
        }

        /**
         * True if console mode (only console and Irrlicht window),
         * false if GUI mode (console, Irrlicht window and Qt window)
         */
        bool mInConsole;

        /**
         * Whether the camera is moved according to camera trajectory files
         * or not
         */
        bool mUseTrajectoryFile;

        /**
         * Irrlicht window size
         */
        dimension2d<u32> mWindowSize;

        /**
         * Background color of 3D view
         */
        SColor mBgColor;

        /**
         * Color of GUI text
         */
        SColor mGuiColor;

        /**
         * Color of jersey text
         */
        SColor mJerseyTextColor;

        /**
         * Path to GUI text font
         */
        const char* mFontGUIPath;

        /**
         * Path to jersey number font
         */
        const char* mFontJerseyPath;

        /**
         * FPS camera speed scale
         */
        float mFpsScale;

        /**
         * Camera field of view
         */
        float mFieldOfView;

        /**
         * Specifies whether virtual Irrlicht axes have to be displayed or not
         */
        bool mDisplayAxes;

        /**
         * Specifies whether fullscreen is enabled or not for Irrlicht window
         */
        bool mFullScreen;
};

#endif // CAMERASETTINGS_H
