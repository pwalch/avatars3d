#ifndef MOVEABLESETTINGS_H
#define MOVEABLESETTINGS_H

/**
 * @brief Moveable initializer settings
 *
 * Contains all the necessary properties to initialize a Moveable object.
 */
class MoveableSettings
{
    public:

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

#endif // MOVEABLESETTINGS_H
