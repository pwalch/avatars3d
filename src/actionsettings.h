/*
  * 3D Avatars
  * Pierre Walch
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
