#ifndef MOVINGBODYSETTINGS_H
#define MOVINGBODYSETTINGS_H

/**
 * @brief MovingBody initializer settings
 *
 * Contains all the necessary properties to initialize a MovingBody object.
 */
class MovingBodySettings
{
    public:

        /**
         * Whether body is visible or not in the 3D view
         */
        bool visible;

        /**
         * Body name to display in GUI
         */
        stringw name;

        /**
         * Path to 3D model
         */
        io::path modelPath;

        /**
         * Path to texture corresponding to 3D model
         */
        io::path texturePath;

        /**
         * Scale of 3D model
         */
        float scale;
};

#endif // MOVINGBODYSETTINGS_H
