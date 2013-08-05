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
    if(doc.LoadFile(cfgPath.c_str()) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : config file named " << cfgPath << " does not exist" << std::endl;
        exit(1);
    }

    tinyxml2::XMLElement* avatarsConfig = doc.FirstChildElement("avatarsConfig");
    if(avatarsConfig == NULL) {
        std::cerr << "Error : avatarsConfig tag does not exist in XML document" << std::endl;
        exit(1);
    }

    // Graphics settings
    tinyxml2::XMLElement* graphics = avatarsConfig->FirstChildElement("graphics");
    if(graphics == NULL) {
        std::cerr << "Error : graphics tag does not exist in avatarsConfig tag" << std::endl;
        exit(1);
    }

    tinyxml2::XMLElement* window = graphics->FirstChildElement("window");
    if(window == NULL) {
        std::cerr << "Error : window tag does not exist in graphics tag" << std::endl;
        exit(1);
    }
    int width = 0, height = 0;
    const char* textFont = window->Attribute("font");
    if(window->QueryIntAttribute("width", &width) != tinyxml2::XML_NO_ERROR
        || window->QueryIntAttribute("height", &height) != tinyxml2::XML_NO_ERROR
        || textFont == NULL) {
        std::cerr << "Error : window tag does not contain valid width or height or font attributes" << std::endl;
        exit(1);
    }
    irr::core::dimension2d<irr::u32> dimensions(width, height);


    // input settings
    tinyxml2::XMLElement* input = avatarsConfig->FirstChildElement("input");
    if(input == NULL) {
        std::cerr << "Error : input tag does not exist in avatarsConfig tag" << std::endl;
        exit(1);
    }
    tinyxml2::XMLElement* tracking = input->FirstChildElement("tracking");
    if(tracking == NULL) {
        std::cerr << "Error : tracking tag does not exist in input tag" << std::endl;
        exit(1);
    }

    const char* trajectoriesPath = tracking->Attribute("trajectories");
    const char* jerseyPath = tracking->Attribute("jerseys");
    if(tracking->QueryIntAttribute("frameNumber", &frameNumber) != tinyxml2::XML_NO_ERROR
        || tracking->QueryIntAttribute("frameRate", &framerate) != tinyxml2::XML_NO_ERROR || trajectoriesPath == NULL || jerseyPath == NULL) {
        std::cerr << "Error : tracking tag does not contain frameNumber or frameRate or trajectories or jerseys attributes" << std::endl;
        exit(1);
    }

    // Team settings
    tinyxml2::XMLElement* teams = input->FirstChildElement("teams");
    if(teams == NULL) {
        std::cerr << "Error : teams tag does not exist in input tag" << std::endl;
        exit(1);
    }
    int teamRedNormal, teamBlueNormal, teamRedSpecial, teamBlueSpecial;
    if(teams->QueryIntAttribute("redNormal", &teamRedNormal) != tinyxml2::XML_NO_ERROR
        || teams->QueryIntAttribute("blueNormal", &teamBlueNormal) != tinyxml2::XML_NO_ERROR
        || teams->QueryIntAttribute("redSpecial", &teamRedSpecial) != tinyxml2::XML_NO_ERROR
        || teams->QueryIntAttribute("blueSpecial", &teamBlueSpecial) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : teams tag does not contain valid redNormal or blueNormal or redSpecial or blueSpecial attributes" << std::endl;
        exit(1);
    }

    // Transformation settings
    tinyxml2::XMLElement* transformation = input->FirstChildElement("transformation");
    if(transformation == NULL) {
        std::cerr << "Error : transformation tag does not exist in input tag" << std::endl;
        exit(1);
    }
    float trajectoryOffsetX, trajectoryOffsetY, trajectoryScaleX, trajectoryScaleY;
    if(transformation->QueryFloatAttribute("offsetX", &trajectoryOffsetX) != tinyxml2::XML_NO_ERROR
        || transformation->QueryFloatAttribute("offsetY", &trajectoryOffsetY) != tinyxml2::XML_NO_ERROR
        || transformation->QueryFloatAttribute("scaleX", &trajectoryScaleX) != tinyxml2::XML_NO_ERROR
        || transformation->QueryFloatAttribute("scaleY", &trajectoryScaleY) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : transformation tag does not contain valid offsetX or offsetY or scaleX or scaleY attributes" << std::endl;
        exit(1);
    }



    // output settings
    tinyxml2::XMLElement* output = avatarsConfig->FirstChildElement("output");
    if(output == NULL) {
        std::cerr << "Error : output tag does not exist in avatarsConfig tag" << std::endl;
        exit(1);
    }

    // video settings
    tinyxml2::XMLElement* video = output->FirstChildElement("video");
    if(video == NULL) {
        std::cerr << "Error : video tag does not exist in avatarsConfig tag" << std::endl;
        exit(1);
    }

    const char* videoNameAtt = video->Attribute("name");
    if(videoNameAtt == NULL) {
        std::cerr << "Error : video tag does not contain name attribute" << std::endl;
        exit(1);
    }
    videoName = videoNameAtt;


    // Camera settings
    tinyxml2::XMLElement* camera = avatarsConfig->FirstChildElement("camera");
    if(camera == NULL) {
        std::cerr << "Error : camera tag does not exist in avatarsConfig tag" << std::endl;
        exit(1);
    }

    tinyxml2::XMLElement* options = camera->FirstChildElement("options");
    if(options == NULL) {
        std::cerr << "Error : camera tag does not contain options tag" << std::endl;
        exit(1);
    }
    int cameraSpeed;
    if(options->QueryIntAttribute("speed", &cameraSpeed) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : options tag does not contain valid speed attribute " << std::endl;
        exit(1);
    }

    tinyxml2::XMLElement* position = camera->FirstChildElement("position");
    tinyxml2::XMLElement* rotation = camera->FirstChildElement("rotation");
    if(position == NULL || rotation ==  NULL) {
        std::cerr << "Error : camera tag does not contain position or rotation tags" << std::endl;
        exit(1);
    }
    irr::core::vector3df initialPosition, initialRotation;
    if(position->QueryFloatAttribute("x", &initialPosition.X) != tinyxml2::XML_NO_ERROR
        || position->QueryFloatAttribute("y", &initialPosition.Y) != tinyxml2::XML_NO_ERROR
        || position->QueryFloatAttribute("z", &initialPosition.Z) != tinyxml2::XML_NO_ERROR
        || rotation->QueryFloatAttribute("x", &initialRotation.X) != tinyxml2::XML_NO_ERROR
        || rotation->QueryFloatAttribute("y", &initialRotation.Y) != tinyxml2::XML_NO_ERROR
        || rotation->QueryFloatAttribute("z", &initialRotation.Z) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : position or rotation tag do not contain valid x or y or z attributes" << std::endl;
        exit(1);
    }


    // Camera initialization
    CameraWindow& cam = CameraWindow::getInstance();
    cam.init(dimensions, initialPosition, initialRotation, textFont, cameraSpeed);



    // Avatars settings
    tinyxml2::XMLElement* avatars = avatarsConfig->FirstChildElement("avatars");
    if(avatars == NULL) {
        std::cerr << "Error : avatars tag does not exist in avatarsConfig tag" << std::endl;
        exit(1);
    }

    // Court settings
    tinyxml2::XMLElement* scene = avatars->FirstChildElement("scene");
    if(scene == NULL) {
        std::cerr << "Error : scene tag does not exist in avatars tag" << std::endl;
        exit(1);
    }
    const char* scenePath = scene->Attribute("irrscene");
    float courtScale;
    if(scenePath == NULL
        || scene->QueryFloatAttribute("scale", &courtScale) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : scene tag does not contain valid irrscene or scale attributes" << std::endl;
        exit(1);
    }

    // Players settings
    tinyxml2::XMLElement* players = avatars->FirstChildElement("players");
    if(players == NULL) {
        std::cerr << "Error : players tag does not exist in avatars tag" << std::endl;
        exit(1);
    }
    const char* playerModelPath = players->Attribute("model");
    float playerScale;
    const char* playerTextureRedNormal = players->Attribute("textureRedNormal");
    const char* playerTextureBlueNormal = players->Attribute("textureBlueNormal");
    const char* playerTextureRedSpecial = players->Attribute("textureRedSpecial");
    const char* playerTextureBlueSpecial = players->Attribute("textureBlueSpecial");
    if(playerModelPath == NULL
        || players->QueryFloatAttribute("scale", &playerScale) != tinyxml2::XML_NO_ERROR
        || playerTextureBlueNormal == NULL
        || playerTextureRedNormal == NULL
        || playerTextureRedSpecial == NULL
        || playerTextureBlueSpecial == NULL) {
        std::cerr << "Error : players tag does not contain model or scale or textureRedNormal or textureBlueNormal or textureRedSpecial or textureBlueSpecial attributes" << std::endl;
        exit(1);
    }

    std::map<AnimState, irr::core::vector2di> stateDates;
    // Vector.x => anim begin; vector.y => anim end
    int animFramerate;
    int standBegin, standEnd, walkBegin, walkEnd, runBegin, runEnd;
    if(players->QueryIntAttribute("framerate", &animFramerate) != tinyxml2::XML_NO_ERROR
            || players->QueryIntAttribute("standBegin", &standBegin) != tinyxml2::XML_NO_ERROR
            || players->QueryIntAttribute("standEnd", &standEnd) != tinyxml2::XML_NO_ERROR
            || players->QueryIntAttribute("walkBegin", &walkBegin) != tinyxml2::XML_NO_ERROR
            || players->QueryIntAttribute("walkEnd", &walkEnd) != tinyxml2::XML_NO_ERROR
            || players->QueryIntAttribute("runBegin", &runBegin) != tinyxml2::XML_NO_ERROR
            || players->QueryIntAttribute("runEnd", &runEnd) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : players tag does not contain valid standBegin or standEnd walkBegin or walkEnd or runBegin or runEnd attributes" << std::endl;
        exit(1);
    }
    stateDates[ANIMATION_STAND] = irr::core::vector2di(standBegin, standEnd);
    stateDates[ANIMATION_WALK] = irr::core::vector2di(walkBegin, walkEnd);
    stateDates[ANIMATION_RUN] = irr::core::vector2di(runBegin, runEnd);

    std::map<AnimState, float> stateThreshold;
    float walkThreshold, runThreshold;
    if(players->QueryFloatAttribute("walkThreshold", &walkThreshold) != tinyxml2::XML_NO_ERROR
            || players->QueryFloatAttribute("runThreshold", &runThreshold) != tinyxml2::XML_NO_ERROR) {
        std::cerr << "Error : players tag does not contain walkThreshold or runThreshold attributes" << std::endl;
        exit(1);
    }
    stateThreshold[ANIMATION_WALK] = walkThreshold;
    stateThreshold[ANIMATION_RUN] = runThreshold;

    // Get trajectories
    std::map<int, Player*> playerMap;
    std::ifstream trajectoriesFile;
    trajectoriesFile.open(trajectoriesPath);

    if(trajectoriesFile.is_open()) {
        while(trajectoriesFile.good()) {
            std::string line;
            std::getline(trajectoriesFile, line);
            std::vector<int> intLine = getSplittenLine(line);

            int frameIndex = intLine[0];
            int playerIndex = intLine[1];
            float posX = intLine[2];
            float posY = intLine[3];

            // If the player does not exist we create it
            if(playerMap.find(playerIndex) == playerMap.end())
                playerMap[playerIndex] = new Player();

            irr::core::vector3df position(posX * trajectoryScaleX + trajectoryOffsetX, 0, posY * trajectoryScaleY + trajectoryOffsetY);
            // We fill the map with the current frame
            playerMap[playerIndex]->mapTime(frameIndex, position);
        }
    }
    trajectoriesFile.close();

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

//    const int firstPos = 2;
//    const int secondPos = 4;
//    Player* p = new Player();
//    for(int i = 0; i < firstPos * framerate; ++i)
//        p->mapFrame(i, irr::core::vector3df(-100, 20, 50));
//    for(int i = 0; i < secondPos * framerate; ++i)
//        p->mapFrame(i + firstPos*framerate, irr::core::vector3df(-150, 20, 50));
//    p->init(playerModelPath, playerTextureBlue, 1);
//    playerMap[0] = p;

//    Player* p2 = new Player();
//    for(int i = 0; i < firstPos * framerate; ++i)
//        p2->mapFrame(i, irr::core::vector3df(-100, 20, -50));
//    for(int i = 0; i < secondPos * framerate; ++i)
//        p2->mapFrame(i + firstPos*framerate, irr::core::vector3df(-150, 20, -50));
//    p2->init(playerModelPath, playerTextureRed, 2);
//    playerMap[1] = p2;

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
            if(team == teamRedNormal)
                p->init(playerModelPath, playerTextureRedNormal, playerScale);
            else if(team == teamBlueNormal)
                p->init(playerModelPath, playerTextureBlueNormal, playerScale);
            else if(team == teamRedSpecial)
                p->init(playerModelPath, playerTextureRedSpecial, playerScale);
            else if(team == teamBlueSpecial)
                p->init(playerModelPath, playerTextureBlueSpecial, playerScale);
            else {
                std::cerr << "Error : player index " << i->first << " does not correspond to any team (" << team << ")" <<  std::endl;
                exit(1);
            }
            ++i;
        }
    }

    // Initialize court
    court = new Court(scenePath, courtScale, playerMap, frameNumber, framerate, animFramerate, stateDates, stateThreshold);

    // Update scene with the initialized court and the initialized camera
    setTime(0);
}

std::vector<int> Engine::getSplittenLine(std::string line) {
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

void Engine::setTime(const int time)
{
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
    // fields (especially ones that you may not know about) are initialized
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
