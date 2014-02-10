/*
  * 3D Avatars
  * Pierre Walch
  */


#include <iostream>
#include <fstream>
#include <sstream>
#include <QDesktopWidget>
#include "engine.h"
#include "science.h"
#include "camerawindow.h"
#include "avatarsfactory.h"

AvatarsFactory::AvatarsFactory(std::string cfgPath)
{
    mSettingsParser = std::unique_ptr<SettingsParser>(new SettingsParser(cfgPath.c_str()));
}

AvatarsFactory::~AvatarsFactory()
{

}


std::unique_ptr<CameraWindow> AvatarsFactory::createCamera()
{
    Engine& e = Engine::getInstance();

    CameraSettings cameraSettings = mSettingsParser->retrieveCameraSettings();

    const QRect& screenSize = QApplication::desktop()->geometry();
    if(cameraSettings.mWindowSize.Width > ((unsigned int)screenSize.width())
            || cameraSettings.mWindowSize.Height > ((unsigned int)screenSize.height())) {
        e.throwError("Window size is bigger than screen size");
    }

    // Actual instance creation
    return std::unique_ptr<CameraWindow>(new CameraWindow(cameraSettings));
}

std::unique_ptr<std::istream> AvatarsFactory::createCameraStream()
{
    std::unique_ptr<std::ifstream> cameraFile(new std::ifstream());
    cameraFile->open(mSettingsParser->retrieveCameraTrajectoryPath());
    if(!cameraFile->is_open()) {
        Engine::getInstance().throwError(L"Camera trajectory file cannot be opened");
    }

    return std::unique_ptr<std::istream>(std::move(cameraFile));
}

std::unique_ptr<std::istream> AvatarsFactory::createPlayerStream()
{
    std::unique_ptr<std::ifstream> playerFile(new std::ifstream());
    playerFile->open(mSettingsParser->retrievePlayerTrajectoryPath());
    if(!playerFile->is_open()) {
        Engine::getInstance().throwError(L"Player trajectory file cannot be opened");
    }

    return std::unique_ptr<std::istream>(std::move(playerFile));
}

std::unique_ptr<std::istream> AvatarsFactory::createBallStream()
{
    std::unique_ptr<std::ifstream> ballFile(new std::ifstream());
    ballFile->open(mSettingsParser->retrieveBallTrajectoryPath());
    if(!ballFile->is_open()) {
        Engine::getInstance().throwError(L"Ball trajectory file cannot be opened");
    }

    return std::unique_ptr<std::istream>(std::move(ballFile));
}

std::pair<VectorSequence, VectorSequence> AvatarsFactory::createCameraChunk(std::istream &cameraStream, int nbFramesToCatch)
{
    Engine& engine = Engine::getInstance();
    const AffineTransformation& tfm = engine.getTransformation();

    // Create pair of maps: first for position and second for rotation
    VectorSequence positions;
    VectorSequence rotations;

    int counter = 0;
    while(!cameraStream.eof()) {
        if(counter >= nbFramesToCatch) {
            break;
        }

        std::string line;
        std::getline(cameraStream, line);
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

            positions.set(frameIndex, tfm.convertToVirtual(realPosition));
            rotations.set(frameIndex, rotation);

            ++counter;
        }
    }

    return std::pair<VectorSequence, VectorSequence>(positions, rotations);
}

std::map<int, VectorSequence > AvatarsFactory::createPlayerChunkMap(std::istream &playerStream,
                                                                      const std::map<int, std::unique_ptr<Player> >& playerMap,
                                                                      int framesToCatch)
{
    Engine& engine = Engine::getInstance();
    const AffineTransformation& tfm = engine.getTransformation();


    std::map<int, VectorSequence > sequenceMap;
    unsigned int counter = 0;
    while(!playerStream.eof()) {
        if(counter >= framesToCatch * playerMap.size()) {
            break;
        }

        std::string line;
        std::getline(playerStream, line);
        std::vector<float> floatLine = SettingsParser::getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int frameIndex = (int) floatLine[0];
            int playerIndex = (int) floatLine[1];

            if(playerMap.find(playerIndex) != playerMap.end()) {
                float posX = floatLine[2];
                float posY = floatLine[3];

                const vector3df realPosition(posX, posY, 0);
                VectorSequence& sequence = sequenceMap[playerIndex];
                sequence.set(frameIndex, tfm.convertToVirtual(realPosition));

                ++counter;
            }
        }
    }

    return sequenceMap;
}

VectorSequence AvatarsFactory::createBallChunk(std::istream& ballStream, int framesToCatch)
{
    Engine& engine = Engine::getInstance();
    const AffineTransformation& tfm = engine.getTransformation();

    VectorSequence positions;
    int counter = 0;
    while(!ballStream.eof()) {
        if(counter >= framesToCatch) {
            break;
        }

        std::string line;
        std::getline(ballStream, line);
        std::vector<float> floatLine = SettingsParser::getSplittenLine(line);

        if(floatLine.size() >= 4) {
            int frameIndex = (int) floatLine[0];

            float posX = floatLine[1];
            float posY = floatLine[2];
            float posZ = floatLine[3];

            // We apply the scaling-offset transformation
            const vector3df realPosition(posX, posY, posZ);
            const vector3df virtualPosition = tfm.convertToVirtual(realPosition);
            positions.set(frameIndex, virtualPosition);

            ++counter;
        }
    }

    return positions;
}

std::unique_ptr<Court> AvatarsFactory::createCourt()
{
    std::unique_ptr<PlayerMap> playerMap = createPlayerMap();
    std::unique_ptr<MovingBody> ball = createBall();

    CourtSettings courtSettings = mSettingsParser->retrieveCourtSettings();

    return std::unique_ptr<Court>(new Court(courtSettings, std::move(playerMap), std::move(ball)));
}

std::unique_ptr<PlayerMap> AvatarsFactory::createPlayerMap()
{
    std::unique_ptr<PlayerMap> playerMap(new PlayerMap());

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
        (*playerMap)[playerIndex] = std::unique_ptr<Player>(new Player(playerBodySettings, playerSettings));
    }

    return playerMap;
}


std::unique_ptr<MovingBody> AvatarsFactory::createBall()
{
    BodySettings ballBodySettings = mSettingsParser->retrieveBallBodySettings();

    return std::unique_ptr<MovingBody>(new MovingBody(ballBodySettings));
}

SequenceSettings AvatarsFactory::retrieveSequenceSettings()
{
    SequenceSettings sequenceSettings = mSettingsParser->retrieveSequenceSettings();

    Engine& e = Engine::getInstance();

    if(sequenceSettings.mInitialTime < 0 || sequenceSettings.mInitialTime > (sequenceSettings.mFrameNumber - 1)) {
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

std::unique_ptr<AffineTransformation> AvatarsFactory::createTransformation()
{
    std::pair<vector3df, vector3df> transformation = mSettingsParser->retrieveAffineTransformation();

    return std::unique_ptr<AffineTransformation>(new AffineTransformation(transformation.first, transformation.second));
}
