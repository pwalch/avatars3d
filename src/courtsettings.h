/*
  * 3D Avatars
  * Pierre Walch
  */


#ifndef COURTSETTINGS_H
#define COURTSETTINGS_H

/**
 * @brief Court initialization settings
 *
 * Contains the properties needed to initialize a CourtSettings object.
 */
class CourtSettings
{
    public:

        /**
         * Creates an empty object with default values
         */
        CourtSettings() {
            mScenePath = "";
            mScale = 0;
        }

        /**
         * Path to Irrlicht scene file ".irr"
         */
        const char* mScenePath;

        /**
         * Scale to apply on the Irrlcht scene
         */
        float mScale;
};

#endif // COURTSETTINGS_H
