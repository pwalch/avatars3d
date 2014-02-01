/*
  * 3D Avatars
  * Pierre Walch
  */


#include <iostream>
#include <fstream>
#include <sstream>
#include <QDesktopWidget>
#include "engine.h"
#include "trajectorychunk.h"
#include "science.h"
#include "camerawindow.h"
#include "avatarsfactory.h"

AvatarsFactory::AvatarsFactory(std::string cfgPath)
{
    mSettingsParser = new SettingsParser(cfgPath.c_str());
}

AvatarsFactory::~AvatarsFactory()
{
    delete mSettingsParser;
}


CameraWindow* AvatarsFactory::createCamera()
{
    Engine& e = Engine::getInstance();

    CameraSettings cameraSettings = mSettingsParser->retrieveCameraSettings();

    const QRect& screenSize = QApplication::desktop()->geometry();
    if(cameraSettings.mWindowSize.Width > ((unsigned int)screenSize.width())
            || cameraSettings.mWindowSize.Height > ((unsigned int)screenSize.height())) {
        e.throwError("Window size is bigger than screen size");
    }

    // Actual instance creation
    CameraWindow* cameraWindow = new CameraWindow(cameraSettings);

    return cameraWindow;
}

std::istream *AvatarsFactory::createCameraStream()
{
    std::ifstream* cameraFile = new std::ifstream();
    cameraFile->open(mSettingsParser->retrieveCameraTrajectoryPath());
    if(!cameraFile->is_open()) {
        Engine::getInstance().throwError(L"Camera trajectory file cannot be opened");
    }

    return cameraFile;
}

std::istream *AvatarsFactory::createPlayerStream()
{
    std::ifstream* playerFile = new std::ifstream();
    playerFile->open(mSettingsParser->retrievePlayerTrajectoryPath());
    if(!playerFile->is_open()) {
        Engine::getInstance().throwError(L"Player trajectory file cannot be opened");
    }

    return playerFile;
}

std::istream *AvatarsFactory::createBallStream()
{
    std::ifstream* ballFile = new std::ifstream();
    ballFile->open(mSettingsParser->retrieveBallTrajectoryPath());
    if(!ballFile->is_open()) {
        Engine::getInstance().throwError(L"Ball trajectory file cannot be opened");
    }

    return ballFile;
}

TrajectoryChunk *AvatarsFactory::createCameraChunk(std::istream *cameraStream, int nbFramesToCatch)
{
    Engine& engine = Engine::getInstance();
    AffineTransformation* tfm = engine.getTransformation();

    // Create pair of maps: first for position and second for rotation
    std::map < int, vector3df > positions;
    std::map < int, vector3df > rotations;

    int counter = 0;
    while(!cameraStream->eof()) {
        if(counter >= nbFramesToCatch) {
            break;
        }

        std::string line;
        std::getline(*cameraStream, line);
        std::vector<float> floatLine = SettingsParser::getSplittenLine(line);
        if(floatLine.size() >= 7) {
            int frameIndex = (int) floatLine[0];

            float posX = floatLine[1];
            float posY = floatLine[2];
            float posZ = floatLine[3];
            float rotX = floatLine[4];
            float rotY = floatLine[5];
            float rotZ = floatLine[6];

            // We apply the scaling-offset transformation
            const vector3df realPosition(posX, posY, posZ);
            const vector3df rotation(rotX, rotY, rotZ);

            positions[frameIndex] = tfm->convertToVirtual(realPosition);
            rotations[frameIndex] = rotation;

            ++counter;
        }
    }

    return new TrajectoryChunk(positions, rotations);
}

std::map<int, TrajectoryChunk* > AvatarsFactory::createPlayerChunkMap(std::istream *playerStream,
                                                                      std::map<int,Player*> playerMap,
                                                                      int framesToCatch)
{
    Engine& engine = Engine::getInstance();
    AffineTransformation* tfm = engine.getTransformation();


    std::map<int, std::map<int, vector3df> > positionMap;

    unsigned int counter = 0;
    while(!playerStream->eof()) {
        if(counter >= framesToCatch * playerMap.size()) {
            break;
        }

        std::string line;
        std::getline(*playerStream, line);
        std::vector<float> floatLine = SettingsParser::getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int frameIndex = (int) floatLine[0];
            int playerIndex = (int) floatLine[1];

            if(playerMap.find(playerIndex) != playerMap.end()) {
                float posX = floatLine[2];
                float posY = floatLine[3];

                const vector3df realPosition(posX, posY, 0);
                positionMap[playerIndex][frameIndex] = tfm->convertToVirtual(realPosition);

                ++counter;
            }
        }
    }

    std::map < int, TrajectoryChunk* > chunkMap;
    for(std::map<int, std::map<int, vector3df> >::const_iterator i = positionMap.begin();
        i != positionMap.end();
        ++i) {
        const int playerIndex = i->first;
        const std::map<int, vector3df>& positions = i->second;

        std::map<int, vector3df> rotations;
        chunkMap[playerIndex] = new TrajectoryChunk(positions, rotations);
    }

    return chunkMap;
}

TrajectoryChunk *AvatarsFactory::createBallChunk(std::istream* ballStream, int framesToCatch)
{
    Engine& engine = Engine::getInstance();
    AffineTransformation* tfm = engine.getTransformation();

    std::map<int, vector3df> positions;
    std::map<int, vector3df> rotations;

    int counter = 0;
    while(!ballStream->eof()) {
        if(counter >= framesToCatch) {
            break;
        }

        std::string line;
        std::getline(*ballStream, line);
        std::vector<float> floatLine = SettingsParser::getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int frameIndex = (int) floatLine[0];

            float posX = floatLine[1];
            float posY = floatLine[2];
            float posZ = floatLine[3];

            // We apply the scaling-offset transformation
            const vector3df realPosition(posX, posY, posZ);
            const vector3df virtualPosition = tfm->convertToVirtual(realPosition);
            positions[frameIndex] = virtualPosition;
            rotations[frameIndex] = vector3df(0, 0, 0);

            ++counter;
        }
    }

   return new TrajectoryChunk(positions, rotations);
}

Court *AvatarsFactory::createCourt()
{
    std::map<int, Player*> playerMap = createPlayerMap();

    MovingBody* ball = createBall();

    CourtSettings courtSettings = mSettingsParser->retrieveCourtSettings();
    Court* court = new Court(courtSettings, playerMap, ball);

    return court;
}

std::map<int, Player *> AvatarsFactory::createPlayerMap()
{
    std::map<int, Player*> playerMap;

    // Retrieve player data from configuration file
    std::map<int, const char*> teamToTexture = mSettingsParser->retrieveTeamToTexture();
    std::map<int, std::pair<int, int> > playerToTeamAndJerseyNumber =
            mSettingsParser->retrievePlayerToTeamAndJerseyNumber();


    for(std::map<int, std::pair<int, int> >::iterator i = playerToTeamAndJerseyNumber.begin();
            i != playerToTeamAndJerseyNumber.end();
            ++i) {

        int playerIndex = i->first;
        std::pair<int, int> teamAndJerseyNumber = i->second;
        int team = teamAndJerseyNumber.first;
        int jerseyNumber = teamAndJerseyNumber.second;

        // Create customized settings for the current player
        BodySettings playerBodySettings = mSettingsParser->retrievePlayerBodySettings(teamToTexture[team]);
        PlayerSettings playerSettings = mSettingsParser->retrievePlayerSettings(team, jerseyNumber);

        // Instanciate the player and reference it in the map
        Player* p = new Player(playerBodySettings, playerSettings);
        playerMap[playerIndex] = p;
    }

    return playerMap;
}


MovingBody* AvatarsFactory::createBall()
{
    BodySettings ballBodySettings = mSettingsParser->retrieveBallBodySettings();
    MovingBody* ball = new MovingBody(ballBodySettings);

    return ball;
}

SequenceSettings AvatarsFactory::retrieveSequenceSettings()
{
    SequenceSettings sequenceSettings = mSettingsParser->retrieveSequenceSettings();

    Engine& e = Engine::getInstance();

    if(sequenceSettings.mCurrentTime < 0 || sequenceSettings.mCurrentTime > (sequenceSettings.mFrameNumber - 1)) {
        e.throwError(L"Invalid initial time index");
    }

    if(sequenceSettings.mStartTime < 0 || sequenceSettings.mStartTime > (sequenceSettings.mFrameNumber - 1)) {
        e.throwError(L"Invalid recording start time index");
    }

    if(sequenceSettings.mEndTime < 0 || sequenceSettings.mEndTime > (sequenceSettings.mFrameNumber - 1)) {
        e.throwError(L"Invalid recording end time index");
    }

    return sequenceSettings;
}

AffineTransformation *AvatarsFactory::createTransformation()
{
    std::pair<vector3df, vector3df> transformation = mSettingsParser->retrieveAffineTransformation();
    AffineTransformation* aff = new AffineTransformation(transformation.first, transformation.second);

    return aff;
}
