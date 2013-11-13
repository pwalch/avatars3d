/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef SETTINGSPARSER_H
#define SETTINGSPARSER_H

#include "libs/tinyxml2.h"
#include "courtsettings.h"
#include "camerasettings.h"
#include "bodysettings.h"

using namespace tinyxml2;
using namespace irr::core;

class SettingsParser
{
    public:

        /**
         * @brief SettingsParser
         * @param path
         */
        SettingsParser(std::string path);

        SequenceSettings retrieveSequenceSettings();

        CourtSettings retrieveCourtSettings();

        /**
         * Returns the affine transformation stored in configuration file
         * @return affine transformation
         */
        std::pair<vector3df, vector3df> retrieveAffineTransformation();

        TrajectoryData* retrieveCameraTrajectory();
        CameraSettings retrieveCameraSettings();

        BodySettings retrieveBallBodySettings();
        std::map<int, vector3df > retrieveBallTrajectory();

        BodySettings retrievePlayerBodySettings(const char* texturePath);
        PlayerSettings retrievePlayerSettings(int team, int jerseyNumber);
        std::map<int, std::map<int, vector3df> > retrievePlayerTrajectories();
        std::map<int, const char*> retrieveTeamToTexture();
        std::map<int, std::pair<int, int> > retrievePlayerToTeamAndJerseyNumber();

    private:

        /**
         * Returns the line splitten using whitespace separator
         * @param line standard string
         * @return list of arguments
         */
        static std::vector<float> getSplittenLine(const std::string& line);

        /**
         * Returns an incomplete MovingBodySettings instance, which must be completed later according to the actual
         * object. Indeed, balls and players do not share the same MovingBodySettings.
         * @return general version of body settings
         */
        BodySettings retrieveGeneralBodySettings();

        // XMLElement shortcut creators
        void exploreGraphicsTag();
        void exploreInputTag();
        void exploreOutputTag();
        void exploreAvatarsTag();

        XMLDocument mDoc;

        XMLElement* mGraphicsTag;
        XMLElement* mModeTag;
        XMLElement* mWindowTag;
        XMLElement* mGuiTextTag;

        XMLElement* mInputTag;
        XMLElement* mImageTag;
        XMLElement* mTrackingTag;
        XMLElement* mTeamsTag;
        XMLElement* mTransformationTag;

        XMLElement* mOutputTag;
        XMLElement* mVideoTag;
        XMLElement* mSequenceTag;
        XMLElement* mCameraTag;

        XMLElement* mAvatarsTag;
        XMLElement* mSceneTag;
        XMLElement* mActionsTag;
        XMLElement* mPlayersTag;
        XMLElement* mJerseysTag;
        XMLElement* mBallTag;
        XMLElement* mColorCurvesTag;
};

#endif // SETTINGSPARSER_H
