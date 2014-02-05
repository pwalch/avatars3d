/*
  * 3D Avatars
  * Pierre Walch
  */


#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "libs/tinyxml2.h"
#include "engine.h"
#include "settingsparser.h"

SettingsParser::SettingsParser(std::string configurationFilePath)
{
    Engine& e = Engine::getInstance();

    // Initialize XMLDocument and create shortcuts to XMLElements for later use
    if(mDoc.LoadFile(configurationFilePath.c_str()) != XML_NO_ERROR)
        e.throwError(L"Config file cannot be loaded");

    XMLElement* avatarsConfig = mDoc.FirstChildElement("avatarsConfig");
    if(avatarsConfig == NULL)
        e.throwError(L"parsing avatarsConfig tag");

    mGraphicsTag = avatarsConfig->FirstChildElement("graphics");
    if(mGraphicsTag == NULL)
        e.throwError(L"parsing graphics tag");
    exploreGraphicsTag();

    mInputTag = avatarsConfig->FirstChildElement("input");
    if(mInputTag == NULL)
        e.throwError(L"parsing input tag");
    exploreInputTag();

    mOutputTag = avatarsConfig->FirstChildElement("output");
    if(mOutputTag == NULL)
        e.throwError(L"parsing output tag");
    exploreOutputTag();

    mAvatarsTag = avatarsConfig->FirstChildElement("avatars");
    if(mAvatarsTag == NULL)
        e.throwError(L"parsing avatars tag");
    exploreAvatarsTag();
}


std::vector<float> SettingsParser::getSplittenLine(const std::string& line)
{
    stringc lineIrr(line.c_str());

    // Use Irrlicht to split the line (only with SPACES and not TABS)
    std::vector<stringc> splitLine;
    lineIrr.split(splitLine, " ");

    // Use standard C++ to convert tokens into integers
    std::vector<float> splitFloat;
    for(unsigned int i = 0; i < splitLine.size(); ++i) {
        float result;
        if(splitLine[i].equals_ignore_case("."))
            result = -1;
        else {
            std::istringstream convert(splitLine[i].c_str());
            convert >> result;
        }
        splitFloat.push_back(result);
    }

    return splitFloat;
}

CourtSettings SettingsParser::retrieveCourtSettings()
{
    Engine& e = Engine::getInstance();

    CourtSettings courtSettings;
    courtSettings.mScenePath = mSceneTag->Attribute("irrscene");
    if(courtSettings.mScenePath == NULL
        || mSceneTag->QueryFloatAttribute("scale", &courtSettings.mScale) != XML_NO_ERROR)
        e.throwError(L"parsing scene path or scale");

    return courtSettings;
}

CameraSettings SettingsParser::retrieveCameraSettings()
{
    Engine& e = Engine::getInstance();
    CameraSettings camSettings;

    // Camera follows trajectory file by default
    camSettings.mFollowTrajectoryFile = true;

    int width, height;
    int bgColorA, bgColorR, bgColorG, bgColorB;
    if(mWindowTag->QueryIntAttribute("width", &width) != XML_NO_ERROR
            || mWindowTag->QueryIntAttribute("height", &height) != XML_NO_ERROR
            || mWindowTag->QueryBoolAttribute("fullscreen", &camSettings.mFullScreen) != XML_NO_ERROR
            || mWindowTag->QueryIntAttribute("bgColorA", &bgColorA) != XML_NO_ERROR
            || mWindowTag->QueryIntAttribute("bgColorR", &bgColorR) != XML_NO_ERROR
            || mWindowTag->QueryIntAttribute("bgColorG", &bgColorG) != XML_NO_ERROR
            || mWindowTag->QueryIntAttribute("bgColorB", &bgColorB) != XML_NO_ERROR)
        e.throwError(L"parsing window size or background color attributes");
    camSettings.mWindowSize = dimension2d<u32>(width, height);
    camSettings.mBgColor = SColor(bgColorA, bgColorR, bgColorG, bgColorB);

    int guiColorA, guiColorR, guiColorG, guiColorB;
    camSettings.mFontGUIPath = mGuiTextTag->Attribute("font");
    if(camSettings.mFontGUIPath == NULL
            || mGuiTextTag->QueryIntAttribute("colorA", &guiColorA) != XML_NO_ERROR
            || mGuiTextTag->QueryIntAttribute("colorR", &guiColorR) != XML_NO_ERROR
            || mGuiTextTag->QueryIntAttribute("colorG", &guiColorG) != XML_NO_ERROR
            || mGuiTextTag->QueryIntAttribute("colorB", &guiColorB) != XML_NO_ERROR)
        e.throwError(L"parsing gui text font or color tag");
    camSettings.mGuiColor = SColor(guiColorA, guiColorR, guiColorG, guiColorB);

    if(mTransformationTag->QueryBoolAttribute("displayAxes", &camSettings.mDisplayAxes) != XML_NO_ERROR)
        e.throwError(L"parsing displayAxes");

    if(mCameraTag->QueryFloatAttribute("fpsScale", &camSettings.mFpsScale) != XML_NO_ERROR
        || mCameraTag->QueryFloatAttribute("fov", &camSettings.mFieldOfView) != XML_NO_ERROR)
        e.throwError(L"parsing fpsScale or fov");

    int jTextColorA, jTextColorR, jTextColorG, jTextColorB;
    camSettings.mFontJerseyPath = mJerseysTag->Attribute("font");
    if(camSettings.mFontJerseyPath == NULL
            || mJerseysTag->QueryIntAttribute("colorA", &jTextColorA) != XML_NO_ERROR
            || mJerseysTag->QueryIntAttribute("colorR", &jTextColorR) != XML_NO_ERROR
            || mJerseysTag->QueryIntAttribute("colorG", &jTextColorG) != XML_NO_ERROR
            || mJerseysTag->QueryIntAttribute("colorB", &jTextColorB) != XML_NO_ERROR)
        e.throwError(L"parsing jersey font or color");
    camSettings.mJerseyTextColor = SColor(jTextColorA, jTextColorR, jTextColorG, jTextColorB);


    return camSettings;
}

std::map< int, std::pair<int, int> > SettingsParser::retrievePlayerToTeamAndJerseyNumber()
{
    Engine& e = Engine::getInstance();

    // Identify players by using team and jersey number
    const char* jerseyPath = mTrackingTag->Attribute("jerseys");
    if(jerseyPath == NULL)
        e.throwError(L"parsing jersey ID file path");

    // Identify players by using team and jersey number
    std::ifstream jerseyFile;
    jerseyFile.open(jerseyPath);
    if(!jerseyFile.is_open()) {
        e.throwError(L"Jersey correspondance file cannot be opened");
    }

    // Establish correspondance between player index and team/jersey
    std::map< int, std::pair<int, int> > playerToTeamAndJersey;
    while(jerseyFile.good()) {
        std::string line;
        std::getline(jerseyFile, line);
        std::vector<float> floatLine = getSplittenLine(line);

        if(floatLine.size() >= 3) {
            int index = (int) floatLine[0];
            int team = (int) floatLine[1];
            int jerseyNumber = (int) floatLine[2];
            if(jerseyNumber != -1) {
                std::pair<int, int> teamAndJerseyNumber = std::pair<int, int>(team, jerseyNumber);
                playerToTeamAndJersey[index] = teamAndJerseyNumber;
            }
        }
    }
    jerseyFile.close();

    return playerToTeamAndJersey;
}

const char* SettingsParser::retrieveCameraTrajectoryPath()
{
    const char* cameraTrackingPath = mCameraTag->Attribute("trajectory");
    if(cameraTrackingPath == NULL)
        Engine::getInstance().throwError(L"parsing camera trajectory path");

    return cameraTrackingPath;
}

const char *SettingsParser::retrievePlayerTrajectoryPath()
{
    const char* playerTrackingPath = mTrackingTag->Attribute("players");
    if(playerTrackingPath == NULL)
        Engine::getInstance().throwError(L"parsing player trajectories path");

    return playerTrackingPath;
}

const char *SettingsParser::retrieveBallTrajectoryPath()
{
    const char* ballTrackingPath = mTrackingTag->Attribute("ball");
    if(ballTrackingPath == NULL)
        Engine::getInstance().throwError(L"parsing ball tracking path tag");

    return ballTrackingPath;
}

std::map<int, const char*> SettingsParser::retrieveTeamToTexture()
{
    Engine& e = Engine::getInstance();

    int teamRedNormal, teamBlueNormal, teamRedSpecial, teamBlueSpecial;
    if(mTeamsTag->QueryIntAttribute("redNormal", &teamRedNormal) != XML_NO_ERROR
            || mTeamsTag->QueryIntAttribute("blueNormal", &teamBlueNormal) != XML_NO_ERROR
            || mTeamsTag->QueryIntAttribute("redSpecial", &teamRedSpecial) != XML_NO_ERROR
            || mTeamsTag->QueryIntAttribute("blueSpecial", &teamBlueSpecial) != XML_NO_ERROR)
        e.throwError(L"parsing team IDs");


    XMLElement* redNormal = mPlayersTag->FirstChildElement("redNormal");
    if(redNormal == NULL)
        e.throwError(L"parsing redNormal tag");
    const char* playerTextureRedNormal = redNormal->Attribute("texture");
    if(playerTextureRedNormal == NULL)
        e.throwError(L"parsing redNormal texture path");

    XMLElement* blueNormal = mPlayersTag->FirstChildElement("blueNormal");
    if(blueNormal == NULL)
        e.throwError(L"parsing blueNormal tag");
    const char* playerTextureBlueNormal = blueNormal->Attribute("texture");
    if(playerTextureBlueNormal == NULL)
        e.throwError(L"parsing blueNormal texture path");

    XMLElement* redSpecial = mPlayersTag->FirstChildElement("redSpecial");
    if(redSpecial == NULL)
        e.throwError(L"parsing redSpecial tag");
    const char* playerTextureRedSpecial = redSpecial->Attribute("texture");
    if(playerTextureRedSpecial == NULL)
        e.throwError(L"parsing redSpecial texture path");

    XMLElement* blueSpecial = mPlayersTag->FirstChildElement("blueSpecial");
    if(blueSpecial == NULL)
        e.throwError(L"parsing blueSpecial tag");
    const char* playerTextureBlueSpecial = blueSpecial->Attribute("texture");
    if(playerTextureBlueSpecial == NULL)
        e.throwError(L"parsing blueSpecial texture path");

    std::map<int, const char*> teamToTexture;
    teamToTexture[teamRedNormal] = playerTextureRedNormal;
    teamToTexture[teamRedSpecial] = playerTextureRedSpecial;
    teamToTexture[teamBlueNormal] = playerTextureBlueNormal;
    teamToTexture[teamBlueSpecial] = playerTextureBlueSpecial;

    return teamToTexture;
}

SequenceSettings SettingsParser::retrieveSequenceSettings()
{
    Engine& e = Engine::getInstance();
    SequenceSettings sequenceSettings;

    int modeNumber = 0;
    if(mModeTag->QueryIntAttribute("type", &modeNumber) != XML_NO_ERROR)
        e.throwError(L"parsing run mode");

    sequenceSettings.mMode = (RUN_MODE) modeNumber;

    if(mImageTag->QueryIntAttribute("frameNumber", &sequenceSettings.mFrameNumber) != XML_NO_ERROR
            || mImageTag->QueryIntAttribute("frameRate", &sequenceSettings.mFramerate) != XML_NO_ERROR
            || mImageTag->QueryIntAttribute("current", &sequenceSettings.mInitialTime) != XML_NO_ERROR)
        e.throwError(L"parsing frameNumber or frameRate or current frame");

    const char* videoNameAtt = mVideoTag->Attribute("name");
    if(videoNameAtt == NULL)
        e.throwError(L"parsing video output path");
    sequenceSettings.mVideoOutputName = videoNameAtt;

    if(mSequenceTag->QueryIntAttribute("start", &sequenceSettings.mStartTime) != XML_NO_ERROR
            || mSequenceTag->QueryIntAttribute("end", &sequenceSettings.mEndTime) != XML_NO_ERROR)
        e.throwError(L"parsing sequence start or end time");

    if(mActionsTag->QueryIntAttribute("speedInterval", &sequenceSettings.mSpeedInterval) != XML_NO_ERROR
        || mActionsTag->QueryIntAttribute("avgNbPoints", &sequenceSettings.mNbPointsAverager) != XML_NO_ERROR)
        e.throwError(L"parsing speed computation interval or number of points for averager");

    return sequenceSettings;
}

BodySettings SettingsParser::retrieveGeneralBodySettings()
{
    Engine& e = Engine::getInstance();
    BodySettings bodySettings;

    bodySettings.mTrajVisible = true;
    int trajA, trajR, trajG, trajB;
    if(mColorCurvesTag->QueryIntAttribute("nbPoints", &bodySettings.mTrajNbPoints) != XML_NO_ERROR
            || mColorCurvesTag->QueryIntAttribute("colorA", &trajA) != XML_NO_ERROR
            || mColorCurvesTag->QueryIntAttribute("colorR", &trajR) != XML_NO_ERROR
            || mColorCurvesTag->QueryIntAttribute("colorG", &trajG) != XML_NO_ERROR
            || mColorCurvesTag->QueryIntAttribute("colorB", &trajB) != XML_NO_ERROR)
        e.throwError(L"parsing color curves color or nbPoints");
    bodySettings.mTrajColor = SColor(trajA, trajR, trajG, trajB);

    return bodySettings;
}

BodySettings SettingsParser::retrievePlayerBodySettings(const char* texturePath)
{
    Engine& e = Engine::getInstance();
    BodySettings playerBodySettings = retrieveGeneralBodySettings();

    playerBodySettings.mModelPath = mPlayersTag->Attribute("model");
    playerBodySettings.mTexturePath = "none";
    if(playerBodySettings.mModelPath == NULL
            || mPlayersTag->QueryBoolAttribute("visible", &playerBodySettings.mVisible) != XML_NO_ERROR
            || mPlayersTag->QueryFloatAttribute("scale", &playerBodySettings.mScale) != XML_NO_ERROR)
        e.throwError(L"parsing player model path or visibility or scale");

    if(mColorCurvesTag->QueryBoolAttribute("playersVisible", &playerBodySettings.mTrajVisible) != XML_NO_ERROR)
        e.throwError(L"parsing player color curve");

    playerBodySettings.mTexturePath = texturePath;

    return playerBodySettings;
}

BodySettings SettingsParser::retrieveBallBodySettings()
{
    Engine& e = Engine::getInstance();
    BodySettings ballBodySettings = retrieveGeneralBodySettings();

    ballBodySettings.mModelPath = mBallTag->Attribute("model");
    ballBodySettings.mTexturePath = mBallTag->Attribute("texture");
    if(ballBodySettings.mModelPath == NULL
            || ballBodySettings.mTexturePath == NULL
            || mBallTag->QueryFloatAttribute("scale", &ballBodySettings.mScale) != XML_NO_ERROR
            || mBallTag->QueryBoolAttribute("visible", &ballBodySettings.mVisible) != XML_NO_ERROR)
        e.throwError(L"parsing ball model path or texture path or visibility or scale");

    if(mColorCurvesTag->QueryBoolAttribute("ballVisible", &ballBodySettings.mTrajVisible) != XML_NO_ERROR)
        e.throwError(L"parsing ball color curve visibility");

    return ballBodySettings;
}


PlayerSettings SettingsParser::retrievePlayerSettings(int team, int jerseyNumber)
{
    Engine& e = Engine::getInstance();
    PlayerSettings playerSettings;

    XMLElement* stand = mActionsTag->FirstChildElement("stand");
    if(stand == NULL)
        e.throwError(L"parsing stand tag");
    if(stand->QueryIntAttribute("begin", &playerSettings.mActions[ANIMATION_STAND].mBegin) != XML_NO_ERROR
        || stand->QueryIntAttribute("end", &playerSettings.mActions[ANIMATION_STAND].mEnd) != XML_NO_ERROR)
        e.throwError(L"parsing stand sequence begin or end");

    XMLElement* walk = mActionsTag->FirstChildElement("walk");
    if(walk == NULL)
        e.throwError(L"parsing walk tag");
    if(walk->QueryIntAttribute("begin", &playerSettings.mActions[ANIMATION_WALK].mBegin) != XML_NO_ERROR
        || walk->QueryIntAttribute("end", &playerSettings.mActions[ANIMATION_WALK].mEnd) != XML_NO_ERROR
        || walk->QueryFloatAttribute("threshold", &playerSettings.mActions[ANIMATION_WALK].mThreshold) != XML_NO_ERROR)
        e.throwError(L"parsing walk sequence begin or end or threshold");

    XMLElement* run = mActionsTag->FirstChildElement("run");
    if(run == NULL)
        e.throwError(L"parsing run tag");
    if(run->QueryIntAttribute("begin", &playerSettings.mActions[ANIMATION_RUN].mBegin) != XML_NO_ERROR
        || run->QueryIntAttribute("end", &playerSettings.mActions[ANIMATION_RUN].mEnd) != XML_NO_ERROR
        || run->QueryFloatAttribute("threshold", &playerSettings.mActions[ANIMATION_RUN].mThreshold) != XML_NO_ERROR)
        e.throwError(L"parsing run sequence begin or end or threshold");

        int playerTextureWidth, playerTextureHeight;
    if(mPlayersTag->QueryIntAttribute("frameRate", &playerSettings.mAnimFramerate) != XML_NO_ERROR
        || mPlayersTag->QueryIntAttribute("textureWidth", &playerTextureWidth) != XML_NO_ERROR
        || mPlayersTag->QueryIntAttribute("textureHeight", &playerTextureHeight) != XML_NO_ERROR)
        e.throwError(L"parsing player animation frameRate or player texture width or height");
    playerSettings.mTextureSize = dimension2d<u32>(playerTextureWidth, playerTextureHeight);

    int jerseyNumberLeft, jerseyNumberTop, jerseyNumberRight, jerseyNumberBottom;
    int jTextColorA, jTextColorR, jTextColorG, jTextColorB;
    if(mJerseysTag->QueryIntAttribute("rectLeft", &jerseyNumberLeft) != XML_NO_ERROR
        || mJerseysTag->QueryIntAttribute("rectTop", &jerseyNumberTop) != XML_NO_ERROR
        || mJerseysTag->QueryIntAttribute("rectRight", &jerseyNumberRight) != XML_NO_ERROR
        || mJerseysTag->QueryIntAttribute("rectBottom", &jerseyNumberBottom) != XML_NO_ERROR
        || mJerseysTag->QueryIntAttribute("colorA", &jTextColorA) != XML_NO_ERROR
        || mJerseysTag->QueryIntAttribute("colorR", &jTextColorR) != XML_NO_ERROR
        || mJerseysTag->QueryIntAttribute("colorG", &jTextColorG) != XML_NO_ERROR
        || mJerseysTag->QueryIntAttribute("colorB", &jTextColorB) != XML_NO_ERROR)
        e.throwError(L"parsing jersey rectangle or color");
    playerSettings.mJerseyTextRect = recti(jerseyNumberLeft, jerseyNumberTop, jerseyNumberRight, jerseyNumberBottom);

    playerSettings.mJerseyNumber = jerseyNumber;
    playerSettings.mTeam = team;

    return playerSettings;
}


void SettingsParser::exploreGraphicsTag()
{
    Engine& e = Engine::getInstance();

    mModeTag = mGraphicsTag->FirstChildElement("mode");
    if(mModeTag == NULL)
        e.throwError(L"parsing mode tag");

    mWindowTag = mGraphicsTag->FirstChildElement("window");
    if(mWindowTag == NULL)
        e.throwError(L"parsing window tag");

    mGuiTextTag = mGraphicsTag->FirstChildElement("guitext");
    if(mGuiTextTag == NULL)
        e.throwError(L"parsing guitext tag");
}

void SettingsParser::exploreInputTag()
{
    Engine& e = Engine::getInstance();

    mImageTag = mInputTag->FirstChildElement("image");
    if(mImageTag == NULL)
        e.throwError(L"parsing image tag");

    mTrackingTag = mInputTag->FirstChildElement("tracking");
    if(mTrackingTag == NULL)
        e.throwError(L"parsing tracking tag");

    mTeamsTag = mInputTag->FirstChildElement("teams");
    if(mTeamsTag == NULL)
        e.throwError(L"parsing teams tag");

    mTransformationTag = mInputTag->FirstChildElement("transformation");
    if(mTransformationTag == NULL)
        e.throwError(L"parsing transformation tag");
}

void SettingsParser::exploreOutputTag()
{
    Engine& e = Engine::getInstance();

    mVideoTag = mOutputTag->FirstChildElement("video");
    if(mVideoTag == NULL)
        e.throwError(L"parsing video tag");

    mSequenceTag = mOutputTag->FirstChildElement("sequence");
    if(mSequenceTag == NULL)
        e.throwError(L"parsing sequence tag");

    mCameraTag = mOutputTag->FirstChildElement("camera");
    if(mCameraTag == NULL)
        e.throwError(L"parsing camera tag");
}

void SettingsParser::exploreAvatarsTag()
{
    Engine& e = Engine::getInstance();

    mSceneTag = mAvatarsTag->FirstChildElement("scene");
    if(mSceneTag == NULL)
        e.throwError(L"parsing scene tag");

    mActionsTag = mAvatarsTag->FirstChildElement("actions");
    if(mActionsTag == NULL)
        e.throwError(L"parsing actions tag");

    mPlayersTag = mAvatarsTag->FirstChildElement("players");
    if(mPlayersTag == NULL)
        e.throwError(L"parsing players tag");

    mJerseysTag = mAvatarsTag->FirstChildElement("jerseys");
    if(mJerseysTag == NULL)
        e.throwError(L"parsing jerseys tag");

    mBallTag = mAvatarsTag->FirstChildElement("ball");
    if(mBallTag == NULL)
        e.throwError(L"parsing ball tag");

    mColorCurvesTag = mAvatarsTag->FirstChildElement("colorcurves");
    if(mColorCurvesTag == NULL)
        e.throwError(L"parsing colorcurves tag");
}

std::pair<vector3df, vector3df> SettingsParser::retrieveAffineTransformation()
{
    Engine& e = Engine::getInstance();

    const XMLElement* tfm = mTransformationTag;
    float tfmScaleX, tfmScaleY, tfmScaleZ, tfmOffsetX, tfmOffsetY, tfmOffsetZ;
    if(tfm->QueryFloatAttribute("scaleX", &tfmScaleX) != XML_NO_ERROR
            || tfm->QueryFloatAttribute("scaleY", &tfmScaleY) != XML_NO_ERROR
            || tfm->QueryFloatAttribute("scaleZ", &tfmScaleZ) != XML_NO_ERROR
            || tfm->QueryFloatAttribute("offsetX", &tfmOffsetX) != XML_NO_ERROR
            || tfm->QueryFloatAttribute("offsetY", &tfmOffsetY) != XML_NO_ERROR
            || tfm->QueryFloatAttribute("offsetZ", &tfmOffsetZ) != XML_NO_ERROR)
        e.throwError(L"parsing transformation scale and offset attributes");

    const vector3df tfmScale(tfmScaleX, tfmScaleY, tfmScaleZ);
    const vector3df tfmOffset(tfmOffsetX, tfmOffsetY, tfmOffsetZ);

    std::pair<vector3df, vector3df> transformation(tfmScale, tfmOffset);

    return transformation;
}
