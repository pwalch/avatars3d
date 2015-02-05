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

#ifndef BODYSETTINGS_H
#define BODYSETTINGS_H

/**
 * @brief MovingBody initialization settings
 *
 * Contains the necessary properties to initialize a MovingBody object.
 */
class BodySettings
{

public:

    /**
     * Creates an empty object with default values
     */
    BodySettings() {
        mVisible = false;
        mModelPath = "";
        mTexturePath = "";
        mScale = 0;
        mTrajVisible = false;
        mTrajColor = SColor(0, 0, 0, 0);
        mTrajNbPoints = 0;
    }

    /**
     * Whether moving body is visible or not in the 3D view
     */
    bool mVisible;
    /**
     * Path to 3D model
     */
    io::path mModelPath;

    /**
     * Path to texture corresponding to 3D model
     */
    io::path mTexturePath;

    /**
     * Scale of 3D model
     */
    float mScale;

    /**
     * Visibility of trajectory color curve
     */
    bool mTrajVisible;

    /**
     * Color of trajectory color curve
     */
    SColor mTrajColor;

    /**
     * Number of points forming trajectory color curve
     */
    int mTrajNbPoints;
};

#endif // BODYSETTINGS_H
