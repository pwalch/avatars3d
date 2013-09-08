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

int Engine::start(const QApplication& app, const std::vector<std::string>& args)
{   
    if(args.size() != 2) {
        std::cerr << "Error : bad arguments, only a unique XML file is accepted as input" << std::endl;
        exit(1);
    }

    std::string cfgPath = args.at(1);
    loadSettings(cfgPath);

    if(inConsole) {
        saveVideo(startTime, endTime);
        return 0;
    } else {
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    }

}

void Engine::loadSettings(const std::string& cfgPath)
{
    XMLDocument doc;
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
    int guiColorA, guiColorR, guiColorG, guiColorB;
    if(window->QueryBoolAttribute("console", &inConsole) != XML_NO_ERROR
        || window->QueryIntAttribute("width", &width) != XML_NO_ERROR
        || window->QueryIntAttribute("height", &height) != XML_NO_ERROR
        || guiFontPath == NULL
        || window->QueryIntAttribute("bgColorA", &bgColorA) != XML_NO_ERROR
        || window->QueryIntAttribute("bgColorR", &bgColorR) != XML_NO_ERROR
        || window->QueryIntAttribute("bgColorG", &bgColorG) != XML_NO_ERROR
        || window->QueryIntAttribute("bgColorB", &bgColorB) != XML_NO_ERROR
        || window->QueryIntAttribute("guiColorA", &guiColorA) != XML_NO_ERROR
        || window->QueryIntAttribute("guiColorR", &guiColorR) != XML_NO_ERROR
        || window->QueryIntAttribute("guiColorG", &guiColorG) != XML_NO_ERROR
        || window->QueryIntAttribute("guiColorB", &guiColorB) != XML_NO_ERROR)
        parsingError("Error parsing window tag");
    const SColor bgColor(bgColorA, bgColorR, bgColorG, bgColorB);
    const SColor guiColor(bgColorA, bgColorR, bgColorG, bgColorB);
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
        || tracking->QueryIntAttribute("current", &currentTime) != XML_NO_ERROR
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

    XMLElement* tfm = input->FirstChildElement("transformation");
    if(tfm == NULL)
        parsingError("Error parsing transformation tag");

    bool dspAxes;
    float tfmScaleX, tfmScaleY, tfmScaleZ, tfmOffsetX, tfmOffsetY, tfmOffsetZ;
    if(tfm->QueryBoolAttribute("displayAxes", &dspAxes) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleX", &tfmScaleX) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleY", &tfmScaleY) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("scaleZ", &tfmScaleZ) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetX", &tfmOffsetX) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetY", &tfmOffsetY) != XML_NO_ERROR
        || tfm->QueryFloatAttribute("offsetZ", &tfmOffsetZ) != XML_NO_ERROR)
        parsingError("Error parsing transformation tag");
    vector3df tfmScale(tfmScaleX, tfmScaleY, tfmScaleZ);
    vector3df tfmOffset(tfmOffsetX, tfmOffsetY, tfmOffsetZ);
    std::vector<vector3df> transformation;
    transformation.push_back(tfmScale);
    transformation.push_back(tfmOffset);

    // Output settings
    XMLElement* output = avatarsConfig->FirstChildElement("output");
    if(output == NULL)
        parsingError("Error parsing output tag");

    // Video settings
    XMLElement* video = output->FirstChildElement("video");
    if(video == NULL)
        parsingError("Error parsing video tag");

    const char* videoNameAtt = video->Attribute("name");
    if(videoNameAtt == NULL
        || video->QueryIntAttribute("start", &startTime) != XML_NO_ERROR
        || video->QueryIntAttribute("end", &endTime) != XML_NO_ERROR)
        parsingError("Error parsing video tag");
    videoName = videoNameAtt;

    // Camera settings
    XMLElement* camera = avatarsConfig->FirstChildElement("camera");
    if(camera == NULL)
        parsingError("Error parsing camera tag");
    float fpsScale;
    float fieldOfView;
    const char* cameraFilePath = camera->Attribute("file");
    if(camera->QueryFloatAttribute("speed", &fpsScale) != XML_NO_ERROR
            || camera->QueryFloatAttribute("fov", &fieldOfView)
            || cameraFilePath == NULL)
        parsingError("Error parsing camera tag");

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
    int trajA, trajR, trajG, trajB;
    bool trajVisible;
    if(trajectories->QueryBoolAttribute("visible", &trajVisible) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorA", &trajA) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorR", &trajR) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorG", &trajG) != XML_NO_ERROR
        || trajectories->QueryIntAttribute("colorB", &trajB) != XML_NO_ERROR)
        parsingError("Error parsing trajectories tag");
    SColor trajColor(trajA, trajR, trajG, trajB);


    // Camera initialization
    CameraWindow& cam = CameraWindow::getInstance();
    cam.init(inConsole, dimensions, bgColor, guiColor, jTextColor, guiFontPath, jerseyFontPath, fpsScale, fieldOfView, transformation, dspAxes);

    std::ifstream cameraFile;
    cameraFile.open(cameraFilePath);
    if(!cameraFile.is_open()) {
        parsingError("Camera trajectory file cannot be open");
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
            cam.mapTime(index, cam.convertToVirtual(realPosition), rotation);
        }
    }

    // We call init before prepareMove because prepareMove requires the transformation matrix
    cam.prepareMove(trajVisible, trajColor, frameNumber, framerate);


    // Get player trajectories
    std::map<int, Player*> playerMap;
    std::ifstream playersFile;
    playersFile.open(playerTrackingPath);

    bool beforeMax = true;
    if(!playersFile.is_open()) {
        parsingError("Player trajectory file cannot be open");
    }
    while(playersFile.good() && beforeMax) {
        std::string line;
        std::getline(playersFile, line);
        std::vector<float> floatLine = getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int frameIndex = (int) floatLine[0];
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
            playerMap[playerIndex]->mapTime(frameIndex, cam.convertToVirtual(realPosition), vector3df(0, 0, 0));
        }
    }

    playersFile.close();

    // Identify players by using team and jersey number
    std::ifstream jerseyFile;
    jerseyFile.open(jerseyPath);
    if(!jerseyFile.is_open()) {
        parsingError("Jersey correspondance file cannot be open");
    }
    while(jerseyFile.good()) {
        std::string line;
        std::getline(jerseyFile, line);
        std::vector<float> floatLine = getSplittenLine(line);

        if(floatLine.size() >= 3) {
            int index = (int) floatLine[0];
            int team = (int) floatLine[1];
            int jerseyNumber = (int) floatLine[2];
            if(jerseyNumber != -1 && (playerMap.find(index) != playerMap.end())) {
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

            p->init(trajVisible, trajColor, frameNumber, framerate, chosenName, playerModelPath, chosenTexture, playerScale, playerTextureSize, playerJerseyNumberRect, animFramerate, stateDates, stateThreshold);
            ++i;
        }
    }

    // Get ball trajectory and initialize it
    MovingBody* b = new MovingBody();
    std::ifstream ballFile;
    ballFile.open(ballTrackingPath);
    if(!ballFile.is_open()) {
        parsingError("Ball trajectory file cannot be open");
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
            b->mapTime(index, cam.convertToVirtual(realPosition), vector3df(0, 0, 0));
        }
    }

    b->init(trajVisible, trajColor, frameNumber, framerate, "Ball", ballModel, ballTexture, ballScale);

    // Initialize court
    court = new Court(scenePath, courtScale, playerMap, b);

    // Update scene with the initialized court and the initialized camera
    setTime(currentTime);
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
    cam.setTime(time);

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

void Engine::saveVideo(int from, int to, int beforeTime)
{
    if(beforeTime == -1)
        beforeTime = from;

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
    // fields especially ones that you may not know about) are initialized
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
    // printf("Encoded %d bytes of audio\n", totalAudioBytes);

    // Finalize encoding. If this step is skipped, the output movie will be unviewable!
    int totalSize;
    revError = Revel_EncodeEnd(encoderHandle, &totalSize);
    if (revError != REVEL_ERR_NONE){
        printf("Revel Error while ending encoding: %d\n", revError);
        exit(1);
    }

    // printf("%s written: %dx%d, %d frames, %d bytes\n", videoName.c_str(), width, height, encodingFrameNumber, totalSize);
    // Final cleanup
    Revel_DestroyEncoder(encoderHandle);
    if (audioBuffer != NULL)
        delete [] audioBuffer;
    delete [] (int*)frame.pixels;

//    const float MBSize = 1048576.0;
//    float size = totalSize / MBSize;
//    std::cout.setf(std::ios::fixed);
//    std::cout << videoName << " (" << std::setprecision(2) << size << " MB) saved in " << width << "*" << height << " with " << encodingFrameNumber << " frames" << std::endl;


    // Restore current frame because video encoding changed it
    setTime(beforeTime);
}

Court *Engine::getCourt() const
{
    return court;
}

int Engine::getCurrentTime() const
{
    return currentTime;
}

int Engine::getEndTime() const
{
    return endTime;
}

int Engine::getStartTime() const
{
    return startTime;
}
