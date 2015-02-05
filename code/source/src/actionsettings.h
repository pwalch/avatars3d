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


#ifndef ACTIONSETTINGS_H
#define ACTIONSETTINGS_H

/**
 * @brief Represents the properties of an animation action
 *
 * Represents the properties of an animation action. The threshold of the slowliest action must be 0
 */
class ActionSettings
{
public:
    /**
     * Creates an empty object with default values
     */
    ActionSettings() {
        mThreshold = 0;
        mBegin = 0;
        mEnd = 0;
    }

    /**
     * Threshold to trigger animation
     */
    float mThreshold;

    /**
     * Begin frame of the animation on player model
     */
    int mBegin;

    /**
     * End frame of the animation on player model
     */
    int mEnd;
};

#endif // ACTIONSETTINGS_H
