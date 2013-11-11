#include <iostream>
#include <fstream>
#include <sstream>
#include "engine.h"
#include "camerawindow.h"
#include "settingsfactory.h"


SettingsFactory::SettingsFactory(std::string cfgPath)
{
    Engine& e = Engine::getInstance();

    if(doc.LoadFile(cfgPath.c_str()) != XML_NO_ERROR)
        e.throwError("Config file cannot be loaded");

    XMLElement* avatarsConfig = doc.FirstChildElement("avatarsConfig");
    if(avatarsConfig == NULL)
        e.throwError("parsing avatarsConfig tag");

    graphicsTag = avatarsConfig->FirstChildElement("graphics");
    if(graphicsTag == NULL)
        e.throwError("parsing graphics tag");
    exploreGraphicsTag();

    inputTag = avatarsConfig->FirstChildElement("input");
    if(inputTag == NULL)
        e.throwError("parsing input tag");
    exploreInputTag();

    outputTag = avatarsConfig->FirstChildElement("output");
    if(outputTag == NULL)
        e.throwError("parsing output tag");
    exploreOutputTag();

    avatarsTag = avatarsConfig->FirstChildElement("avatars");
    if(avatarsTag == NULL)
        e.throwError("parsing avatars tag");
    exploreAvatarsTag();
}

SettingsFactory::~SettingsFactory()
{

}

std::vector<float> SettingsFactory::getSplittenLine(const std::string& line)
{
    stringc lineIrr(line.c_str());

    // Use Irrlicht to split the line (only with SPACES and not TABS)
    std::vector<stringc> splitLine;
    lineIrr.split(splitLine, " ");

    // Use C++ to convert tokens into integers
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

CameraSettings SettingsFactory::createCameraSettings()
{
    Engine& e = Engine::getInstance();
    CameraSettings camSettings;

    camSettings.mUseTrajectoryFile = true;

    if(modeTag->QueryBoolAttribute("console",
            &camSettings.mInConsole) != XML_NO_ERROR)
        e.throwError("parsing console");

    int width, height;
    int bgColorA, bgColorR, bgColorG, bgColorB;
    if(windowTag->QueryIntAttribute("width", &width) != XML_NO_ERROR
            || windowTag->QueryIntAttribute("height", &height) != XML_NO_ERROR
            || windowTag->QueryBoolAttribute("fullscreen",
                    &camSettings.mFullScreen) != XML_NO_ERROR
            || windowTag->QueryIntAttribute("bgColorA",
                    &bgColorA) != XML_NO_ERROR
            || windowTag->QueryIntAttribute("bgColorR",
                    &bgColorR) != XML_NO_ERROR
            || windowTag->QueryIntAttribute("bgColorG",
                    &bgColorG) != XML_NO_ERROR
            || windowTag->QueryIntAttribute("bgColorB",
                    &bgColorB) != XML_NO_ERROR)
        e.throwError("parsing window size or background color attributes");
    camSettings.mWindowSize = dimension2d<u32>(width, height);
    camSettings.mBgColor = SColor(bgColorA, bgColorR, bgColorG, bgColorB);

    int guiColorA, guiColorR, guiColorG, guiColorB;
    camSettings.mFontGUIPath = guiTextTag->Attribute("font");
    if(camSettings.mFontGUIPath == NULL
            || guiTextTag->QueryIntAttribute("colorA",
                    &guiColorA) != XML_NO_ERROR
            || guiTextTag->QueryIntAttribute("colorR",
                    &guiColorR) != XML_NO_ERROR
            || guiTextTag->QueryIntAttribute("colorG",
                    &guiColorG) != XML_NO_ERROR
            || guiTextTag->QueryIntAttribute("colorB",
                    &guiColorB) != XML_NO_ERROR)
        e.throwError("parsing gui text font or color tag");
    camSettings.mGuiColor = SColor(guiColorA, guiColorR, guiColorG, guiColorB);

    if(transformationTag->QueryBoolAttribute("displayAxes",
            &camSettings.mDisplayAxes) != XML_NO_ERROR)
        e.throwError("parsing displayAxes");

    if(cameraTag->QueryFloatAttribute("fpsScale",
            &camSettings.mFpsScale) != XML_NO_ERROR
        || cameraTag->QueryFloatAttribute("fov",
            &camSettings.mFieldOfView) != XML_NO_ERROR)
        e.throwError("parsing fpsScale or fov");

    int jTextColorA, jTextColorR, jTextColorG, jTextColorB;
    camSettings.mFontJerseyPath = jerseysTag->Attribute("font");
    if(camSettings.mFontJerseyPath == NULL
            || jerseysTag->QueryIntAttribute("colorA",
                        &jTextColorA) != XML_NO_ERROR
            || jerseysTag->QueryIntAttribute("colorR",
                        &jTextColorR) != XML_NO_ERROR
            || jerseysTag->QueryIntAttribute("colorG",
                        &jTextColorG) != XML_NO_ERROR
            || jerseysTag->QueryIntAttribute("colorB",
                        &jTextColorB) != XML_NO_ERROR)
        e.throwError("parsing jersey font or color");
    camSettings.mJerseyTextColor
        = SColor(jTextColorA, jTextColorR, jTextColorG, jTextColorB);


    return camSettings;
}

AffineTransformation* SettingsFactory::createAffineTransformation()
{
    Engine& e = Engine::getInstance();

    const XMLElement* tfm = transformationTag;
    float tfmScaleX, tfmScaleY, tfmScaleZ, tfmOffsetX, tfmOffsetY, tfmOffsetZ;
    if(    tfm->QueryFloatAttribute("scaleX", &tfmScaleX) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleY", &tfmScaleY) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleZ", &tfmScaleZ) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetX", &tfmOffsetX) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetY", &tfmOffsetY) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetZ", &tfmOffsetZ) != XML_NO_ERROR)
        e.throwError("parsing transformation scale and offset attributes");
    const vector3df tfmScale(tfmScaleX, tfmScaleY, tfmScaleZ);
    const vector3df tfmOffset(tfmOffsetX, tfmOffsetY, tfmOffsetZ);
    AffineTransformation* aff = new AffineTransformation(tfmScale, tfmOffset);

    return aff;
}

MoveableSettings SettingsFactory::createCameraMoveableSettings()
{
    MoveableSettings moveableCameraSettings = createGeneralMoveableSettings();
    moveableCameraSettings.mTrajVisible = false;

    return moveableCameraSettings;
}

void SettingsFactory::fillCameraTrajectory()
{
    Engine& e = Engine::getInstance();

    const char* cameraFilePath = cameraTag->Attribute("trajectory");
    if(cameraFilePath == NULL)
        e.throwError("parsing camera trajectory path");

    std::ifstream cameraFile;
    cameraFile.open(cameraFilePath);
    if(!cameraFile.is_open()) {
        e.throwError("Camera trajectory file cannot be opened");
    }

    CameraWindow& cam = CameraWindow::getInstance();
    AffineTransformation* tfm = e.getTransformation();
    while(cameraFile.good()) {
        std::string line;
        std::getline(cameraFile, line);
        std::vector<float> floatLine = getSplittenLine(line);
        if(floatLine.size() >= 7) {
            int index = (int) floatLine[0];
            float posX = floatLine[1];
            float posY = floatLine[2];
            float posZ = floatLine[3];
            float rotX = floatLine[4];
            float rotY = floatLine[5];
            float rotZ = floatLine[6];

            // We apply the scaling-offset transformation
            const vector3df realPosition(posX, posY, posZ);
            const vector3df rotation(rotX, rotY, rotZ);
            cam.mapTime(index,
                        tfm->convertToVirtual(realPosition),
                        rotation);
        }
    }
    cameraFile.close();
}


std::map<int, Player *> SettingsFactory::putTrajectories(
        std::map<int, Player *> playerMap)
{
    Engine& e = Engine::getInstance();

    const char* playerTrackingPath = trackingTag->Attribute("players");
    if(playerTrackingPath == NULL)
        e.throwError("parsing player trajectories path");

    int frameNumber = 0;
    if(imageTag->QueryIntAttribute("frameNumber", &frameNumber) != XML_NO_ERROR)
        e.throwError("parsing frame number");

    // Get player trajectories from text file
    bool beforeMax = true;
    std::ifstream playersFile;
    playersFile.open(playerTrackingPath);
    if(!playersFile.is_open()) {
        e.throwError("player trajectory file cannot be opened");
    }

    while(playersFile.good() && beforeMax) {
        std::string line;
        std::getline(playersFile, line);
        std::vector<float> floatLine = getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int frameIndex = (int) floatLine[0];

            // Stop if not interested in the rest
            if(frameIndex >= frameNumber)
                beforeMax = false;

            int playerIndex = (int) floatLine[1];
            float posX = floatLine[2];
            float posY = floatLine[3];

            // If the player does not exist we create it
            if(playerMap.find(playerIndex) == playerMap.end())
                playerMap[playerIndex] = new Player();

            // We apply the scaling-offset transformation
            const vector3df realPosition(posX, posY, 0);
            // We fill the map with the current frame
            playerMap[playerIndex]->mapTime(
                         frameIndex,
                         e.getTransformation()->convertToVirtual(realPosition),
                         vector3df(0, 0, 0));
        }
    }
    playersFile.close();

    return playerMap;
}

std::map<int, Player *> SettingsFactory::putJerseyNumber(
        std::map<int, Player *> playerMap)
{
    Engine& e = Engine::getInstance();

    // Identify players by using team and jersey number
    const char* jerseyPath = trackingTag->Attribute("jerseys");
    if(jerseyPath == NULL)
        e.throwError("parsing jersey ID file path");

    // Identify players by using team and jersey number
    std::ifstream jerseyFile;
    jerseyFile.open(jerseyPath);
    if(!jerseyFile.is_open()) {
        e.throwError("Jersey correspondance file cannot be opened");
    }
    while(jerseyFile.good()) {
        std::string line;
        std::getline(jerseyFile, line);
        std::vector<float> floatLine = getSplittenLine(line);

        if(floatLine.size() >= 3) {
            int index = (int) floatLine[0];
            int team = (int) floatLine[1];
            int jerseyNumber = (int) floatLine[2];
            if(jerseyNumber != -1
                    && (playerMap.find(index) != playerMap.end())) {
                Player* p = playerMap[index];
                p->setTeam(team);
                p->setJerseyNumber(jerseyNumber);
            }
        }
    }
    jerseyFile.close();

    return playerMap;
}

std::map<int, Player *> SettingsFactory::putModel(
        std::map<int, Player *> playerMap)
{
    Engine& e = Engine::getInstance();
    PlayerSettings playerSettings = createPlayerSettings();

    int teamRedNormal, teamBlueNormal, teamRedSpecial, teamBlueSpecial;
    if(teamsTag->QueryIntAttribute("redNormal", &teamRedNormal) != XML_NO_ERROR
        || teamsTag->QueryIntAttribute("blueNormal",
                                    &teamBlueNormal) != XML_NO_ERROR
        || teamsTag->QueryIntAttribute("redSpecial",
                                    &teamRedSpecial) != XML_NO_ERROR
        || teamsTag->QueryIntAttribute("blueSpecial",
                                    &teamBlueSpecial) != XML_NO_ERROR)
        e.throwError("parsing team IDs");


    XMLElement* redNormal = playersTag->FirstChildElement("redNormal");
    if(redNormal == NULL)
        e.throwError("parsing redNormal tag");
    const char* playerTextureRedNormal = redNormal->Attribute("texture");
    if(playerTextureRedNormal == NULL)
        e.throwError("parsing redNormal texture path");

    XMLElement* blueNormal = playersTag->FirstChildElement("blueNormal");
    if(blueNormal == NULL)
        e.throwError("parsing blueNormal tag");
    const char* playerTextureBlueNormal = blueNormal->Attribute("texture");
    if(playerTextureBlueNormal == NULL)
        e.throwError("parsing blueNormal texture path");

    XMLElement* redSpecial = playersTag->FirstChildElement("redSpecial");
    if(redSpecial == NULL)
        e.throwError("parsing redSpecial tag");
    const char* playerTextureRedSpecial = redSpecial->Attribute("texture");
    if(playerTextureRedSpecial == NULL)
        e.throwError("parsing redSpecial texture path");

    XMLElement* blueSpecial = playersTag->FirstChildElement("blueSpecial");
    if(blueSpecial == NULL)
        e.throwError("parsing blueSpecial tag");
    const char* playerTextureBlueSpecial = blueSpecial->Attribute("texture");
    if(playerTextureBlueSpecial == NULL)
        e.throwError("parsing blueSpecial texture path");


    // Initialize players since they are identified now
    std::map<int, Player*>::iterator i = playerMap.begin();
    MovingBodySettings generalPlayerBodySettings =
            createGeneralPlayerBodySettings();
    MoveableSettings playerMoveableSettings =
            createPlayerMoveableSettings();
    while (i != playerMap.end()) {
        Player *p = i->second;
        const int jerseyNumber = p->getJerseyNumber();

        // If the jersey number has not been associated with any team, we
        // remove the person (which is not a player) of the player map
        if(jerseyNumber == NOT_A_PLAYER) {
            delete p;
            playerMap.erase(i++);
        }
        else {
            MovingBodySettings specificPlayerBodySettings =
                    generalPlayerBodySettings;

            const int team = p->getTeam();
            if(team == teamRedNormal) {
                specificPlayerBodySettings.mTexturePath
                        = playerTextureRedNormal;
            } else if(team == teamBlueNormal) {
                specificPlayerBodySettings.mTexturePath
                        = playerTextureBlueNormal;
            } else if(team == teamRedSpecial) {
                specificPlayerBodySettings.mTexturePath
                        = playerTextureRedSpecial;
            } else if(team == teamBlueSpecial) {
                specificPlayerBodySettings.mTexturePath
                        = playerTextureBlueSpecial;
            } else {
                e.throwError(
                        "Some player index does not correspond to any team");
            }

            p->init(playerMoveableSettings,
                    specificPlayerBodySettings,
                    playerSettings);
            ++i;
        }
    }

    return playerMap;
}

std::map<int, Player *> SettingsFactory::createPlayerMap()
{
    std::map<int, Player*> playerMap;
    playerMap = putTrajectories(playerMap);
    playerMap = putJerseyNumber(playerMap);
    playerMap = putModel(playerMap);

    return playerMap;
}

Court *SettingsFactory::createCourt()
{
    Engine& e = Engine::getInstance();

    const char* scenePath = sceneTag->Attribute("irrscene");
    float courtScale;
    if(scenePath == NULL
        || sceneTag->QueryFloatAttribute("scale", &courtScale) != XML_NO_ERROR)
        e.throwError("parsing scene path or scale");


    std::map<int, Player*> playerMap = createPlayerMap();
    MovingBody* b = createBall();

    Court* court = new Court(scenePath, courtScale, playerMap, b);

    return court;
}

SequenceSettings SettingsFactory::createSequenceSettings()
{
    Engine& e = Engine::getInstance();
    SequenceSettings sequenceSettings;

    if(imageTag->QueryIntAttribute("frameNumber",
                &sequenceSettings.mFrameNumber) != XML_NO_ERROR
        || imageTag->QueryIntAttribute("frameRate",
                &sequenceSettings.mFramerate) != XML_NO_ERROR
        || imageTag->QueryIntAttribute("current",
                &sequenceSettings.mCurrentTime) != XML_NO_ERROR)
        e.throwError("parsing frameNumber or frameRate or current frame");

    const char* videoNameAtt = videoTag->Attribute("name");
    if(videoNameAtt == NULL)
        e.throwError("parsing video output path");
    sequenceSettings.mName = videoNameAtt;

    if(sequenceTag->QueryIntAttribute("start",
            &sequenceSettings.mStartTime) != XML_NO_ERROR
        || sequenceTag->QueryIntAttribute("end",
            &sequenceSettings.mEndTime) != XML_NO_ERROR)
        e.throwError("parsing sequence start or end time");

    return sequenceSettings;
}

void SettingsFactory::constructCamera()
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.init(createCameraSettings());
    fillCameraTrajectory();
    cam.prepareMove(createCameraMoveableSettings());
}

MoveableSettings SettingsFactory::createGeneralMoveableSettings()
{
    Engine& e = Engine::getInstance();
    MoveableSettings moveableSettings;

    moveableSettings.mTrajVisible = true;
    int trajA, trajR, trajG, trajB;
    if(colorCurvesTag->QueryIntAttribute("nbPoints",
                        &moveableSettings.mTrajNbPoints) != XML_NO_ERROR
        || colorCurvesTag->QueryIntAttribute("colorA", &trajA) != XML_NO_ERROR
        || colorCurvesTag->QueryIntAttribute("colorR", &trajR) != XML_NO_ERROR
        || colorCurvesTag->QueryIntAttribute("colorG", &trajG) != XML_NO_ERROR
        || colorCurvesTag->QueryIntAttribute("colorB", &trajB) != XML_NO_ERROR)
        e.throwError("parsing color curves color or nbPoints");
    moveableSettings.mTrajColor = SColor(trajA, trajR, trajG, trajB);

    return moveableSettings;
}

MovingBody *SettingsFactory::createBall()
{
    Engine& e = Engine::getInstance();
    MovingBody* b = new MovingBody();

    const char* ballTrackingPath = trackingTag->Attribute("ball");
    if(ballTrackingPath == NULL)
        e.throwError("parsing ball tracking path tag");
    std::ifstream ballFile;
    ballFile.open(ballTrackingPath);
    if(!ballFile.is_open()) {
        e.throwError("ball trajectory file cannot be opened");
    }

    while(ballFile.good()) {
        std::string line;
        std::getline(ballFile, line);
        std::vector<float> floatLine = getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int index = (int) floatLine[0];
            float posX = floatLine[1];
            float posY = floatLine[2];
            float posZ = floatLine[3];

            // We apply the scaling-offset transformation
            const vector3df realPosition(posX, posY, posZ);
            b->mapTime(index,
                       e.getTransformation()->convertToVirtual(realPosition),
                       vector3df(0, 0, 0));
        }
    }
    ballFile.close();

    MoveableSettings moveableBallSettings = createBallMoveableSettings();
    MovingBodySettings ballSettings = createBallBodySettings();

    b->init(moveableBallSettings, ballSettings);

    return b;
}

MovingBodySettings SettingsFactory::createGeneralPlayerBodySettings()
{
    Engine& e = Engine::getInstance();
    MovingBodySettings playerBodySettings;

    playerBodySettings.mModelPath = playersTag->Attribute("model");
    if(playerBodySettings.mModelPath == NULL
        || playersTag->QueryBoolAttribute("visible",
                    &playerBodySettings.mVisible) != XML_NO_ERROR
        || playersTag->QueryFloatAttribute("scale",
                    &playerBodySettings.mScale) != XML_NO_ERROR)
        e.throwError("parsing player model path or visibility or scale");

    return playerBodySettings;
}

MoveableSettings SettingsFactory::createPlayerMoveableSettings()
{
    Engine& e = Engine::getInstance();
    MoveableSettings moveablePlayerSettings = createGeneralMoveableSettings();

    if(colorCurvesTag->QueryBoolAttribute("playersVisible",
                        &moveablePlayerSettings.mTrajVisible) != XML_NO_ERROR)
        e.throwError("parsing player color curve");

    return moveablePlayerSettings;
}

MoveableSettings SettingsFactory::createBallMoveableSettings()
{
    Engine& e = Engine::getInstance();
    MoveableSettings moveableBallSettings = createGeneralMoveableSettings();
    if(colorCurvesTag->QueryBoolAttribute("ballVisible",
                        &moveableBallSettings.mTrajVisible) != XML_NO_ERROR)
        e.throwError("parsing ball color curve visibility");

    return moveableBallSettings;
}

MovingBodySettings SettingsFactory::createBallBodySettings()
{
    Engine& e = Engine::getInstance();
    MovingBodySettings ballBodySettings;
    ballBodySettings.mModelPath = ballTag->Attribute("model");
    ballBodySettings.mTexturePath = ballTag->Attribute("texture");
    if(ballBodySettings.mModelPath == NULL
            || ballBodySettings.mTexturePath == NULL
            || ballTag->QueryFloatAttribute("scale",
                    &ballBodySettings.mScale) != XML_NO_ERROR
            || ballTag->QueryBoolAttribute("visible",
                    &ballBodySettings.mVisible) != XML_NO_ERROR)
        e.throwError(
            "parsing ball model path or texture path or visibility or scale");

    return ballBodySettings;
}

PlayerSettings SettingsFactory::createPlayerSettings()
{
    Engine& e = Engine::getInstance();
    PlayerSettings playerSettings;

    if(actionsTag->QueryIntAttribute("speedInterval",
                &playerSettings.mSpeedInterval) != XML_NO_ERROR
        || actionsTag->QueryIntAttribute("avgNbPoints",
                &playerSettings.mNbPointsAverager) != XML_NO_ERROR)
        e.throwError(
        "parsing speed computation interval or number of points for averager");

    XMLElement* stand = actionsTag->FirstChildElement("stand");
    if(stand == NULL)
        e.throwError("parsing stand tag");
    if(stand->QueryIntAttribute("begin",
            &playerSettings.mActions[ANIMATION_STAND].mBegin) != XML_NO_ERROR
        || stand->QueryIntAttribute("end",
            &playerSettings.mActions[ANIMATION_STAND].mEnd) != XML_NO_ERROR)
        e.throwError("parsing stand sequence begin or end");

    XMLElement* walk = actionsTag->FirstChildElement("walk");
    if(walk == NULL)
        e.throwError("parsing walk tag");
    if(walk->QueryIntAttribute("begin",
            &playerSettings.mActions[ANIMATION_WALK].mBegin) != XML_NO_ERROR
        || walk->QueryIntAttribute("end",
             &playerSettings.mActions[ANIMATION_WALK].mEnd) != XML_NO_ERROR
        || walk->QueryFloatAttribute("threshold",
          &playerSettings.mActions[ANIMATION_WALK].mThreshold) != XML_NO_ERROR)
        e.throwError("parsing walk sequence begin or end or threshold");

    XMLElement* run = actionsTag->FirstChildElement("run");
    if(run == NULL)
        e.throwError("parsing run tag");
    if(run->QueryIntAttribute("begin",
            &playerSettings.mActions[ANIMATION_RUN].mBegin) != XML_NO_ERROR
        || run->QueryIntAttribute("end",
            &playerSettings.mActions[ANIMATION_RUN].mEnd) != XML_NO_ERROR
        || run->QueryFloatAttribute("threshold",
           &playerSettings.mActions[ANIMATION_RUN].mThreshold) != XML_NO_ERROR)
        e.throwError("parsing run sequence begin or end or threshold");

        int playerTextureWidth, playerTextureHeight;
    if(playersTag->QueryIntAttribute("frameRate",
                    &playerSettings.mAnimFramerate) != XML_NO_ERROR
        || playersTag->QueryIntAttribute("textureWidth",
                    &playerTextureWidth) != XML_NO_ERROR
        || playersTag->QueryIntAttribute("textureHeight",
                    &playerTextureHeight) != XML_NO_ERROR)
        e.throwError(
        "parsing player animation frameRate or player texture width or height");
    playerSettings.mTextureSize = dimension2d<u32>(playerTextureWidth,
                                                  playerTextureHeight);

    int jerseyNumberLeft, jerseyNumberTop,
        jerseyNumberRight, jerseyNumberBottom,
        jTextColorA, jTextColorR, jTextColorG, jTextColorB;
    if(jerseysTag->QueryIntAttribute("rectLeft",
                    &jerseyNumberLeft) != XML_NO_ERROR
        || jerseysTag->QueryIntAttribute("rectTop",
                    &jerseyNumberTop) != XML_NO_ERROR
        || jerseysTag->QueryIntAttribute("rectRight",
                    &jerseyNumberRight) != XML_NO_ERROR
        || jerseysTag->QueryIntAttribute("rectBottom",
                    &jerseyNumberBottom) != XML_NO_ERROR
        || jerseysTag->QueryIntAttribute("colorA",
                    &jTextColorA) != XML_NO_ERROR
        || jerseysTag->QueryIntAttribute("colorR",
                    &jTextColorR) != XML_NO_ERROR
        || jerseysTag->QueryIntAttribute("colorG",
                    &jTextColorG) != XML_NO_ERROR
        || jerseysTag->QueryIntAttribute("colorB",
                    &jTextColorB) != XML_NO_ERROR)
        e.throwError("parsing jersey rectangle or color");
    playerSettings.mJerseyTextRect
        = recti(jerseyNumberLeft, jerseyNumberTop,
                jerseyNumberRight, jerseyNumberBottom);

    return playerSettings;
}


void SettingsFactory::exploreGraphicsTag()
{
    Engine& e = Engine::getInstance();

    modeTag = graphicsTag->FirstChildElement("mode");
    if(modeTag == NULL)
        e.throwError("parsing mode tag");

    windowTag = graphicsTag->FirstChildElement("window");
    if(windowTag == NULL)
        e.throwError("parsing window tag");

    guiTextTag = graphicsTag->FirstChildElement("guitext");
    if(guiTextTag == NULL)
        e.throwError("parsing guitext tag");
}

void SettingsFactory::exploreInputTag()
{
    Engine& e = Engine::getInstance();

    imageTag = inputTag->FirstChildElement("image");
    if(imageTag == NULL)
        e.throwError("parsing image tag");

    trackingTag = inputTag->FirstChildElement("tracking");
    if(trackingTag == NULL)
        e.throwError("parsing tracking tag");

    teamsTag = inputTag->FirstChildElement("teams");
    if(teamsTag == NULL)
        e.throwError("parsing teams tag");

    transformationTag = inputTag->FirstChildElement("transformation");
    if(transformationTag == NULL)
        e.throwError("parsing transformation tag");
}

void SettingsFactory::exploreOutputTag()
{
    Engine& e = Engine::getInstance();

    videoTag = outputTag->FirstChildElement("video");
    if(videoTag == NULL)
        e.throwError("parsing video tag");

    sequenceTag = outputTag->FirstChildElement("sequence");
    if(sequenceTag == NULL)
        e.throwError("parsing sequence tag");

    cameraTag = outputTag->FirstChildElement("camera");
    if(cameraTag == NULL)
        e.throwError("parsing camera tag");
}

void SettingsFactory::exploreAvatarsTag()
{
    Engine& e = Engine::getInstance();

    sceneTag = avatarsTag->FirstChildElement("scene");
    if(sceneTag == NULL)
        e.throwError("parsing scene tag");

    actionsTag = avatarsTag->FirstChildElement("actions");
    if(actionsTag == NULL)
        e.throwError("parsing actions tag");

    playersTag = avatarsTag->FirstChildElement("players");
    if(playersTag == NULL)
        e.throwError("parsing players tag");

    jerseysTag = avatarsTag->FirstChildElement("jerseys");
    if(jerseysTag == NULL)
        e.throwError("parsing jerseys tag");

    ballTag = avatarsTag->FirstChildElement("ball");
    if(ballTag == NULL)
        e.throwError("parsing ball tag");

    colorCurvesTag = avatarsTag->FirstChildElement("colorcurves");
    if(colorCurvesTag == NULL)
        e.throwError("parsing colorcurves tag");
}
