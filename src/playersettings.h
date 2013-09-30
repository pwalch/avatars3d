#ifndef PLAYERSETTINGS_H
#define PLAYERSETTINGS_H

#include "actioninformation.h"

/**
 * Enum value to distinguish players of the teams from other people of the tracking file
 */
enum { NOT_A_PLAYER = -100 };

/**
 * Possible animation actions for players
 */
enum AnimationAction{ ANIMATION_STAND, ANIMATION_WALK, ANIMATION_RUN, ANIMATION_JUMP };

/**
 * @brief Player initializer settings
 *
 * Contains all the necessary properties to initialize a Player object.
 */
class PlayerSettings
{
public:

    /**
     * Texture dimensions
     */
    dimension2d<u32> textureSize;

    /**
     * Rectangle where to draw jersey number
     */
    recti jerseyTextRect;

    /**
     * Frame rate of animation in player model
     */
    int animFramerate;

    /**
     * Mapping animation actions to their properties
     */
    std::map<AnimationAction, ActionInformation> actions;

    /**
     * Interval for speed computation (derivative)
     */
    int speedInterval;

    /**
     * Number of points for N-points averager
     */
    int nbPointsAverager;

};

#endif // PLAYERSETTINGS_H
