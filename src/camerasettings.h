/*
 *  Copyright 2014 Pierre Walch
 *  Website : www.pwalch.net
 *
 *  Avatars is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Avatars is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
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
