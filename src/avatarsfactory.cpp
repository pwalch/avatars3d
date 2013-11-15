/*
  * 3D Avatars
  * Pierre Walch
  */


#include <iostream>
#include <fstream>
#include <sstream>
#include <QDesktopWidget>
#include "engine.h"
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


void AvatarsFactory::constructCamera()
{
    Engine& e = Engine::getInstance();

    std::pair< std::map<int, vector3df>, std::map<int, vector3df> > cameraPositionAndRotation =
            mSettingsParser->retrieveCameraTrajectory();
    TrajectoryData* cameraTrajectory = new TrajectoryData(cameraPositionAndRotation.first,
                                                          cameraPositionAndRotation.second);
    CameraSettings cameraSettings = mSettingsParser->retrieveCameraSettings();

    const QRect& screenSize = QApplication::desktop()->geometry();
    if(cameraSettings.mWindowSize.Width > ((unsigned int)screenSize.width())
            || cameraSettings.mWindowSize.Height > ((unsigned int)screenSize.height())) {
        e.throwError("Window size is bigger than screen size");
    }

    // Actual instance creation
    CameraWindow* cameraWindow = new CameraWindow(cameraTrajectory, cameraSettings);
    CameraWindow::sInstance.reset(cameraWindow);
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
    std::map<int, std::map<int, vector3df> > playerTrajectories = mSettingsParser->retrievePlayerTrajectories();
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

        // Create dummy rotation map
        const std::map<int, vector3df>& positions = playerTrajectories[playerIndex];
        std::map<int, vector3df> rotations;
        for(std::map<int, vector3df>::const_iterator i = positions.begin();
                i != positions.end();
                ++i) {
            int playerIndex = i->first;
            rotations[playerIndex] = vector3df(0, 0, 0);
        }

        TrajectoryData* td = new TrajectoryData(positions, rotations);

        // Create customized settings for the current player
        BodySettings playerBodySettings = mSettingsParser->retrievePlayerBodySettings(teamToTexture[team]);
        PlayerSettings playerSettings = mSettingsParser->retrievePlayerSettings(team, jerseyNumber);

        // Instanciate the player and reference it in the map
        Player* p = new Player(td, playerBodySettings, playerSettings);
        playerMap[playerIndex] = p;
    }

    return playerMap;
}


MovingBody* AvatarsFactory::createBall()
{
    std::map<int, vector3df > ballPositions = mSettingsParser->retrieveBallTrajectory();

    // Create dummy rotation
    std::map<int, vector3df > ballRotations;
    for(std::map<int, vector3df >::iterator i = ballPositions.begin(); i != ballPositions.end(); ++i) {
        int index = i->first;
        ballRotations[index] = vector3df(0, 0, 0);
    }

    TrajectoryData* td = new TrajectoryData(ballPositions, ballRotations);
    BodySettings ballBodySettings = mSettingsParser->retrieveBallBodySettings();
    MovingBody* ball = new MovingBody(td, ballBodySettings);

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
