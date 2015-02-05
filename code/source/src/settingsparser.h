/*
 *  Copyright 2014 Pierre Walch
 *  Website : www.pwalch.net
 *
 *  Avatars is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Avatars is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGSPARSER_H
#define SETTINGSPARSER_H

#include <tuple>
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
    explicit SettingsParser(std::string configurationFilePath);

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
    std::pair<vector3df, vector3df> retrieveAffineTransformationPair();

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
     * Returns association between team and texture
     * @return map from team ID to texture path
     */
    std::map<int, const char*> retrieveTeamToTexture();

    /**
     * Returns association between players and their (team ID / jersey number)
     * @return map from player ID to pair where first member is team ID and second member is jersey number
     */
    std::map<int, std::pair<int, int> > retrievePlayerToTeamAndJerseyNumber();

    /**
     * Returns path to camera trajectory file
     * @return path
     */
    const char* retrieveCameraTrajectoryPath();

    /**
     * Returns path to player trajectory file
     * @return path
     */
    const char* retrievePlayerTrajectoryPath();

    /**
     * Returns path to ball trajectory file
     * @return path
     */
    const char* retrieveBallTrajectoryPath();

    /**
     * Returns player trajectory tokens in this order: frame index -> player index -> position vector.
     * @param line line to parse
     * @return tokens
     */
    static std::tuple<int, int, vector2df > getPlayerTokens(const std::string& line);

    /**
     * Returns ball trajectory tokens in this order: frame index -> position vector.
     * @param line line to parse
     * @return tokens
     */
    static std::tuple<int, vector3df > getBallTokens(const std::string& line);

    /**
     * Returns camera trajectory tokens in this order: frame index -> position vector -> rotation vector.
     * @param line line to parse
     * @return tokens
     */
    static std::tuple<int, vector3df, vector3df > getCameraTokens(const std::string& line);

private:

    static std::tuple<int, int, int > getTeamCorrespondance(const std::string& line);

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
