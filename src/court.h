/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef COURT_H
#define COURT_H

#include <map>
#include <irrlicht.h>
#include "courtsettings.h"
#include "player.h"
#include "timeable.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;

/**
 * @brief Court containing players and ball
 *
 * Represents the court or field where players are moving.
 * Contains all the player and ball trajectories, and also the background scene node.
 */
class Court : public Timeable
{
    public:

        /**
         * Constructs the court using the given settings
         * @param courtSettings court settings used to construct
         * @param playerMap players to use
         * @param ball ball to use
         */
        Court(CourtSettings courtSettings, const std::map<int, Player*>& playerMap, MovingBody* ball);

        /**
         * Releases memory for all the players and the ball
         */
        virtual ~Court();

        /**
         * Moves the players and the ball to the position (and orientation if player) according to their
         * trajectory data.
         * @param time time index
         */
        void setTime(int time);

        /**
         * Returns the players
         * @return player map
         */
        const std::map<int, Player *>& getPlayers() const;

    private:
        ISceneNode* mNode;
        std::map<int, Player*> mPlayers;
        MovingBody* mBall;
};

#endif // COURT_H
