/*
  * 3D Avatars
  * Pierre Walch
  */


#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "libs/tinyxml2.h"
#include "science.h"
#include "engine.h"
#include "settingsparser.h"

SettingsParser::SettingsParser(std::string configurationFilePath)
{
    Engine& e = Engine::getInstance();

    // Initialize XMLDocument and create shortcuts to XMLElements for later use
    if(mDoc.LoadFile(configurationFilePath.c_str()) != XML_NO_ERROR)
        e.throwError(L"Config file cannot be loaded");

    auto avatarsConfig = mDoc.FirstChildElement("avatarsConfig");
    if(avatarsConfig == nullptr)
        e.throwError(L"parsing avatarsConfig tag");

    mGraphicsTag = avatarsConfig->FirstChildElement("graphics");
    if(mGraphicsTag == nullptr)
        e.throwError(L"parsing graphics tag");
    exploreGraphicsTag();

    mInputTag = avatarsConfig->FirstChildElement("input");
    if(mInputTag == nullptr)
        e.throwError(L"parsing input tag");
    exploreInputTag();

    mOutputTag = avatarsConfig->FirstChildElement("output");
    if(mOutputTag == nullptr)
        e.throwError(L"parsing output tag");
    exploreOutputTag();

    mAvatarsTag = avatarsConfig->FirstChildElement("avatars");
    if(mAvatarsTag == nullptr)
        e.throwError(L"parsing avatars tag");
    exploreAvatarsTag();
}

std::tuple<int, int, vector2df> SettingsParser::getPlayerTokens(const std::string &line)
{
    auto strTokens = Science::split(line);
    return std::make_tuple(std::stoi(strTokens.at(0)), std::stoi(strTokens.at(1)),
                           vector2df(std::stof(strTokens.at(2)), std::stof(strTokens.at(3))));
}

std::tuple<int, vector3df> SettingsParser::getBallTokens(const std::string &line)
{
    auto strTokens = Science::split(line);
    return std::make_tuple(std::stoi(strTokens.at(0)),
                           vector3df(std::stof(strTokens.at(1)), std::stof(strTokens.at(2)), std::stof(strTokens.at(3))));
}

std::tuple<int, vector3df, vector3df> SettingsParser::getCameraTokens(const std::string &line)
{
    auto strTokens = Science::split(line);
    return std::make_tuple(std::stoi(strTokens.at(0)),
                           vector3df(std::stof(strTokens.at(1)), std::stof(strTokens.at(2)), std::stof(strTokens.at(3))),
                           vector3df(std::stof(strTokens.at(4)), std::stof(strTokens.at(5)), std::stof(strTokens.at(6))));
}

std::tuple<int, int, int> SettingsParser::getTeamCorrespondance(const std::string &line)
{
    auto strTokens = Science::split(line);
    return std::make_tuple(std::stoi(strTokens.at(0)), std::stoi(strTokens.at(1)), strTokens.at(2).compare(".") == 0 ? -1 : std::stoi(strTokens.at(2)));
}


CourtSettings SettingsParser::retrieveCourtSettings()
{
    Engine& e = Engine::getInstance();

    CourtSettings courtSettings;
    courtSettings.mScenePath = mSceneTag->Attribute("irrscene");
    if(courtSettings.mScenePath == nullptr
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
    if(camSettings.mFontGUIPath == nullptr
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
    if(camSettings.mFontJerseyPath == nullptr
            || mJerseysTag->QueryIntAttribute("colorA", &jTextColorA) != XML_NO_ERROR
            || mJerseysTag->QueryIntAttribute("colorR", &jTextColorR) != XML_NO_ERROR
            || mJerseysTag->QueryIntAttribute("colorG", &jTextColorG) != XML_NO_ERROR
            || mJerseysTag->QueryIntAttribute("colorB", &jTextColorB) != XML_NO_ERROR)
        e.throwError(L"parsing jersey font or color");
    camSettings.mJerseyTextColor = SColor(jTextColorA, jTextColorR, jTextColorG, jTextColorB);


    return camSettings;
}

const char* SettingsParser::retrieveCameraTrajectoryPath()
{
    auto cameraTrackingPath = mCameraTag->Attribute("trajectory");
    if(cameraTrackingPath == nullptr)
        Engine::getInstance().throwError(L"parsing camera trajectory path");

    return cameraTrackingPath;
}

const char *SettingsParser::retrievePlayerTrajectoryPath()
{
    auto playerTrackingPath = mTrackingTag->Attribute("players");
    if(playerTrackingPath == nullptr)
        Engine::getInstance().throwError(L"parsing player trajectories path");

    return playerTrackingPath;
}

const char *SettingsParser::retrieveBallTrajectoryPath()
{
    auto ballTrackingPath = mTrackingTag->Attribute("ball");
    if(ballTrackingPath == nullptr)
        Engine::getInstance().throwError(L"parsing ball tracking path tag");

    return ballTrackingPath;
}

std::map< int, std::pair<int, int> > SettingsParser::retrievePlayerToTeamAndJerseyNumber()
{
    Engine& e = Engine::getInstance();

    // Identify players by using team and jersey number
    const char* jerseyPath = mTrackingTag->Attribute("jerseys");
    if(jerseyPath == nullptr)
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
        int index = 0, team = 0, jerseyNumber = -1;
        std::tie(index, team, jerseyNumber) = SettingsParser::getTeamCorrespondance(line);

        if(jerseyNumber != -1) {
            auto teamAndJerseyNumber = std::pair<int, int>(team, jerseyNumber);
            playerToTeamAndJersey[index] = teamAndJerseyNumber;
        }

    }
    jerseyFile.close();

    return playerToTeamAndJersey;
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


    auto redNormal = mPlayersTag->FirstChildElement("redNormal");
    if(redNormal == nullptr)
        e.throwError(L"parsing redNormal tag");
    auto playerTextureRedNormal = redNormal->Attribute("texture");
    if(playerTextureRedNormal == nullptr)
        e.throwError(L"parsing redNormal texture path");

    auto blueNormal = mPlayersTag->FirstChildElement("blueNormal");
    if(blueNormal == nullptr)
        e.throwError(L"parsing blueNormal tag");
    auto playerTextureBlueNormal = blueNormal->Attribute("texture");
    if(playerTextureBlueNormal == nullptr)
        e.throwError(L"parsing blueNormal texture path");

    auto redSpecial = mPlayersTag->FirstChildElement("redSpecial");
    if(redSpecial == nullptr)
        e.throwError(L"parsing redSpecial tag");
    auto playerTextureRedSpecial = redSpecial->Attribute("texture");
    if(playerTextureRedSpecial == nullptr)
        e.throwError(L"parsing redSpecial texture path");

    auto blueSpecial = mPlayersTag->FirstChildElement("blueSpecial");
    if(blueSpecial == nullptr)
        e.throwError(L"parsing blueSpecial tag");
    auto playerTextureBlueSpecial = blueSpecial->Attribute("texture");
    if(playerTextureBlueSpecial == nullptr)
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

    auto videoNameAtt = mVideoTag->Attribute("name");
    if(videoNameAtt == nullptr)
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
    auto playerBodySettings = retrieveGeneralBodySettings();
    playerBodySettings.mTexturePath = "none";

    playerBodySettings.mModelPath = mPlayersTag->Attribute("model");
    if(playerBodySettings.mModelPath == nullptr
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
    auto ballBodySettings = retrieveGeneralBodySettings();

    ballBodySettings.mModelPath = mBallTag->Attribute("model");
    ballBodySettings.mTexturePath = mBallTag->Attribute("texture");
    if(ballBodySettings.mModelPath == nullptr
            || ballBodySettings.mTexturePath == nullptr
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

    auto stand = mActionsTag->FirstChildElement("stand");
    if(stand == nullptr)
        e.throwError(L"parsing stand tag");
    if(stand->QueryIntAttribute("begin", &playerSettings.mActions[AnimationAction::Stand].mBegin) != XML_NO_ERROR
        || stand->QueryIntAttribute("end", &playerSettings.mActions[AnimationAction::Stand].mEnd) != XML_NO_ERROR)
        e.throwError(L"parsing stand sequence begin or end");

    auto walk = mActionsTag->FirstChildElement("walk");
    if(walk == nullptr)
        e.throwError(L"parsing walk tag");
    if(walk->QueryIntAttribute("begin", &playerSettings.mActions[AnimationAction::Walk].mBegin) != XML_NO_ERROR
        || walk->QueryIntAttribute("end", &playerSettings.mActions[AnimationAction::Walk].mEnd) != XML_NO_ERROR
        || walk->QueryFloatAttribute("threshold", &playerSettings.mActions[AnimationAction::Walk].mThreshold) != XML_NO_ERROR)
        e.throwError(L"parsing walk sequence begin or end or threshold");

    auto run = mActionsTag->FirstChildElement("run");
    if(run == nullptr)
        e.throwError(L"parsing run tag");
    if(run->QueryIntAttribute("begin", &playerSettings.mActions[AnimationAction::Run].mBegin) != XML_NO_ERROR
        || run->QueryIntAttribute("end", &playerSettings.mActions[AnimationAction::Run].mEnd) != XML_NO_ERROR
        || run->QueryFloatAttribute("threshold", &playerSettings.mActions[AnimationAction::Run].mThreshold) != XML_NO_ERROR)
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

std::pair<vector3df, vector3df> SettingsParser::retrieveAffineTransformationPair()
{
    Engine& e = Engine::getInstance();

    auto tfm = mTransformationTag;
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


void SettingsParser::exploreGraphicsTag()
{
    Engine& e = Engine::getInstance();

    mModeTag = mGraphicsTag->FirstChildElement("mode");
    if(mModeTag == nullptr)
        e.throwError(L"parsing mode tag");

    mWindowTag = mGraphicsTag->FirstChildElement("window");
    if(mWindowTag == nullptr)
        e.throwError(L"parsing window tag");

    mGuiTextTag = mGraphicsTag->FirstChildElement("guitext");
    if(mGuiTextTag == nullptr)
        e.throwError(L"parsing guitext tag");
}

void SettingsParser::exploreInputTag()
{
    Engine& e = Engine::getInstance();

    mImageTag = mInputTag->FirstChildElement("image");
    if(mImageTag == nullptr)
        e.throwError(L"parsing image tag");

    mTrackingTag = mInputTag->FirstChildElement("tracking");
    if(mTrackingTag == nullptr)
        e.throwError(L"parsing tracking tag");

    mTeamsTag = mInputTag->FirstChildElement("teams");
    if(mTeamsTag == nullptr)
        e.throwError(L"parsing teams tag");

    mTransformationTag = mInputTag->FirstChildElement("transformation");
    if(mTransformationTag == nullptr)
        e.throwError(L"parsing transformation tag");
}

void SettingsParser::exploreOutputTag()
{
    Engine& e = Engine::getInstance();

    mVideoTag = mOutputTag->FirstChildElement("video");
    if(mVideoTag == nullptr)
        e.throwError(L"parsing video tag");

    mSequenceTag = mOutputTag->FirstChildElement("sequence");
    if(mSequenceTag == nullptr)
        e.throwError(L"parsing sequence tag");

    mCameraTag = mOutputTag->FirstChildElement("camera");
    if(mCameraTag == nullptr)
        e.throwError(L"parsing camera tag");
}

void SettingsParser::exploreAvatarsTag()
{
    Engine& e = Engine::getInstance();

    mSceneTag = mAvatarsTag->FirstChildElement("scene");
    if(mSceneTag == nullptr)
        e.throwError(L"parsing scene tag");

    mActionsTag = mAvatarsTag->FirstChildElement("actions");
    if(mActionsTag == nullptr)
        e.throwError(L"parsing actions tag");

    mPlayersTag = mAvatarsTag->FirstChildElement("players");
    if(mPlayersTag == nullptr)
        e.throwError(L"parsing players tag");

    mJerseysTag = mAvatarsTag->FirstChildElement("jerseys");
    if(mJerseysTag == nullptr)
        e.throwError(L"parsing jerseys tag");

    mBallTag = mAvatarsTag->FirstChildElement("ball");
    if(mBallTag == nullptr)
        e.throwError(L"parsing ball tag");

    mColorCurvesTag = mAvatarsTag->FirstChildElement("colorcurves");
    if(mColorCurvesTag == nullptr)
        e.throwError(L"parsing colorcurves tag");
}
