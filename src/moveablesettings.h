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
        bool trajVisible;

        /**
         * Color of trajectory color curve
         */
        SColor trajColor;

        /**
         * Number of points forming trajectory color curve
         */
        int trajNbPoints;

};

#endif // MOVEABLESETTINGS_H
