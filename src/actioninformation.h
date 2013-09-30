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

        /**
         * Threshold to trigger animation
         */
        float threshold;

        /**
         * Begin frame of the animation on player model
         */
        int begin;

        /**
         * End frame of the animation on player model
         */
        int end;
};

#endif // ACTIONINFORMATION_H
