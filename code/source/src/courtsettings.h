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

#ifndef COURTSETTINGS_H
#define COURTSETTINGS_H

/**
 * @brief Court initialization settings
 *
 * Contains the properties needed to initialize a CourtSettings object.
 */
class CourtSettings
{
public:

    /**
     * Creates an empty object with default values
     */
    CourtSettings() {
        mScenePath = "";
        mScale = 0;
    }

    /**
     * Path to Irrlicht scene file ".irr"
     */
    const char* mScenePath;

    /**
     * Scale to apply on the Irrlcht scene
     */
    float mScale;
};

#endif // COURTSETTINGS_H
