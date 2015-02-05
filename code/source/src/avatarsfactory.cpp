/*
 *  Copyright 2014 Pierre Walch
 *  Website : www.pwalch.net
 *
 *  Avatars is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Avatars is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
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

std::unique_ptr<CameraWindow> AvatarsFactory::createCamera() const
{
    Engine& e = Engine::getInstance();

    auto cameraSettings = mSettingsParser->retrieveCameraSettings();

    auto screenSize = QApplication::desktop()->geometry();
    if(cameraSettings.mWindowSize.Width > ((unsigned int)screenSize.width())
            || cameraSettings.mWindowSize.Height > ((unsigned int)screenSize.height())) {
        e.throwError("Window size is bigger than screen size");
    }

    // Actual instance creation
    return std::unique_ptr<CameraWindow>(new CameraWindow(cameraSettings));
}

std::unique_ptr<std::istream> AvatarsFactory::createCameraStream() const
{
    auto cameraFile = std::unique_ptr<std::ifstream>(new std::ifstream());
    cameraFile->open(mSettingsParser->retrieveCameraTrajectoryPath());
    if(!cameraFile->is_open()) {
        Engine::getInstance().throwError(L"Camera trajectory file cannot be opened");
    }

    return std::unique_ptr<std::istream>(std::move(cameraFile));
}

std::unique_ptr<std::istream> AvatarsFactory::createPlayerStream() const
{
    auto playerFile = std::unique_ptr<std::ifstream>(new std::ifstream());
    playerFile->open(mSettingsParser->retrievePlayerTrajectoryPath());
    if(!playerFile->is_open()) {
        Engine::getInstance().throwError(L"Player trajectory file cannot be opened");
    }

    return std::unique_ptr<std::istream>(std::move(playerFile));
}

std::unique_ptr<std::istream> AvatarsFactory::createBallStream() const
{
    auto ballFile = std::unique_ptr<std::ifstream>(new std::ifstream());
    ballFile->open(mSettingsParser->retrieveBallTrajectoryPath());
    if(!ballFile->is_open()) {
        Engine::getInstance().throwError(L"Ball trajectory file cannot be opened");
    }

    return std::unique_ptr<std::istream>(std::move(ballFile));
}

const std::pair<VectorSequence, VectorSequence> AvatarsFactory::createCameraChunk(std::istream &cameraStream, int nbFramesToCatch) const
{
    Engine& engine = Engine::getInstance();
    auto tfm = engine.getAffineTransformation();

    // Create pair of maps: first for position and second for rotation
    VectorSequence positions;
    VectorSequence rotations;

    int counter = 0;
    while(!cameraStream.eof()) {
        if(counter >= nbFramesToCatch) {
            break;
        }

        std::string line = "";
        std::getline(cameraStream, line);

        if(line.compare("") != 0) {
            int frameIndex = 0;
            vector3df realPosition, rotation;
            std::tie(frameIndex, realPosition, rotation) = SettingsParser::getCameraTokens(line);

            positions.set(frameIndex, tfm.convertToVirtual(realPosition));
            rotations.set(frameIndex, rotation);

            ++counter;
        }
    }

    return std::pair<VectorSequence, VectorSequence>(positions, rotations);
}

const std::map<int, VectorSequence > AvatarsFactory::createPlayerChunkMap(std::istream &playerStream,
                                                                      const std::map<int, std::unique_ptr<Player> >& playerMap,
                                                                      int framesToCatch) const
{
    Engine& engine = Engine::getInstance();
    auto tfm = engine.getAffineTransformation();

    std::map<int, VectorSequence > sequenceMap;
    unsigned int counter = 0;
    while(!playerStream.eof()) {
        if(counter >= framesToCatch * playerMap.size()) {
            break;
        }

        std::string line = "";
        std::getline(playerStream, line);

        if(line.compare("") != 0) {
            int frameIndex = 0, playerIndex = 0;
            vector2df pos2D;
            std::tie(frameIndex, playerIndex, pos2D) = SettingsParser::getPlayerTokens(line);

            if(playerMap.find(playerIndex) != playerMap.end()) {
                const vector3df realPosition(pos2D.X, pos2D.Y, 0);
                VectorSequence& sequence = sequenceMap[playerIndex];
                sequence.set(frameIndex, tfm.convertToVirtual(realPosition));

                ++counter;
            }
        }
    }

    return sequenceMap;
}

const VectorSequence AvatarsFactory::createBallChunk(std::istream& ballStream, int framesToCatch) const
{
    Engine& engine = Engine::getInstance();
    auto tfm = engine.getAffineTransformation();

    VectorSequence positions;
    int counter = 0;
    while(!ballStream.eof()) {
        if(counter >= framesToCatch) {
            break;
        }

        std::string line = "";
        std::getline(ballStream, line);
        if(line.compare("") != 0) {
            int frameIndex = 0;
            vector3df realPosition;
            std::tie(frameIndex, realPosition) = SettingsParser::getBallTokens(line);

            const vector3df virtualPosition = tfm.convertToVirtual(realPosition);
            positions.set(frameIndex, virtualPosition);

            ++counter;
        }
    }

    return positions;
}

std::unique_ptr<Court> AvatarsFactory::createCourt() const
{
    auto playerMap = createPlayerMap();
    auto ball = createBall();
    auto courtSettings = mSettingsParser->retrieveCourtSettings();

    return std::unique_ptr<Court>(new Court(courtSettings, std::move(playerMap), std::move(ball)));
}

std::unique_ptr<PlayerMap> AvatarsFactory::createPlayerMap() const
{
    std::unique_ptr<PlayerMap> playerMap(new PlayerMap());

    // Retrieve player data from configuration file
    auto teamToTexture = mSettingsParser->retrieveTeamToTexture();
    auto playerToTeamAndJerseyNumber = mSettingsParser->retrievePlayerToTeamAndJerseyNumber();

    for(auto i = playerToTeamAndJerseyNumber.cbegin();
            i != playerToTeamAndJerseyNumber.cend();
            ++i) {

        int playerIndex = i->first;
        auto teamJerseyNumberPair = i->second;
        int team = teamJerseyNumberPair.first;
        int jerseyNumber = teamJerseyNumberPair.second;

        // Create customized settings for the current player
        auto playerBodySettings = mSettingsParser->retrievePlayerBodySettings(teamToTexture[team]);
        auto playerSettings = mSettingsParser->retrievePlayerSettings(team, jerseyNumber);

        // Instanciate the player and reference it in the map
        (*playerMap)[playerIndex] = std::unique_ptr<Player>(new Player(playerBodySettings, playerSettings));
    }

    return playerMap;
}


std::unique_ptr<MovingBody> AvatarsFactory::createBall() const
{
    auto ballBodySettings = mSettingsParser->retrieveBallBodySettings();

    return std::unique_ptr<MovingBody>(new MovingBody(ballBodySettings));
}

SequenceSettings AvatarsFactory::retrieveSequenceSettings() const
{
    auto sequenceSettings = mSettingsParser->retrieveSequenceSettings();
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

std::unique_ptr<AffineTransformation> AvatarsFactory::createTransformation() const
{
    auto transformation = mSettingsParser->retrieveAffineTransformationPair();

    return std::unique_ptr<AffineTransformation>(new AffineTransformation(transformation.first, transformation.second));
}
