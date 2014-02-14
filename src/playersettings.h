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

#ifndef PLAYERSETTINGS_H
#define PLAYERSETTINGS_H

#include "actionsettings.h"

/**
 * Possible animation actions for players
 */
enum class AnimationAction{ Stand, Walk, Run, Jump };

/**
 * @brief Player initialization settings
 *
 * Contains all the necessary properties to initialize a Player object.
 */
class PlayerSettings
{
public:

    /**
     * Creates an empty object with default values
     */
    PlayerSettings() {
        mTextureSize = dimension2d<u32>(0, 0);
        mJerseyTextRect = recti(0, 0, 0, 0);
        mAnimFramerate = 0;
        mTeam = 0;
        mJerseyNumber = 0;
    }

    /**
     * Texture dimensions
     */
    dimension2d<u32> mTextureSize;

    /**
     * Rectangle where to draw jersey number
     */
    recti mJerseyTextRect;

    /**
     * Frame rate of animation in player model
     */
    int mAnimFramerate;

    /**
     * Mapping animation actions to their properties
     */
    std::map<AnimationAction, ActionSettings> mActions;

    /**
     * Team identification number
     */
    int mTeam;

    /**
     * Number drawn on jersey back
     */
    int mJerseyNumber;

};

#endif // PLAYERSETTINGS_H
