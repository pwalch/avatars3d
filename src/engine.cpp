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
    mainWindow.setWindowTitle("Avatars");
    mainWindow.show();

    return app.exec();
}

void Engine::loadSettings()
{    
    tinyxml2::XMLDocument doc;
    const std::string cfgPath("config.xml");
    if(doc.LoadFile(cfgPath.c_str()) != tinyxml2::XML_NO_ERROR)
        parsingError("Config file cannot be loaded");

    tinyxml2::XMLElement* avatarsConfig = doc.FirstChildElement("avatarsConfig");
    if(avatarsConfig == NULL)
        parsingError("Error parsing avatarsConfig tag");

    // Graphics settings
    tinyxml2::XMLElement* graphics = avatarsConfig->FirstChildElement("graphics");
    if(graphics == NULL)
        parsingError("Error parsing graphics tag");

    tinyxml2::XMLElement* window = graphics->FirstChildElement("window");
    if(window == NULL)
        parsingError("Error parsing window tag");
    int width = 0, height = 0;
    const char* guiFontPath = window->Attribute("font");
    if(window->QueryIntAttribute("width", &width) != tinyxml2::XML_NO_ERROR
        || window->QueryIntAttribute("height", &height) != tinyxml2::XML_NO_ERROR
        || guiFontPath == NULL)
        parsingError("Error parsing window tag");
    irr::core::dimension2d<irr::u32> dimensions(width, height);


    // input settings
    tinyxml2::XMLElement* input = avatarsConfig->FirstChildElement("input");
    if(input == NULL)
        parsingError("Error parsing input tag");
    tinyxml2::XMLElement* tracking = input->FirstChildElement("tracking");
    if(tracking == NULL)
        parsingError("Error parsing tracking tag");

    const char* playerTrackingPath = tracking->Attribute("players");
    const char* ballTrackingPath = tracking->Attribute("ball");
    const char* jerseyPath = tracking->Attribute("jerseys");
    if(tracking->QueryIntAttribute("frameNumber", &frameNumber) != tinyxml2::XML_NO_ERROR
        || tracking->QueryIntAttribute("frameRate", &framerate) != tinyxml2::XML_NO_ERROR
        || playerTrackingPath == NULL
        || ballTrackingPath == NULL
        || jerseyPath == NULL)
        parsingError("Error parsing tracking tag");

    // Team settings
    tinyxml2::XMLElement* teams = input->FirstChildElement("teams");
    if(teams == NULL)
        parsingError("Error parsing teams tag");
    int teamRedNormal, teamBlueNormal, teamRedSpecial, teamBlueSpecial;
    if(teams->QueryIntAttribute("redNormal", &teamRedNormal) != tinyxml2::XML_NO_ERROR
        || teams->QueryIntAttribute("blueNormal", &teamBlueNormal) != tinyxml2::XML_NO_ERROR
        || teams->QueryIntAttribute("redSpecial", &teamRedSpecial) != tinyxml2::XML_NO_ERROR
        || teams->QueryIntAttribute("blueSpecial", &teamBlueSpecial) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing teams tag");

    // Player trajectory transformation settings
    tinyxml2::XMLElement* transformationPlayers = input->FirstChildElement("transformationPlayers");
    if(transformationPlayers == NULL)
        parsingError("Error parsing transformationPlayers tag");
    float trajectoryPlayerOffsetX, trajectoryPlayerOffsetY, trajectoryPlayerScaleX, trajectoryPlayerScaleY;
    if(transformationPlayers->QueryFloatAttribute("offsetX", &trajectoryPlayerOffsetX) != tinyxml2::XML_NO_ERROR
        || transformationPlayers->QueryFloatAttribute("offsetY", &trajectoryPlayerOffsetY) != tinyxml2::XML_NO_ERROR
        || transformationPlayers->QueryFloatAttribute("scaleX", &trajectoryPlayerScaleX) != tinyxml2::XML_NO_ERROR
        || transformationPlayers->QueryFloatAttribute("scaleY", &trajectoryPlayerScaleY) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing transformationPlayers tag");

    // Ball trajectory transformation settings
    tinyxml2::XMLElement* transformationBall = input->FirstChildElement("transformationBall");
    if(transformationBall == NULL)
        parsingError("Error parsing transformationBall tag");
    float trajectoryBallOffsetX, trajectoryBallOffsetY, trajectoryBallOffsetZ, trajectoryBallScaleX, trajectoryBallScaleY, trajectoryBallScaleZ;
    if(transformationBall->QueryFloatAttribute("offsetX", &trajectoryBallOffsetX) != tinyxml2::XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("offsetY", &trajectoryBallOffsetY) != tinyxml2::XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("offsetZ", &trajectoryBallOffsetZ) != tinyxml2::XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("scaleX", &trajectoryBallScaleX) != tinyxml2::XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("scaleY", &trajectoryBallScaleY) != tinyxml2::XML_NO_ERROR
        || transformationBall->QueryFloatAttribute("scaleZ", &trajectoryBallScaleZ) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing transformationBall tag");


    // output settings
    tinyxml2::XMLElement* output = avatarsConfig->FirstChildElement("output");
    if(output == NULL)
        parsingError("Error parsing output tag");

    // video settings
    tinyxml2::XMLElement* video = output->FirstChildElement("video");
    if(video == NULL)
        parsingError("Error parsing video tag");

    const char* videoNameAtt = video->Attribute("name");
    if(videoNameAtt == NULL)
        parsingError("Error parsing video tag");
    videoName = videoNameAtt;


    // Camera settings
    tinyxml2::XMLElement* camera = avatarsConfig->FirstChildElement("camera");
    if(camera == NULL)
        parsingError("Error parsing camera tag");

    tinyxml2::XMLElement* options = camera->FirstChildElement("options");
    if(options == NULL)
        parsingError("Error parsing options tag");
    int cameraSpeed;
    if(options->QueryIntAttribute("speed", &cameraSpeed) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing options tag");

    tinyxml2::XMLElement* position = camera->FirstChildElement("position");
    tinyxml2::XMLElement* rotation = camera->FirstChildElement("rotation");
    if(position == NULL || rotation ==  NULL)
        parsingError("Error parsing position tag or rotation tag");

    irr::core::vector3df initialPosition, initialRotation;
    if(position->QueryFloatAttribute("x", &initialPosition.X) != tinyxml2::XML_NO_ERROR
        || position->QueryFloatAttribute("y", &initialPosition.Y) != tinyxml2::XML_NO_ERROR
        || position->QueryFloatAttribute("z", &initialPosition.Z) != tinyxml2::XML_NO_ERROR
        || rotation->QueryFloatAttribute("x", &initialRotation.X) != tinyxml2::XML_NO_ERROR
        || rotation->QueryFloatAttribute("y", &initialRotation.Y) != tinyxml2::XML_NO_ERROR
        || rotation->QueryFloatAttribute("z", &initialRotation.Z) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing position tag or rotation tag");


    // Avatars settings
    tinyxml2::XMLElement* avatars = avatarsConfig->FirstChildElement("avatars");
    if(avatars == NULL)
        parsingError("Error parsing avatars tag");

    // Court settings
    tinyxml2::XMLElement* scene = avatars->FirstChildElement("scene");
    if(scene == NULL)
        parsingError("Error parsing scene tag");
    const char* scenePath = scene->Attribute("irrscene");
    float courtScale;
    if(scenePath == NULL
        || scene->QueryFloatAttribute("scale", &courtScale) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing scene tag");


    tinyxml2::XMLElement* actions = avatars->FirstChildElement("actions");
    if(actions == NULL)
        parsingError("Error parsing actions tag");

    tinyxml2::XMLElement* stand = actions->FirstChildElement("stand");
    if(stand == NULL)
        parsingError("Error parsing stand tag");
    int standBegin, standEnd;
    if(stand->QueryIntAttribute("begin", &standBegin) != tinyxml2::XML_NO_ERROR
        || stand->QueryIntAttribute("end", &standEnd) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing stand tag");

    tinyxml2::XMLElement* walk = actions->FirstChildElement("walk");
    if(walk == NULL)
        parsingError("Error parsing walk tag");
    int walkBegin, walkEnd;
    float walkThreshold;
    if(walk->QueryIntAttribute("begin", &walkBegin) != tinyxml2::XML_NO_ERROR
        || walk->QueryIntAttribute("end", &walkEnd) != tinyxml2::XML_NO_ERROR
        || walk->QueryFloatAttribute("threshold", &walkThreshold))
        parsingError("Error parsing walk tag");

    tinyxml2::XMLElement* run = actions->FirstChildElement("run");
    if(run == NULL)
        parsingError("Error parsing run tag");
    int runBegin, runEnd;
    float runThreshold;
    if(run->QueryIntAttribute("begin", &runBegin) != tinyxml2::XML_NO_ERROR
        || run->QueryIntAttribute("end", &runEnd) != tinyxml2::XML_NO_ERROR
        || run->QueryFloatAttribute("threshold", &runThreshold))
        parsingError("Error parsing run tag");

    std::map<AnimState, irr::core::vector2di> stateDates;
    stateDates[ANIMATION_STAND] = irr::core::vector2di(standBegin, standEnd);
    stateDates[ANIMATION_WALK] = irr::core::vector2di(walkBegin, walkEnd);
    stateDates[ANIMATION_RUN] = irr::core::vector2di(runBegin, runEnd);

    std::map<AnimState, float> stateThreshold;
    stateThreshold[ANIMATION_WALK] = walkThreshold;
    stateThreshold[ANIMATION_RUN] = runThreshold;


    // Players settings
    tinyxml2::XMLElement* players = avatars->FirstChildElement("players");
    if(players == NULL)
        parsingError("Error parsing players tag");

    const char* playerModelPath = players->Attribute("model");
    const char* jerseyFontPath = players->Attribute("font");
    float playerScale;
    int animFramerate, playerTextureSizeX, playerTextureSizeY, jerseyNumberLeft, jerseyNumberTop, jerseyNumberRight, jerseyNumberBottom;
    if(playerModelPath == NULL
        || jerseyFontPath == NULL
        || players->QueryFloatAttribute("scale", &playerScale) != tinyxml2::XML_NO_ERROR
        || players->QueryIntAttribute("framerate", &animFramerate) != tinyxml2::XML_NO_ERROR
        || players->QueryIntAttribute("textureSizeX", &playerTextureSizeX) != tinyxml2::XML_NO_ERROR
        || players->QueryIntAttribute("textureSizeY", &playerTextureSizeY) != tinyxml2::XML_NO_ERROR
        || players->QueryIntAttribute("rectLeft", &jerseyNumberLeft) != tinyxml2::XML_NO_ERROR
        || players->QueryIntAttribute("rectTop", &jerseyNumberTop) != tinyxml2::XML_NO_ERROR
        || players->QueryIntAttribute("rectRight", &jerseyNumberRight) != tinyxml2::XML_NO_ERROR
        || players->QueryIntAttribute("rectBottom", &jerseyNumberBottom) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing players tag");
    const irr::core::dimension2d<irr::u32> playerTextureSize(playerTextureSizeX, playerTextureSizeY);
    const irr::core::recti playerJerseyNumberRect(jerseyNumberLeft, jerseyNumberTop, jerseyNumberRight, jerseyNumberBottom);

    tinyxml2::XMLElement* redNormal = players->FirstChildElement("redNormal");
    if(redNormal == NULL)
        parsingError("Error parsing redNormal tag");
    const char* playerTextureRedNormal = redNormal->Attribute("texture");
    if(playerTextureRedNormal == NULL)
        parsingError("Error parsing redNormal tag");

    tinyxml2::XMLElement* blueNormal = players->FirstChildElement("blueNormal");
    if(blueNormal == NULL)
        parsingError("Error parsing blueNormal tag");
    const char* playerTextureBlueNormal = blueNormal->Attribute("texture");
    if(playerTextureBlueNormal == NULL)
        parsingError("Error parsing blueNormal tag");

    tinyxml2::XMLElement* redSpecial = players->FirstChildElement("redSpecial");
    if(redSpecial == NULL)
        parsingError("Error parsing redSpecial tag");
    const char* playerTextureRedSpecial = redSpecial->Attribute("texture");
    if(playerTextureRedSpecial == NULL)
        parsingError("Error parsing redSpecial tag");

    tinyxml2::XMLElement* blueSpecial = players->FirstChildElement("blueSpecial");
    if(blueSpecial == NULL)
        parsingError("Error parsing blueSpecial tag");
    const char* playerTextureBlueSpecial = blueSpecial->Attribute("texture");
    if(playerTextureBlueSpecial == NULL)
        parsingError("Error parsing blueSpecial tag");

    tinyxml2::XMLElement* ball = avatars->FirstChildElement("ball");
    if(ball == NULL)
        parsingError("Error parsing ball tag");
    const char* ballModel = ball->Attribute("model");
    const char* ballTexture = ball->Attribute("texture");
    float ballScale;
    if(ballModel == NULL
        || ballTexture == NULL
        || ball->QueryFloatAttribute("scale", &ballScale) != tinyxml2::XML_NO_ERROR)
        parsingError("Error parsing ball tag");

    // Camera initialization
    CameraWindow& cam = CameraWindow::getInstance();
    cam.init(dimensions, initialPosition, initialRotation, guiFontPath, jerseyFontPath, cameraSpeed);

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

            irr::core::vector3df position(posX * trajectoryPlayerScaleX + trajectoryPlayerOffsetX, 0, posY * trajectoryPlayerScaleY + trajectoryPlayerOffsetY);
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

        if(jerseyNumber == NOT_A_PLAYER) {
            delete p;
            playerMap.erase(i++);
        }
        else {
            const int team = p->getTeam();
            irr::core::stringw chosenTexture;
            irr::core::stringw chosenName;
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

            chosenName += p->getJerseyNumber();

            p->init(chosenName, playerModelPath, chosenTexture, playerScale, playerTextureSize, playerJerseyNumberRect);
            ++i;
        }
    }

    // Get ball trajectory and initialize
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

            b->mapTime(index, irr::core::vector3df(posX*trajectoryBallScaleX + trajectoryBallOffsetX, posZ*trajectoryBallScaleZ + trajectoryBallOffsetZ, posY*trajectoryBallScaleY + trajectoryBallOffsetY));
        }
    }
    b->init("Ball", ballModel, ballTexture, 1);

    // Initialize court
    court = new Court(scenePath, courtScale, playerMap, b, frameNumber, framerate, animFramerate, stateDates, stateThreshold);

    // Update scene with the initialized court and the initialized camera
    setTime(0);
}

std::vector<int> Engine::getSplittenLine(std::string line)
{
    irr::core::stringc lineIrr(line.c_str());

    // Use Irrlicht to split the line (only with SPACES and not TABS)
    std::vector<irr::core::stringc> splitLine;
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

void Engine::parsingError(std::string msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

void Engine::setTime(const int time)
{
    currentTime = time;
    court->setTime(time);
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

void Engine::saveVideo(const int from, const int to, const int currentFrame)
{
    CameraWindow& cam = CameraWindow::getInstance();

    // Define window size and frames to encode
    irr::core::dimension2di windowSize = cam.getWindowSize();
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
        irr::video::IImage* image = cam.getScreenshot();
        int pixelCounter = 0;
        for(unsigned int x = 0; x < frame.height; ++x) {
            for(unsigned int y = 0; y < frame.width; ++y) {
                irr::video::SColor pixel = image->getPixel(y, x);

                // Transforming color to RBGA format
                irr::u8 blue = pixel.getBlue(), green = pixel.getGreen(), red = pixel.getRed(), alpha = pixel.getAlpha();
                irr::u32 color = (alpha << 24) | (blue << 16) | (green << 8) | red;
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
