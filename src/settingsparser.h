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


/**
 * @brief Parser for configuration file
 *
 * Parses the configuration file to extract wanted data. Is then used by AvatarsFactory to create objects.
 * The frame number of SequenceSettings in Engine is used to stop trajectory retrieving when all frames until
 * (frameNumber - 1) have been stored.
 *
 * @see AvatarsFactory
 */
class SettingsParser
{
    public:

        /**
         * Constructs parser and creates XMLElement shortcuts for later use
         * @param configurationFilePath to XML configuration file
         */
        SettingsParser(std::string configurationFilePath);

        /**
         * Returns sequence settings from configuration file
         * @return sequence settings
         */
        SequenceSettings retrieveSequenceSettings();

        /**
         * Returns court settings from configuration file
         * @return court settings
         */
        CourtSettings retrieveCourtSettings();

        /**
         * Returns the affine transformation stored in configuration file
         * @return affine transformation
         */
        std::pair<vector3df, vector3df> retrieveAffineTransformation();

        /**
         * Returns camera trajectory by parsing the trajectory file specified in configuration file
         * @return pair where first member is the position map and the second member is the rotation map
         */
        std::pair< std::map<int, vector3df>, std::map<int, vector3df> > retrieveCameraTrajectory();

        /**
         * Returns camera settings stored in configuration file
         * @return camera settings
         */
        CameraSettings retrieveCameraSettings();

        /**
         * Returns ball body settings stored in configuration file
         * @return ball body settings
         */
        BodySettings retrieveBallBodySettings();

        /**
         * Returns ball trajectory by parsing the trajectory file specified in configuration file
         * @return ball position map
         */
        std::map<int, vector3df > retrieveBallTrajectory();

        /**
         * Returns the player body settings with the given texture
         * @param texturePath path to texture of the player
         * @return completed player body settings
         */
        BodySettings retrievePlayerBodySettings(const char* texturePath);

        /**
         * Returns player settings completed with team ID and jersey number
         * @param team team ID of the player
         * @param jerseyNumber jersey number of the player
         * @return completed player settings
         */
        PlayerSettings retrievePlayerSettings(int team, int jerseyNumber);

        /**
         * Returns player trajectories
         * @return map from players to (map from index to position)
         */
        std::map<int, std::map<int, vector3df> > retrievePlayerTrajectories();

        /**
         * Returns association between team and texture
         * @return map from team ID to texture path
         */
        std::map<int, const char*> retrieveTeamToTexture();

        /**
         * Returns association between players and their (team ID / jersey number)
         * @return map from player ID to pair where first member is team ID and second member is jersey number
         */
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

        // Shortcuts
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
