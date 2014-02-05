/*
  * 3D Avatars
  * Pierre Walch
  */


#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <irrlicht.h>

using namespace irr;
using namespace irr::core;
using namespace irr::video;

/**
 * @brief CameraWindow initialization settings
 *
 * Contains the properties needed to initialize a CameraWindow object.
 */
class CameraSettings
{
public:

    /**
     * Creates an empty object with default values
     */
    CameraSettings() {
        mFollowTrajectoryFile = false;
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
     * Whether the camera is moved according to camera trajectory files, or relying on current camera tab state
     * of Qt window
     */
    bool mFollowTrajectoryFile;

    /**
     * Irrlicht window size in pixels
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
     * FPS camera speed scale, arbitrary scalar
     */
    float mFpsScale;

    /**
     * Camera field of view angle in radians
     */
    float mFieldOfView;

    /**
     * Specifies whether virtual Irrlicht axes have to be displayed
     */
    bool mDisplayAxes;

    /**
     * Specifies whether fullscreen is enabled for Irrlicht window
     */
    bool mFullScreen;
};

#endif // CAMERASETTINGS_H
