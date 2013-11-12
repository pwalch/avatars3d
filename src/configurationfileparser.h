/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef CONFIGURATIONFILEPARSER_H
#define CONFIGURATIONFILEPARSER_H

#include "libs/tinyxml2.h"
#include "courtsettings.h"
#include "camerasettings.h"

using namespace tinyxml2;
using namespace irr::core;

class ConfigurationFileParser
{
    public:

    ConfigurationFileParser(std::string path);

    std::vector<float> getSplittenLine(const std::string& line);

    SequenceSettings retrieveSequenceSettings();

    CourtSettings retrieveCourtSettings();

    MoveableSettings retrieveCameraMoveableSettings();
    TrajectoryData* retrieveCameraTrajectory();
    MoveableSettings retrieveGeneralMoveableSettings();
    CameraSettings retrieveCameraSettings();

    MoveableSettings retrieveBallMoveableSettings();
    MovingBodySettings retrieveBallBodySettings();
    std::map<int, vector3df > retrieveBallTrajectory();

    MovingBodySettings retrievePlayerBodySettings(const char* texturePath);
    MoveableSettings retrievePlayerMoveableSettings();
    PlayerSettings retrievePlayerSettings(int jerseyNumber);
    std::map<int, std::map<int, vector3df> > retrievePlayerTrajectories();
    std::map<int, const char*> retrieveTeamToTexture();
    std::map<int, std::pair<int, int> > retrievePlayerToTeamAndJerseyNumber();

    /**
     * Returns the affine transformation stored in configuration file
     * @return affine transformation
     */
    AffineTransformation* createAffineTransformation();

private:

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

#endif // CONFIGURATIONFILEPARSER_H
