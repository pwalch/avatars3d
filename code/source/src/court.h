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

#ifndef COURT_H
#define COURT_H

#include <irrlicht.h>
#include "courtsettings.h"
#include "player.h"
#include "itimeable.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;

typedef std::map<int, std::unique_ptr<Player> > PlayerMap;

/**
 * @brief Court containing players and ball
 *
 * Represents the court or field where players are moving.
 * Contains all the player and ball trajectories, and also the background scene node.
 */
class Court : public ITimeable
{

public:

    /**
     * Constructs the court using the given settings
     * @param courtSettings court settings used to construct
     * @param playerMap players to use
     * @param ball ball to use
     */
    Court(const CourtSettings& courtSettings, std::unique_ptr<PlayerMap> playerMap, std::unique_ptr<MovingBody> ball);

    /**
     * Releases memory for all the players and the ball
     */
    virtual ~Court();

    /**
     * Appends the new player and ball chunks to their corresponding objects
     * @param playerChunk player trajectory chunk map
     * @param ballChunk ball trajectory chunk
     */
    void updateTrajectories(const std::map<int, VectorSequence>& playerChunk,
                            const VectorSequence& ballChunk);

    /**
     * Moves the players and the ball to the position (and orientation if player) according to their
     * trajectory data.
     * @param time time index
     */
    virtual void setTime(int time) override;

    /**
     * Returns the players
     * @return player map
     */
    const std::map<int, std::unique_ptr<Player> >& getPlayers() const;

private:
    ISceneNode* mNode;
    std::unique_ptr<PlayerMap> mPlayers;
    std::unique_ptr<MovingBody> mBall;
};

#endif // COURT_H
