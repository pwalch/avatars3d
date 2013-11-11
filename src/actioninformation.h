#ifndef ACTIONINFORMATION_H
#define ACTIONINFORMATION_H

/**
 * @brief Represents the properties of an AnimationAction
 *
 * Represents the properties of an AnimationAction. The first threshold
 * (stand still) does not need a threshold.
 */
class ActionInformation
{
    public:

        ActionInformation() {
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

#endif // ACTIONINFORMATION_H
