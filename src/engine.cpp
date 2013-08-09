/**
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <fstream>
#include <sstream>
#include <irrlicht.h>
#include <revel.h>
#include <QTime>
#include "mainwindow.h"
#include "../libs/tinyxml2.h"
#include "camerawindow.h"
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
    delete court;
}

int Engine::start(const QApplication& app)
{
    loadSettings();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

void Engine::loadSettings()
{
    XMLDocument doc;
    const std::string cfgPath("config.xml");
    if(doc.LoadFile(cfgPath.c_str()) != XML_NO_ERROR)
        parsingError("Config file cannot be loaded");

    XMLElement* avatarsConfig = doc.FirstChildElement("avatarsConfig");
    if(avatarsConfig == NULL)
        parsingError("Error parsing avatarsConfig tag");

    // Graphics settings
    XMLElement* graphics = avatarsConfig->FirstChildElement("graphics");
    if(graphics == NULL)
        parsingError("Error parsing graphics tag");

    XMLElement* window = graphics->FirstChildElement("window");
    if(window == NULL)
        parsingError("Error parsing window tag");
    int width = 0, height = 0;
    const char* guiFontPath = window->Attribute("font");
    int bgColorA, bgColorR, bgColorG, bgColorB;
    if(window->QueryIntAttribute("width", &width) != XML_NO_ERROR
        || window->QueryIntAttribute("height", &height) != XML_NO_ERROR
        || guiFontPath == NULL
        || window->QueryIntAttribute("colorA", &bgColorA) != XML_NO_ERROR
        || window->QueryIntAttribute("colorR", &bgColorR) != XML_NO_ERROR
        || window->QueryIntAttribute("colorG", &bgColorG) != XML_NO_ERROR
        || window->QueryIntAttribute("colorB", &bgColorB) != XML_NO_ERROR)
        parsingError("Error parsing window tag");
    const SColor bgColor(bgColorA, bgColorR, bgColorG, bgColorB);
    const dimension2d<u32> dimensions(width, height);


    // input settings
    XMLElement* input = avatarsConfig->FirstChildElement("input");
    if(input == NULL)
        parsingError("Error parsing input tag");
    XMLElement* tracking = input->FirstChildElement("tracking");
    if(tracking == NULL)
        parsingError("Error parsing tracking tag");

    const char* playerTrackingPath = tracking->Attribute("players");
    const char* ballTrackingPath = tracking->Attribute("ball");
    const char* jerseyPath = tracking->Attribute("jerseys");
    if(tracking->QueryIntAttribute("frameNumber", &frameNumber) != XML_NO_ERROR
        || tracking->QueryIntAttribute("frameRate", &framerate) != XML_NO_ERROR
        || playerTrackingPath == NULL
        || ballTrackingPath == NULL
        || jerseyPath == NULL)
        parsingError("Error parsing tracking tag");

    // Team settings
    XMLElement* teams = input->FirstChildElement("teams");
    if(teams == NULL)
        parsingError("Error parsing teams tag");
    int teamRedNormal, teamBlueNormal, teamRedSpecial, teamBlueSpecial;
    if(teams->QueryIntAttribute("redNormal", &teamRedNormal) != XML_NO_ERROR
        || teams->QueryIntAttribute("blueNormal", &teamBlueNormal) != XML_NO_ERROR
        || teams->QueryIntAttribute("redSpecial", &teamRedSpecial) != XML_NO_ERROR
        || teams->QueryIntAttribute("blueSpecial", &teamBlueSpecial) != XML_NO_ERROR)
        parsingError("Error parsing teams tag");

    // Player trajectory transformation settings
    XMLElement* transformationPlayers = input->FirstChildElement("transformationPlayers");
    if(transformationPlayers == NULL)
        parsingError("Error parsing transformationPlayers tag");
    float trajectoryPlayerOffsetX, trajectoryPlayerOffsetY, trajectoryPlayerScaleX, trajectoryPlayerScaleY;
    if(transformationPlayers->QueryFloatAttribute("offsetX", &trajectoryPlayerOffsetX) != XML_NO_ERROR
        || transformationPlayers->QueryFloatAttribute("offsetY", &trajectoryPlayerOffsetY) != XML_NO_ERROR
        || transformationPlayers->QueryFloatAttribute("scaleX", &trajectoryPlayerScaleX) != XML_NO_ERROR
        || transformationPlayers->QueryFloatAttribute("scaleY", &trajectoryPlayerScaleY) != XML_NO_ERROR)
        parsingError("Error parsing transformationPlayers tag");

    // Ball trajectory transformation settings
    XMLElement* transformationBall = input->FirstChildElement("transformationBall");
    if(transformationBall == NULL)
        parsingError("Error parsing transformationBall tag");
    float trajectoryBallOffsetX, trajectoryBallOffsetY, trajectoryBallOffsetZ, trajectoryBallScaleX, trajectoryBallScaleY, trajectoryBallScaleZ;
    if(transformationBall->QueryFloatAttribute("offsetX", &trajectoryBallOffsetX) != XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("offsetY", &trajectoryBallOffsetY) != XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("offsetZ", &trajectoryBallOffsetZ) != XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("scaleX", &trajectoryBallScaleX) != XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("scaleY", &trajectoryBallScaleY) != XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("scaleZ", &trajectoryBallScaleZ) != XML_NO_ERROR)
        parsingError("Error parsing transformationBall tag");


    // Output settings
    XMLElement* output = avatarsConfig->FirstChildElement("output");
    if(output == NULL)
        parsingError("Error parsing output tag");

    // Video settings
    XMLElement* video = output->FirstChildElement("video");
    if(video == NULL)
        parsingError("Error parsing video tag");

    const char* videoNameAtt = video->Attribute("name");
    if(videoNameAtt == NULL)
        parsingError("Error parsing video tag");
    videoName = videoNameAtt;



    // Camera settings
    XMLElement* camera = avatarsConfig->FirstChildElement("camera");
    if(camera == NULL)
        parsingError("Error parsing camera tag");

    XMLElement* options = camera->FirstChildElement("options");
    if(options == NULL)
        parsingError("Error parsing options tag");
    int cameraSpeed;
    if(options->QueryIntAttribute("speed", &cameraSpeed) != XML_NO_ERROR)
        parsingError("Error parsing options tag");

    XMLElement* position = camera->FirstChildElement("position");
    XMLElement* rotation = camera->FirstChildElement("rotation");
    if(position == NULL || rotation ==  NULL)
        parsingError("Error parsing position tag or rotation tag");

    vector3df initialPosition, initialRotation;
    if(position->QueryFloatAttribute("x", &initialPosition.X) != XML_NO_ERROR
        || position->QueryFloatAttribute("y", &initialPosition.Y) != XML_NO_ERROR
        || position->QueryFloatAttribute("z", &initialPosition.Z) != XML_NO_ERROR
        || rotation->QueryFloatAttribute("x", &initialRotation.X) != XML_NO_ERROR
        || rotation->QueryFloatAttribute("y", &initialRotation.Y) != XML_NO_ERROR
        || rotation->QueryFloatAttribute("z", &initialRotation.Z) != XML_NO_ERROR)
        parsingError("Error parsing position tag or rotation tag");



    // Avatars settings
    XMLElement* avatars = avatarsConfig->FirstChildElement("avatars");
    if(avatars == NULL)
        parsingError("Error parsing avatars tag");

    // Court settings
    XMLElement* scene = avatars->FirstChildElement("scene");
    if(scene == NULL)
        parsingError("Error parsing scene tag");
    const char* scenePath = scene->Attribute("irrscene");
    float courtScale;
    if(scenePath == NULL
        || scene->QueryFloatAttribute("scale", &courtScale) != XML_NO_ERROR)
        parsingError("Error parsing scene tag");




    // Actions settings
    XMLElement* actions = avatars->FirstChildElement("actions");
    if(actions == NULL)
        parsingError("Error parsing actions tag");

    XMLElement* stand = actions->FirstChildElement("stand");
    if(stand == NULL)
        parsingError("Error parsing stand tag");
    int standBegin, standEnd;
    if(stand->QueryIntAttribute("begin", &standBegin) != XML_NO_ERROR
        || stand->QueryIntAttribute("end", &standEnd) != XML_NO_ERROR)
        parsingError("Error parsing stand tag");

    XMLElement* walk = actions->FirstChildElement("walk");
    if(walk == NULL)
        parsingError("Error parsing walk tag");
    int walkBegin, walkEnd;
    float walkThreshold;
    if(walk->QueryIntAttribute("begin", &walkBegin) != XML_NO_ERROR
        || walk->QueryIntAttribute("end", &walkEnd) != XML_NO_ERROR
        || walk->QueryFloatAttribute("threshold", &walkThreshold))
        parsingError("Error parsing walk tag");

    XMLElement* run = actions->FirstChildElement("run");
    if(run == NULL)
        parsingError("Error parsing run tag");
    int runBegin, runEnd;
    float runThreshold;
    if(run->QueryIntAttribute("begin", &runBegin) != XML_NO_ERROR
        || run->QueryIntAttribute("end", &runEnd) != XML_NO_ERROR
        || run->QueryFloatAttribute("threshold", &runThreshold))
        parsingError("Error parsing run tag");

    std::map<AnimState, vector2di> stateDates;
    stateDates[ANIMATION_STAND] = vector2di(standBegin, standEnd);
    stateDates[ANIMATION_WALK] = vector2di(walkBegin, walkEnd);
    stateDates[ANIMATION_RUN] = vector2di(runBegin, runEnd);

    std::map<AnimState, float> stateThreshold;
    stateThreshold[ANIMATION_WALK] = walkThreshold;
    stateThreshold[ANIMATION_RUN] = runThreshold;



    // Players settings
    XMLElement* players = avatars->FirstChildElement("players");
    if(players == NULL)
        parsingError("Error parsing players tag");

    const char* playerModelPath = players->Attribute("model");
    float playerScale;
    int animFramerate, playerTextureSizeX, playerTextureSizeY;
    if(playerModelPath == NULL
        || players->QueryFloatAttribute("scale", &playerScale) != XML_NO_ERROR
        || players->QueryIntAttribute("framerate", &animFramerate) != XML_NO_ERROR
        || players->QueryIntAttribute("textureSizeX", &playerTextureSizeX) != XML_NO_ERROR
        || players->QueryIntAttribute("textureSizeY", &playerTextureSizeY) != XML_NO_ERROR)
        parsingError("Error parsing players tag");
    const dimension2d<u32> playerTextureSize(playerTextureSizeX, playerTextureSizeY);

    XMLElement* redNormal = players->FirstChildElement("redNormal");
    if(redNormal == NULL)
        parsingError("Error parsing redNormal tag");
    const char* playerTextureRedNormal = redNormal->Attribute("texture");
    if(playerTextureRedNormal == NULL)
        parsingError("Error parsing redNormal tag");

    XMLElement* blueNormal = players->FirstChildElement("blueNormal");
    if(blueNormal == NULL)
        parsingError("Error parsing blueNormal tag");
    const char* playerTextureBlueNormal = blueNormal->Attribute("texture");
    if(playerTextureBlueNormal == NULL)
        parsingError("Error parsing blueNormal tag");

    XMLElement* redSpecial = players->FirstChildElement("redSpecial");
    if(redSpecial == NULL)
        parsingError("Error parsing redSpecial tag");
    const char* playerTextureRedSpecial = redSpecial->Attribute("texture");
    if(playerTextureRedSpecial == NULL)
        parsingError("Error parsing redSpecial tag");

    XMLElement* blueSpecial = players->FirstChildElement("blueSpecial");
    if(blueSpecial == NULL)
        parsingError("Error parsing blueSpecial tag");
    const char* playerTextureBlueSpecial = blueSpecial->Attribute("texture");
    if(playerTextureBlueSpecial == NULL)
        parsingError("Error parsing blueSpecial tag");

    XMLElement* jerseys = avatars->FirstChildElement("jerseys");
    if(jerseys == NULL)
        parsingError("Error parsing jerseys tag");
    const char* jerseyFontPath = jerseys->Attribute("font");
    int jerseyNumberLeft, jerseyNumberTop, jerseyNumberRight, jerseyNumberBottom, jTextColorA, jTextColorR, jTextColorG, jTextColorB;
    if(jerseyFontPath == NULL
        || jerseys->QueryIntAttribute("rectLeft", &jerseyNumberLeft) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("rectTop", &jerseyNumberTop) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("rectRight", &jerseyNumberRight) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("rectBottom", &jerseyNumberBottom) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorA", &jTextColorA) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorR", &jTextColorR) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorG", &jTextColorG) != XML_NO_ERROR
        || jerseys->QueryIntAttribute("colorB", &jTextColorB) != XML_NO_ERROR)
        parsingError("Error parsing jerseys tag");
    const recti playerJerseyNumberRect(jerseyNumberLeft, jerseyNumberTop, jerseyNumberRight, jerseyNumberBottom);
    const SColor jTextColor(jTextColorA, jTextColorR, jTextColorG, jTextColorB);



    XMLElement* ball = avatars->FirstChildElement("ball");
    if(ball == NULL)
        parsingError("Error parsing ball tag");
    const char* ballModel = ball->Attribute("model");
    const char* ballTexture = ball->Attribute("texture");
    float ballScale;
    if(ballModel == NULL
        || ballTexture == NULL
        || ball->QueryFloatAttribute("scale", &ballScale) != XML_NO_ERROR)
        parsingError("Error parsing ball tag");



    XMLElement* trajectories = avatars->FirstChildElement("trajectories");
    if(trajectories == NULL)
        parsingError("Error parsing trajectories tag");
    int samples, trajA, trajR, trajG, trajB;
    if(trajectories->QueryIntAttribute("samples", &samples) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorA", &trajA) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorR", &trajR) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorG", &trajG) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorB", &trajB) != XML_NO_ERROR)
        parsingError("Error parsing trajectories tag");
    SColor trajColor(trajA, trajR, trajG, trajB);



    // Camera initialization
    CameraWindow& cam = CameraWindow::getInstance();
    cam.init(dimensions, bgColor, jTextColor, initialPosition, initialRotation, guiFontPath, jerseyFontPath, cameraSpeed);

    // Get player trajectories
    std::map<int, Player*> playerMap;
    std::ifstream playersFile;
    playersFile.open(playerTrackingPath);

    if(playersFile.is_open()) {
        while(playersFile.good()) {
            std::string line;
            std::getline(playersFile, line);
            std::vector<int> intLine = getSplittenLine(line);

            int frameIndex = intLine[0];
            int playerIndex = intLine[1];
            float posX = intLine[2];
            float posY = intLine[3];

            // If the player does not exist we create it
            if(playerMap.find(playerIndex) == playerMap.end())
                playerMap[playerIndex] = new Player();

            // We apply the scaling-offset transformation
            const vector3df position(posX * trajectoryPlayerScaleX + trajectoryPlayerOffsetX, 0, posY * trajectoryPlayerScaleY + trajectoryPlayerOffsetY);
            // We fill the map with the current frame
            playerMap[playerIndex]->mapTime(frameIndex, position);
        }
    }
    playersFile.close();

    // Identify players by using team and jersey number
    std::ifstream jerseyFile;
    jerseyFile.open(jerseyPath);
    if(jerseyFile.is_open()) {
        while(jerseyFile.good()) {
            std::string line;
            std::getline(jerseyFile, line);
            std::vector<int> intLine = getSplittenLine(line);

            int index = intLine[0];
            int team = intLine[1];
            int jerseyNumber = intLine[2];
            if(jerseyNumber != -1 && (playerMap.find(index) != playerMap.end())) {
                playerMap[index]->setTeam(team);
                playerMap[index]->setJerseyNumber(jerseyNumber);
            }
        }
    }
    jerseyFile.close();


    // Initialize players since they are identified
    std::map<int, Player*>::iterator i = playerMap.begin();
    while (i != playerMap.end()) {
        Player *p = i->second;
        const int jerseyNumber = p->getJerseyNumber();

        // If the jersey number has not been associated with any team, we remove the person (which is not a player) of the player map
        if(jerseyNumber == NOT_A_PLAYER) {
            delete p;
            playerMap.erase(i++);
        }
        else {
            const int team = p->getTeam();
            stringw chosenTexture;
            stringw chosenName;
            if(team == teamRedNormal) {
                chosenTexture = playerTextureRedNormal;
                chosenName = "Rd";
            } else if(team == teamBlueNormal) {
                chosenTexture = playerTextureBlueNormal;
                chosenName = "Bl";
            } else if(team == teamRedSpecial) {
                chosenTexture = playerTextureRedSpecial;
                chosenName= "RdSp";
            } else if(team == teamBlueSpecial) {
                chosenTexture = playerTextureBlueSpecial;
                chosenName = "BlSp";
            } else {
                std::cerr << "Error : player index " << i->first << " does not correspond to any team (" << team << ")" <<  std::endl;
                exit(1);
            }
            // Add jersey number to jersey text
            chosenName += p->getJerseyNumber();

            p->init(chosenName, playerModelPath, chosenTexture, playerScale, playerTextureSize, playerJerseyNumberRect, trajColor, frameNumber, framerate, animFramerate, stateDates, stateThreshold);
            ++i;
        }
    }

    // Get ball trajectory and initialize it
    Ball* b = new Ball();
    std::ifstream ballFile;
    ballFile.open(ballTrackingPath);
    if(ballFile.is_open()) {
        while(ballFile.good()) {
            std::string line;
            std::getline(ballFile, line);
            std::vector<int> intLine = getSplittenLine(line);

            int index = intLine[0];
            int posX = intLine[1];
            int posY = intLine[2];
            int posZ = intLine[3];

            // We apply the scaling-offset transformation
            const vector3df position(posX*trajectoryBallScaleX + trajectoryBallOffsetX, posZ*trajectoryBallScaleZ + trajectoryBallOffsetZ, posY*trajectoryBallScaleY + trajectoryBallOffsetY);
            b->mapTime(index, position);
        }
    }
    b->init("Ball", ballModel, ballTexture, 1, trajColor, frameNumber);

    // Initialize court
    court = new Court(scenePath, courtScale, playerMap, b);

    // Update scene with the initialized court and the initialized camera
    setTime(0);
}

std::vector<int> Engine::getSplittenLine(const std::string& line)
{
    stringc lineIrr(line.c_str());

    // Use Irrlicht to split the line (only with SPACES and not TABS)
    std::vector<stringc> splitLine;
    lineIrr.split(splitLine, " ");

    // Use C++ to convert tokens into integers
    std::vector<int> splitInt;
    for(unsigned int i = 0; i < splitLine.size(); ++i) {
        int result;
        if(splitLine[i].equals_ignore_case("."))
            result = -1;
        else {
            std::istringstream convert(splitLine[i].c_str());
            convert >> result;
        }
        splitInt.push_back(result);
    }

    return splitInt;
}

void Engine::parsingError(const std::string& msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

void Engine::setTime(int time)
{
    // Updates the scene to new time value
    currentTime = time;
    court->setTime(time);
    // Updates frame count text in Irrlicht window
    CameraWindow& cam = CameraWindow::getInstance();
    cam.setFrameCount(time);

    cam.updateScene();
}

int Engine::getFrameNumber() const
{
    return frameNumber;
}

int Engine::getFramerate() const
{
    return framerate;
}

void Engine::saveVideo(int from, int to, int currentFrame)
{
    CameraWindow& cam = CameraWindow::getInstance();

    // Define window size and frames to encode
    dimension2di windowSize = cam.getWindowSize();
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
    // fields (        void setCourt(Court *value);especially ones that you may not know about) are initialized
    // to safe values.
    Revel_Params revParams;
    Revel_InitializeParams(&revParams);
    revParams.width = width;
    revParams.height = height;
    revParams.frameRate = framerate;
    revParams.quality = 1.0f;
    revParams.codec = REVEL_CD_XVID;

    revParams.hasAudio = false;
    revParams.audioChannels = 2;
    revParams.audioRate = 22050;
    revParams.audioBits = 16;
    revParams.audioSampleFormat = REVEL_ASF_PCM;

    // Initialize encoding
    revError = Revel_EncodeStart(encoderHandle, videoName.c_str(), &revParams);
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
                u8 blue = pixel.getBlue(), green = pixel.getGreen(), red = pixel.getRed(), alpha = pixel.getAlpha();
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
        // printf("Frame %d of %d: %d bytes\n", i+1, encodingFrameNumbers, frameSize);
    }

    // Choose audio settings
    int totalAudioBytes = 0;
    int audioBufferSize = encodingFrameNumber;
    char* audioBuffer = new char[encodingFrameNumber];
    for(int i = 0; i < encodingFrameNumber; ++i)
        audioBuffer[i] = 0;
    revError = Revel_EncodeAudio(encoderHandle, audioBuffer, audioBufferSize, &totalAudioBytes);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while writing audio: %d\n", revError);
        exit(1);
    }
    printf("Encoded %d bytes of audio\n", totalAudioBytes);

    // Finalize encoding. If this step is skipped, the output movie will be unviewable!
    int totalSize;
    revError = Revel_EncodeEnd(encoderHandle, &totalSize);
    if (revError != REVEL_ERR_NONE){
        printf("Revel Error while ending encoding: %d\n", revError);
        exit(1);
    }
    printf("%s written: %dx%d, %d frames, %d bytes\n", videoName.c_str(), width, height,
        encodingFrameNumber, totalSize);

    // Final cleanup
    Revel_DestroyEncoder(encoderHandle);
    if (audioBuffer != NULL)
        delete [] audioBuffer;
    delete [] (int*)frame.pixels;

    // Restore current frame because video encoding changed it
    setTime(currentFrame);
}

Court *Engine::getCourt() const
{
    return court;
}

int Engine::getCurrentTime() const
{
    return currentTime;
}
