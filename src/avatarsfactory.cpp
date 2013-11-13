/**
  * 3D Avatars
  * Pierre Walch
  */


#include <iostream>
#include <fstream>
#include <sstream>
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
    TrajectoryData* cameraTrajectory = mSettingsParser->retrieveCameraTrajectory();
    CameraSettings cameraSettings = mSettingsParser->retrieveCameraSettings();
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

    std::map<int, std::map<int, vector3df> > playerTrajectories = mSettingsParser->retrievePlayerTrajectories();
    std::map<int, const char*> teamToTexture = mSettingsParser->retrieveTeamToTexture();
    std::map<int, std::pair<int, int> > playerToTeamAndJerseyNumber =
            mSettingsParser->retrievePlayerToTeamAndJerseyNumber();

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

        BodySettings playerBodySettings = mSettingsParser->retrievePlayerBodySettings(teamToTexture[team]);
        PlayerSettings playerSettings = mSettingsParser->retrievePlayerSettings(team, jerseyNumber);

        Player* p = new Player(td, playerBodySettings, playerSettings);
        playerMap[index] = p;
    }

    return playerMap;
}



MovingBody* AvatarsFactory::createBall()
{
    std::map<int, vector3df > ballPositions = mSettingsParser->retrieveBallTrajectory();
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

    return sequenceSettings;
}

AffineTransformation *AvatarsFactory::createTransformation()
{
    std::pair<vector3df, vector3df> transformation = mSettingsParser->retrieveAffineTransformation();
    AffineTransformation* aff = new AffineTransformation(transformation.first, transformation.second);

    return aff;
}
