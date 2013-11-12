/**
  * 3D Avatars
  * Pierre Walch
  */


#include <iostream>
#include <fstream>
#include <sstream>
#include "engine.h"
#include "camerawindow.h"
#include "settingsfactory.h"

SettingsFactory::SettingsFactory(std::string cfgPath)
{
    mConfigurationFileParser = new ConfigurationFileParser(cfgPath.c_str());
}

SettingsFactory::~SettingsFactory()
{
    delete mConfigurationFileParser;
}


std::map<int, Player *> SettingsFactory::createPlayerMap()
{
    std::map<int, Player*> playerMap;

    std::map<int, std::map<int, vector3df> > playerTrajectories = mConfigurationFileParser->retrievePlayerTrajectories();
    std::map<int, const char*> teamToTexture = mConfigurationFileParser->retrieveTeamToTexture();
    std::map<int, std::pair<int, int> > playerToTeamAndJerseyNumber = mConfigurationFileParser->retrievePlayerToTeamAndJerseyNumber();

    for(std::map<int, std::pair<int, int> >::iterator i = playerToTeamAndJerseyNumber.begin();
            i != playerToTeamAndJerseyNumber.end();
            ++i) {
        int index = i->first;
        std::pair<int, int> teamAndJerseyNumber = i->second;
        int team = teamAndJerseyNumber.first;
        int jerseyNumber = teamAndJerseyNumber.second;


        const std::map<int, vector3df>& positions = playerTrajectories[index];
        std::map<int, vector3df> rotations;
        for(std::map<int, vector3df>::const_iterator i = positions.begin();
                i != positions.end();
                ++i) {
            int index = i->first;
            rotations[index] = vector3df(0, 0, 0);
        }

        TrajectoryData* td = new TrajectoryData(playerTrajectories[index], rotations);

        MoveableSettings playerMoveableSettings = mConfigurationFileParser->retrievePlayerMoveableSettings();
        MovingBodySettings playerBodySettings = mConfigurationFileParser->retrievePlayerBodySettings(teamToTexture[team]);
        PlayerSettings playerSettings = mConfigurationFileParser->retrievePlayerSettings(jerseyNumber);

        Player* p = new Player(td, playerMoveableSettings, playerBodySettings, playerSettings);
        playerMap[index] = p;
    }

    return playerMap;
}

Court *SettingsFactory::createCourt()
{
    std::map<int, Player*> playerMap = createPlayerMap();
    MovingBody* ball = createBall();

    CourtSettings courtSettings = mConfigurationFileParser->retrieveCourtSettings();
    Court* court = new Court(courtSettings, playerMap, ball);

    return court;
}

SequenceSettings SettingsFactory::retrieveSequenceSettings()
{
    return mConfigurationFileParser->retrieveSequenceSettings();
}

void SettingsFactory::constructCamera()
{
    TrajectoryData* cameraTrajectory = mConfigurationFileParser->retrieveCameraTrajectory();
    MoveableSettings cameraMoveableSettings = mConfigurationFileParser->retrieveCameraMoveableSettings();
    CameraSettings cameraSettings = mConfigurationFileParser->retrieveCameraSettings();
    CameraWindow* cameraWindow = new CameraWindow(cameraTrajectory, cameraMoveableSettings, cameraSettings);
    CameraWindow::mInstance.reset(cameraWindow);
}


MovingBody* SettingsFactory::createBall()
{
    std::map<int, vector3df > ballPositions = mConfigurationFileParser->retrieveBallTrajectory();
    std::map<int, vector3df > ballRotations;
    for(std::map<int, vector3df >::iterator i = ballPositions.begin(); i != ballPositions.end(); ++i) {
        int index = i->first;
        ballRotations[index] = vector3df(0, 0, 0);
    }

    TrajectoryData* td = new TrajectoryData(ballPositions, ballRotations);

    MoveableSettings moveableBallSettings = mConfigurationFileParser->retrieveBallMoveableSettings();
    MovingBodySettings ballBodySettings = mConfigurationFileParser->retrieveBallBodySettings();

    MovingBody* ball = new MovingBody(td, moveableBallSettings, ballBodySettings);

    return ball;
}


AffineTransformation *SettingsFactory::createAffineTransformation()
{
    return mConfigurationFileParser->createAffineTransformation();
}
