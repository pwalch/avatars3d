/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef COURTSETTINGS_H
#define COURTSETTINGS_H

class CourtSettings
{
    public:
        CourtSettings() {
            mScenePath = "";
            mScale = 0;
        }

        const char* mScenePath;
        float mScale;
};

#endif // COURTSETTINGS_H
