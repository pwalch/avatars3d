/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef BODYSETTINGS_H
#define BODYSETTINGS_H

/**
 * @brief MovingBody initialization settings
 *
 * Contains the necessary properties to initialize a MovingBody object.
 */
class BodySettings
{
    public:

        /**
         * Creates an empty object with default values
         */
        BodySettings() {
            mVisible = false;
            mModelPath = "";
            mTexturePath = "";
            mScale = 0;
            mTrajVisible = false;
            mTrajColor = SColor(0, 0, 0, 0);
            mTrajNbPoints = 0;
        }

        /**
         * Whether moving body is visible or not in the 3D view
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

#endif // BODYSETTINGS_H
