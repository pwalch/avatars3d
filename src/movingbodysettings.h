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

        MovingBodySettings() {
            mVisible = false;
            mModelPath = "";
            mTexturePath = "";
            mScale = 0;
        }

        /**
         * Whether body is visible or not in the 3D view
         */
        bool mVisible;
        /**
         * Path to 3D model
         */
        io::path mModelPath;

        /**
         * Path to texture corresponding to 3D model
         */
        io::path mTexturePath;

        /**
         * Scale of 3D model
         */
        float mScale;
};

#endif // MOVINGBODYSETTINGS_H
