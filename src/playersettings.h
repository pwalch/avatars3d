/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef PLAYERSETTINGS_H
#define PLAYERSETTINGS_H

#include "actionsettings.h"

/**
 * Possible animation actions for players
 */
enum AnimationAction{ ANIMATION_STAND = 1,
                      ANIMATION_WALK = 2,
                      ANIMATION_RUN = 3,
                      ANIMATION_JUMP = 4 };

/**
 * @brief Player initializer settings
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
            mSpeedInterval = 0;
            mNbPointsAverager = 0;
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
         * Interval for speed computation (derivative)
         */
        int mSpeedInterval;

        /**
         * Number of points for N-points averager
         */
        int mNbPointsAverager;

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
