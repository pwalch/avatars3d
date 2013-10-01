/**
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <irrlicht.h>
#include <revel.h>
#include <QTime>
#include <locale.h>

#include "mainwindow.h"
#include "../libs/tinyxml2.h"
#include "camerawindow.h"
#include "affinetransformation.h"
#include "court.h"
#include "engine.h"

using namespace tinyxml2;

using namespace irr;
using namespace irr::core;
using namespace irr::video;

Engine& Engine::getInstance()
{
    static Engine instance;
    return instance;
}

Engine::~Engine()
{
    delete transformation;
    delete court;
}

int Engine::start(const QApplication& app,
                  const std::vector<std::string>& args)
{
    if(args.size() != 2) {
        throwError(
        "Error : bad arguments, only a unique XML file is accepted as input");
    }

    std::string cfgPath = args.at(1);
    loadSettings(cfgPath);

    if(CameraWindow::getInstance().getSettings().inConsole) {
        saveVideo(sequenceSettings.startTime, sequenceSettings.endTime);
        return 0;
    } else {
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    }

}

void Engine::loadSettings(const std::string& cfgPath)
{
    setlocale(LC_NUMERIC, "C");

    XMLDocument doc;
    if(doc.LoadFile(cfgPath.c_str()) != XML_NO_ERROR)
        throwError("Config file cannot be loaded");

    XMLElement* avatarsConfig = doc.FirstChildElement("avatarsConfig");
    if(avatarsConfig == NULL)
        throwError("Error parsing avatarsConfig tag");

    CameraSettings camSettings;

    camSettings.useTrajectoryFile = true;

    // Graphics tag
    XMLElement* graphics = avatarsConfig->FirstChildElement("graphics");
    if(graphics == NULL)
        throwError("Error parsing graphics tag");

    XMLElement* mode = graphics->FirstChildElement("mode");
    if(mode == NULL)
        throwError("Error parsing mode tag");
    if(mode->QueryBoolAttribute("console",
            &camSettings.inConsole) != XML_NO_ERROR)
        throwError("Error parsing mode tag");

    XMLElement* window = graphics->FirstChildElement("window");
    if(window == NULL)
        throwError("Error parsing window tag");
    int width, height;
    int bgColorA, bgColorR, bgColorG, bgColorB;
    if(window->QueryIntAttribute("width", &width) != XML_NO_ERROR
            || window->QueryIntAttribute("height", &height) != XML_NO_ERROR
            || window->QueryBoolAttribute("fullscreen",
                    &camSettings.fullScreen) != XML_NO_ERROR
            || window->QueryIntAttribute("bgColorA",
                    &bgColorA) != XML_NO_ERROR
            || window->QueryIntAttribute("bgColorR",
                    &bgColorR) != XML_NO_ERROR
            || window->QueryIntAttribute("bgColorG",
                    &bgColorG) != XML_NO_ERROR
            || window->QueryIntAttribute("bgColorB",
                    &bgColorB) != XML_NO_ERROR)
        throwError("Error parsing window tag");
    camSettings.windowSize = dimension2d<u32>(width, height);
    camSettings.bgColor = SColor(bgColorA, bgColorR, bgColorG, bgColorB);

    XMLElement* guitext = graphics->FirstChildElement("guitext");
    if(guitext == NULL)
        throwError("Error parsing guitext tag");
    camSettings.fontGUIPath = guitext->Attribute("font");
    int guiColorA, guiColorR, guiColorG, guiColorB;
    if(camSettings.fontGUIPath == NULL
            || guitext->QueryIntAttribute("colorA",
                    &guiColorA) != XML_NO_ERROR
            || guitext->QueryIntAttribute("colorR",
                    &guiColorR) != XML_NO_ERROR
            || guitext->QueryIntAttribute("colorG",
                    &guiColorG) != XML_NO_ERROR
            || guitext->QueryIntAttribute("colorB",
                    &guiColorB) != XML_NO_ERROR)
        throwError("Error parsing guitext tag");
    camSettings.guiColor = SColor(guiColorA, guiColorR, guiColorG, guiColorB);

    // Input tag
    XMLElement* input = avatarsConfig->FirstChildElement("input");
    if(input == NULL)
        throwError("Error parsing input tag");



    XMLElement* image = input->FirstChildElement("image");
    if(image == NULL)
        throwError("Error parsing image tag");
    if(image->QueryIntAttribute("frameNumber",
                &sequenceSettings.frameNumber) != XML_NO_ERROR
        || image->QueryIntAttribute("frameRate",
                &sequenceSettings.framerate) != XML_NO_ERROR
        || image->QueryIntAttribute("current",
                &sequenceSettings.currentTime) != XML_NO_ERROR)
        throwError("Error parsing image tag");

    XMLElement* tracking = input->FirstChildElement("tracking");
    if(tracking == NULL)
        throwError("Error parsing tracking tag");
    const char* playerTrackingPath = tracking->Attribute("players");
    const char* ballTrackingPath = tracking->Attribute("ball");
    const char* jerseyPath = tracking->Attribute("jerseys");
    if(playerTrackingPath == NULL
            || ballTrackingPath == NULL
            || jerseyPath == NULL)
        throwError("Error parsing tracking tag");

    XMLElement* teams = input->FirstChildElement("teams");
    if(teams == NULL)
        throwError("Error parsing teams tag");
    int teamRedNormal, teamBlueNormal, teamRedSpecial, teamBlueSpecial;
    if(teams->QueryIntAttribute("redNormal",
                &teamRedNormal) != XML_NO_ERROR
        || teams->QueryIntAttribute("blueNormal",
                                    &teamBlueNormal) != XML_NO_ERROR
        || teams->QueryIntAttribute("redSpecial",
                                    &teamRedSpecial) != XML_NO_ERROR
        || teams->QueryIntAttribute("blueSpecial",
                                    &teamBlueSpecial) != XML_NO_ERROR)
        throwError("Error parsing teams tag");

    XMLElement* tfm = input->FirstChildElement("transformation");
    if(tfm == NULL)
        throwError("Error parsing transformation tag");

    float tfmScaleX, tfmScaleY, tfmScaleZ, tfmOffsetX, tfmOffsetY, tfmOffsetZ;
    if(tfm->QueryBoolAttribute("displayAxes",
            &camSettings.displayAxes) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleX", &tfmScaleX) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleY", &tfmScaleY) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleZ", &tfmScaleZ) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetX", &tfmOffsetX) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetY", &tfmOffsetY) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetZ", &tfmOffsetZ) != XML_NO_ERROR)
        throwError("Error parsing transformation tag");
    const vector3df tfmScale(tfmScaleX, tfmScaleY, tfmScaleZ);
    const vector3df tfmOffset(tfmOffsetX, tfmOffsetY, tfmOffsetZ);
    transformation = new AffineTransformation(tfmScale, tfmOffset);

    // Output settings
    XMLElement* output = avatarsConfig->FirstChildElement("output");
    if(output == NULL)
        throwError("Error parsing output tag");

    XMLElement* video = output->FirstChildElement("video");
    if(video == NULL)
        throwError("Error parsing video tag");
    const char* videoNameAtt = video->Attribute("name");
    if(videoNameAtt == NULL)
        throwError("Error parsing video tag");
    sequenceSettings.name = videoNameAtt;

    XMLElement* sequence = output->FirstChildElement("sequence");
    if(sequence->QueryIntAttribute("start",
            &sequenceSettings.startTime) != XML_NO_ERROR
        || sequence->QueryIntAttribute("end",
            &sequenceSettings.endTime) != XML_NO_ERROR)
        throwError("Error parsing sequence tag");

    XMLElement* camera = output->FirstChildElement("camera");
    if(camera == NULL)
        throwError("Error parsing camera tag");
    const char* cameraFilePath = camera->Attribute("trajectory");
    if(camera->QueryFloatAttribute("fpsScale",
            &camSettings.fpsScale) != XML_NO_ERROR
        || camera->QueryFloatAttribute("fov",
            &camSettings.fieldOfView) != XML_NO_ERROR
        || cameraFilePath == NULL)
        throwError("Error parsing camera tag");

    // Avatars settings
    XMLElement* avatars = avatarsConfig->FirstChildElement("avatars");
    if(avatars == NULL)
        throwError("Error parsing avatars tag");

    XMLElement* scene = avatars->FirstChildElement("scene");
    if(scene == NULL)
        throwError("Error parsing scene tag");
    const char* scenePath = scene->Attribute("irrscene");
    float courtScale;
    if(scenePath == NULL
            || scene->QueryFloatAttribute("scale",
                &courtScale) != XML_NO_ERROR)
        throwError("Error parsing scene tag");


    PlayerSettings playerSettings;

    XMLElement* actions = avatars->FirstChildElement("actions");
    if(actions == NULL)
        throwError("Error parsing actions tag");

    if(actions->QueryIntAttribute("speedInterval",
                &playerSettings.speedInterval) != XML_NO_ERROR
        || actions->QueryIntAttribute("avgNbPoints",
                &playerSettings.nbPointsAverager) != XML_NO_ERROR)
        throwError("Error parsing actions tag");


    XMLElement* stand = actions->FirstChildElement("stand");
    if(stand == NULL)
        throwError("Error parsing stand tag");
    if(stand->QueryIntAttribute("begin",
            &playerSettings.actions[ANIMATION_STAND].begin) != XML_NO_ERROR
        || stand->QueryIntAttribute("end",
            &playerSettings.actions[ANIMATION_STAND].end) != XML_NO_ERROR)
        throwError("Error parsing stand tag");

    XMLElement* walk = actions->FirstChildElement("walk");
    if(walk == NULL)
        throwError("Error parsing walk tag");
    if(walk->QueryIntAttribute("begin",
            &playerSettings.actions[ANIMATION_WALK].begin) != XML_NO_ERROR
        || walk->QueryIntAttribute("end",
             &playerSettings.actions[ANIMATION_WALK].end) != XML_NO_ERROR
        || walk->QueryFloatAttribute("threshold",
             &playerSettings.actions[ANIMATION_WALK].threshold) != XML_NO_ERROR)
        throwError("Error parsing walk tag");

    XMLElement* run = actions->FirstChildElement("run");
    if(run == NULL)
        throwError("Error parsing run tag");
    if(run->QueryIntAttribute("begin",
            &playerSettings.actions[ANIMATION_RUN].begin) != XML_NO_ERROR
        || run->QueryIntAttribute("end",
            &playerSettings.actions[ANIMATION_RUN].end) != XML_NO_ERROR
        || run->QueryFloatAttribute("threshold",
            &playerSettings.actions[ANIMATION_RUN].threshold) != XML_NO_ERROR)
        throwError("Error parsing run tag");

    // Players settings
    XMLElement* players = avatars->FirstChildElement("players");
    if(players == NULL)
        throwError("Error parsing players tag");

    MovingBodySettings playerBodySettings;

    playerBodySettings.modelPath = players->Attribute("model");
    int playerTextureWidth, playerTextureHeight;
    if(playerBodySettings.modelPath == NULL
        || players->QueryBoolAttribute("visible",
                    &playerBodySettings.visible) != XML_NO_ERROR
        || players->QueryFloatAttribute("scale",
                    &playerBodySettings.scale) != XML_NO_ERROR
        || players->QueryIntAttribute("frameRate",
                    &playerSettings.animFramerate) != XML_NO_ERROR
        || players->QueryIntAttribute("textureWidth",
                    &playerTextureWidth) != XML_NO_ERROR
        || players->QueryIntAttribute("textureHeight",
                    &playerTextureHeight) != XML_NO_ERROR)
        throwError("Error parsing players tag");
    playerSettings.textureSize = dimension2d<u32>(playerTextureWidth,
                                                  playerTextureHeight);

    XMLElement* redNormal = players->FirstChildElement("redNormal");
    if(redNormal == NULL)
        throwError("Error parsing redNormal tag");
    const char* playerTextureRedNormal = redNormal->Attribute("texture");
    if(playerTextureRedNormal == NULL)
        throwError("Error parsing redNormal tag");

    XMLElement* blueNormal = players->FirstChildElement("blueNormal");
    if(blueNormal == NULL)
        throwError("Error parsing blueNormal tag");
    const char* playerTextureBlueNormal = blueNormal->Attribute("texture");
    if(playerTextureBlueNormal == NULL)
        throwError("Error parsing blueNormal tag");

    XMLElement* redSpecial = players->FirstChildElement("redSpecial");
    if(redSpecial == NULL)
        throwError("Error parsing redSpecial tag");
    const char* playerTextureRedSpecial = redSpecial->Attribute("texture");
    if(playerTextureRedSpecial == NULL)
        throwError("Error parsing redSpecial tag");

    XMLElement* blueSpecial = players->FirstChildElement("blueSpecial");
    if(blueSpecial == NULL)
        throwError("Error parsing blueSpecial tag");
    const char* playerTextureBlueSpecial = blueSpecial->Attribute("texture");
    if(playerTextureBlueSpecial == NULL)
        throwError("Error parsing blueSpecial tag");

    XMLElement* jerseys = avatars->FirstChildElement("jerseys");
    if(jerseys == NULL)
        throwError("Error parsing jerseys tag");
    camSettings.fontJerseyPath = jerseys->Attribute("font");
    int jerseyNumberLeft, jerseyNumberTop,
        jerseyNumberRight, jerseyNumberBottom,
        jTextColorA, jTextColorR, jTextColorG, jTextColorB;
    if(camSettings.fontJerseyPath == NULL
        || jerseys->QueryIntAttribute("rectLeft",
                    &jerseyNumberLeft) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("rectTop",
                    &jerseyNumberTop) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("rectRight",
                    &jerseyNumberRight) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("rectBottom",
                    &jerseyNumberBottom) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorA",
                    &jTextColorA) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorR",
                    &jTextColorR) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorG",
                    &jTextColorG) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorB",
                    &jTextColorB) != XML_NO_ERROR)
        throwError("Error parsing jerseys tag");
    playerSettings.jerseyTextRect
        = recti(jerseyNumberLeft, jerseyNumberTop,
                jerseyNumberRight, jerseyNumberBottom);
    camSettings.jerseyTextColor
        = SColor(jTextColorA, jTextColorR, jTextColorG, jTextColorB);

    MovingBodySettings ballSettings;
    ballSettings.name = "Ball";
    XMLElement* ball = avatars->FirstChildElement("ball");
    if(ball == NULL)
        throwError("Error parsing ball tag");
    ballSettings.modelPath = ball->Attribute("model");
    ballSettings.texturePath = ball->Attribute("texture");
    if(ballSettings.modelPath == NULL
            || ballSettings.texturePath == NULL
            || ball->QueryFloatAttribute("scale",
                    &ballSettings.scale) != XML_NO_ERROR
            || ball->QueryBoolAttribute("visible",
                    &ballSettings.visible) != XML_NO_ERROR)
        throwError("Error parsing ball tag");


    MoveableSettings moveableSettings;

    XMLElement* colorcurves = avatars->FirstChildElement("colorcurves");
    if(colorcurves == NULL)
        throwError("Error parsing colorcurves tag");
    int trajA, trajR, trajG, trajB;
    if(colorcurves->QueryIntAttribute("nbPoints",
                        &moveableSettings.trajNbPoints) != XML_NO_ERROR
        || colorcurves->QueryIntAttribute("colorA", &trajA) != XML_NO_ERROR
        || colorcurves->QueryIntAttribute("colorR", &trajR) != XML_NO_ERROR
        || colorcurves->QueryIntAttribute("colorG", &trajG) != XML_NO_ERROR
        || colorcurves->QueryIntAttribute("colorB", &trajB) != XML_NO_ERROR)
        throwError("Error parsing colorcurves tag");
    moveableSettings.trajColor = SColor(trajA, trajR, trajG, trajB);

    MoveableSettings moveableCameraSettings = moveableSettings;

    MoveableSettings moveableBallSettings = moveableSettings;
    MoveableSettings moveablePlayerSettings = moveableSettings;

    if(colorcurves->QueryBoolAttribute("playersVisible",
                        &moveablePlayerSettings.trajVisible) != XML_NO_ERROR
            || colorcurves->QueryBoolAttribute("ballVisible",
                        &moveableBallSettings.trajVisible) != XML_NO_ERROR)
        throwError("Error parsing colorcurves tag");

    moveableCameraSettings.trajVisible = false;

    // Camera initialization
    CameraWindow& cam = CameraWindow::getInstance();
    cam.init(camSettings);

    std::ifstream cameraFile;
    cameraFile.open(cameraFilePath);
    if(!cameraFile.is_open()) {
        throwError("Camera trajectory file cannot be open");
    }
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
                        transformation->convertToVirtual(realPosition),
                        rotation);
        }
    }

    // We call init before prepareMove because prepareMove requires
    // the transformation matrix
    cam.prepareMove(moveableCameraSettings);

    // Get player trajectories
    std::map<int, Player*> playerMap;
    std::ifstream playersFile;
    playersFile.open(playerTrackingPath);

    bool beforeMax = true;
    if(!playersFile.is_open()) {
        throwError("Player trajectory file cannot be open");
    }
    while(playersFile.good() && beforeMax) {
        std::string line;
        std::getline(playersFile, line);
        std::vector<float> floatLine = getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int frameIndex = (int) floatLine[0];

            // Stop if not interested in the rest
            if(frameIndex >= sequenceSettings.frameNumber)
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
                            transformation->convertToVirtual(realPosition),
                            vector3df(0, 0, 0));
        }
    }

    playersFile.close();

    // Identify players by using team and jersey number
    std::ifstream jerseyFile;
    jerseyFile.open(jerseyPath);
    if(!jerseyFile.is_open()) {
        throwError("Jersey correspondance file cannot be open");
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


    // Initialize players since they are identified now
    std::map<int, Player*>::iterator i = playerMap.begin();
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
            MovingBodySettings specificPlayerBodySettings = playerBodySettings;

            const int team = p->getTeam();
            if(team == teamRedNormal) {
                specificPlayerBodySettings.texturePath
                        = playerTextureRedNormal;
                specificPlayerBodySettings.name = "Rd";
            } else if(team == teamBlueNormal) {
                specificPlayerBodySettings.texturePath
                        = playerTextureBlueNormal;
                specificPlayerBodySettings.name = "Bl";
            } else if(team == teamRedSpecial) {
                specificPlayerBodySettings.texturePath
                        = playerTextureRedSpecial;
                specificPlayerBodySettings.name = "RdSp";
            } else if(team == teamBlueSpecial) {
                specificPlayerBodySettings.texturePath
                        = playerTextureBlueSpecial;
                specificPlayerBodySettings.name = "BlSp";
            } else {
                std::string errorMsg = "Error : player index ";
                errorMsg += i->first;
                errorMsg += " does not correspond to any team (";
                errorMsg += team;
                errorMsg += ")";
                throwError(errorMsg);
            }
            // Add jersey number to jersey text
            specificPlayerBodySettings.name += p->getJerseyNumber();

            p->init(moveablePlayerSettings,
                    specificPlayerBodySettings,
                    playerSettings);
            ++i;
        }
    }

    // Get ball trajectory and initialize it
    MovingBody* b = new MovingBody();
    std::ifstream ballFile;
    ballFile.open(ballTrackingPath);
    if(!ballFile.is_open()) {
        throwError("Ball trajectory file cannot be open");
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
                       transformation->convertToVirtual(realPosition),
                       vector3df(0, 0, 0));
        }
    }

    b->init(moveableBallSettings, ballSettings);

    // Initialize court
    court = new Court(scenePath, courtScale, playerMap, b);

    // Update scene with the initialized court and the initialized camera
    setTime(sequenceSettings.currentTime);
}

std::vector<float> Engine::getSplittenLine(const std::string& line)
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

void Engine::throwError(const std::string& msg)
{
    std::cerr << "Error: ";
    std::cerr << msg << std::endl;
    exit(1);
}

AffineTransformation* Engine::getTransformation() const
{
    return transformation;
}

void Engine::setTime(int time)
{
    // Updates the scene to new time value
    sequenceSettings.currentTime = time;

    // Update model (MVC)
    court->setTime(time);

    // Updates frame count text and camera position in Irrlicht
    CameraWindow& cam = CameraWindow::getInstance();
    cam.setTime(time);

    cam.updateScene();
}

const SequenceSettings &Engine::getSequenceSettings() const
{
    return sequenceSettings;
}

void Engine::saveVideo(int from, int to, int beforeTime)
{
    if(beforeTime == -1)
        beforeTime = from;

    CameraWindow& cam = CameraWindow::getInstance();

    // Define window size and frames to encode
    dimension2d<u32> windowSize = cam.getSettings().windowSize;
    int width = windowSize.Width;
    int height = windowSize.Height;
    int encodingFrameNumber = to - from;

    // Make sure the API version of Revel we're compiling against matches the
    // header files!  This is terribly important!
    if (REVEL_API_VERSION != Revel_GetApiVersion()) {
        printf("ERROR: Revel version mismatch!\n");
        printf("Headers: version %06x, API version %d\n", REVEL_VERSION,
               REVEL_API_VERSION);
        printf("Library: version %06x, API version %d\n", Revel_GetVersion(),
               Revel_GetApiVersion());
        exit(1);
    }

    // Create an encoder
    int encoderHandle;
    Revel_Error revError = Revel_CreateEncoder(&encoderHandle);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while creating encoder: %d\n", revError);
        exit(1);
    }

    // Set up the encoding parameters.  ALWAYS call Revel_InitializeParams()
    // before filling in your application's parameters, to ensure that all
    // fields especially ones that you may not know about) are initialized
    // to safe values.
    Revel_Params revParams;
    Revel_InitializeParams(&revParams);
    revParams.width = width;
    revParams.height = height;
    revParams.frameRate = sequenceSettings.framerate;
    revParams.quality = 1.0f;
    revParams.codec = REVEL_CD_XVID;

    revParams.hasAudio = false;
    revParams.audioChannels = 2;
    revParams.audioRate = 22050;
    revParams.audioBits = 16;
    revParams.audioSampleFormat = REVEL_ASF_PCM;

    // Initialize encoding
    revError = Revel_EncodeStart(encoderHandle, sequenceSettings.name.c_str(), &revParams);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while starting encoding: %d\n", revError);
        exit(1);
    }

    // Choose video settings
    Revel_VideoFrame frame;
    frame.width = width;
    frame.height = height;
    frame.bytesPerPixel = 4; // RGBA
    frame.pixelFormat = REVEL_PF_RGBA;
    frame.pixels = new int[width*height];
    memset(frame.pixels, 0, width*height*4);
    int* pixels = (int*) frame.pixels;

    // Filling pixel array for each frame
    for(int i = from; i <= to; ++i)
    {
        setTime(i);
        IImage* image = cam.createScreenshot();
        int pixelCounter = 0;
        for(unsigned int x = 0; x < frame.height; ++x) {
            for(unsigned int y = 0; y < frame.width; ++y) {
                SColor pixel = image->getPixel(y, x);

                // Transforming color to RBGA format
                u8 blue = pixel.getBlue(),
                   green = pixel.getGreen(),
                   red = pixel.getRed(),
                   alpha = pixel.getAlpha();
                u32 color = (alpha << 24) | (blue << 16) | (green << 8) | red;
                pixels[pixelCounter] = color;

                ++pixelCounter;
            }
        }

        // Drop Irrlicht resource after its process
        image->drop();

        int frameSize;
        revError = Revel_EncodeFrame(encoderHandle, &frame, &frameSize);
        if (revError != REVEL_ERR_NONE) {
            printf("Revel Error while writing frame: %d\n", revError);
            exit(1);
        }
        // printf("Frame %d of %d: %d bytes\n", i+1, encodingFrameNumbers,
        // frameSize);
    }

    // Choose audio settings
    int totalAudioBytes = 0;
    int audioBufferSize = encodingFrameNumber;
    char* audioBuffer = new char[encodingFrameNumber];
    for(int i = 0; i < encodingFrameNumber; ++i)
        audioBuffer[i] = 0;
    revError
        = Revel_EncodeAudio(encoderHandle,
                            audioBuffer,
                            audioBufferSize,
                            &totalAudioBytes);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while writing audio: %d\n", revError);
        exit(1);
    }
    // printf("Encoded %d bytes of audio\n", totalAudioBytes);

    // Finalize encoding. If this step is skipped, the output movie will
    // be unviewable!
    int totalSize;
    revError = Revel_EncodeEnd(encoderHandle, &totalSize);
    if (revError != REVEL_ERR_NONE){
        printf("Revel Error while ending encoding: %d\n", revError);
        exit(1);
    }

    // printf("%s written: %dx%d, %d frames, %d bytes\n", videoName.c_str(),
    // width, height, encodingFrameNumber, totalSize);
    // Final cleanup
    Revel_DestroyEncoder(encoderHandle);
    if (audioBuffer != NULL)
        delete [] audioBuffer;
    delete [] (int*)frame.pixels;

    //    const float MBSize = 1048576.0;
    //    float size = totalSize / MBSize;
    //    std::cout.setf(std::ios::fixed);
    //    std::cout << videoName << " (" << std::setprecision(2) << size <<
    //    " MB) saved in " << width << "*" << height << " with " <<
    //    encodingFrameNumber << " frames" << std::endl;


    // Restore current frame because video encoding changed it
    setTime(beforeTime);
}

Court *Engine::getCourt() const
{
    return court;
}
