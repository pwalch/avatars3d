/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef PLAYERSETTINGS_H
#define PLAYERSETTINGS_H

#include "actioninformation.h"

/**
 * Possible animation actions for players
 */
enum AnimationAction{ ANIMATION_STAND,
                      ANIMATION_WALK,
                      ANIMATION_RUN,
                      ANIMATION_JUMP };

/**
 * @brief Player initializer settings
 *
 * Contains all the necessary properties to initialize a Player object.
 */
class PlayerSettings
{
public:

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
    std::map<AnimationAction, ActionInformation> mActions;

    /**
     * Interval for speed computation (derivative)
     */
    int mSpeedInterval;

    /**
     * Number of points for N-points averager
     */
    int mNbPointsAverager;

    /**
     * Team ID
     */
    int mTeam;

    /**
     * Number drawn on jersey back
     */
    int mJerseyNumber;

};

#endif // PLAYERSETTINGS_H
