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
#include <map>
#include <irrlicht.h>
#include "camerawindow.h"
#include "player.h"
#include "court.h"
#include "engine.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

Court::Court(const CourtSettings& courtSettings,
             std::unique_ptr< PlayerMap > playerMap,
             std::unique_ptr<MovingBody> ball)
{
    mPlayers = std::move(playerMap);
    //mPlayers = playerMap;
    mBall = std::move(ball);

    CameraWindow& cam = Engine::getInstance().getCameraWindow();
    auto sceneManager = cam.getSceneManager();

    Engine& engine = Engine::getInstance();

    if(sceneManager->loadScene(courtSettings.mScenePath) == false) {
        engine.throwError(L"Scene file could not be loaded");
    }

    mNode = sceneManager->getSceneNodeFromName("court");
    if(mNode == nullptr) {
        engine.throwError(L"Scene file does not contain court node");
    }

    mNode->setVisible(true);
    const float scale = courtSettings.mScale;
    mNode->setScale(vector3df(scale, scale, scale));

    // Activate smoothing functions
    mNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    mNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    mNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
}

Court::~Court()
{
}

void Court::updateTrajectories(const std::map<int, VectorSequence>& playerChunk,
                               const VectorSequence& ballChunk)
{
    // Update positions of each player
    for(auto i = playerChunk.cbegin(); i != playerChunk.cend(); ++i) {
        (*mPlayers)[i->first]->updatePositions(i->second);
    }

    mBall->updatePositions(ballChunk);
}

void Court::setTime(int time)
{
    for(auto i = mPlayers->cbegin(); i != mPlayers->cend(); ++i) {
        i->second->setTime(time);
    }

    mBall->setTime(time);
}

const std::map<int, std::unique_ptr<Player> > & Court::getPlayers() const
{
    return *mPlayers;
}

