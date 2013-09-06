/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef COURT_H
#define COURT_H

#include <map>
#include <irrlicht.h>
#include "player.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;

/**
 * @brief Court containing players and ball (model)
 *
 * This class represents the model in MVC pattern. It contains all the player and ball trajectories.
 */
class Court
{
    public:

       /**
         * Court constructor
         * @param scenePath path to the Irrlicht scene of the court
         * @param scale scale of the -court- node of the scene
         * @param playerMap players of the court
         * @param ballInit ball of the court
         */
        Court(const io::path& scenePath, float scale,
              const std::map<int, Player*>& playerMap, MovingBody* ballInit);

        /**
         * Destroys the players and the ball
         */
        ~Court();

        /**
         * Adapts the positions of the players and of the ball to the new time value
         * @param time new date
         */
        void setTime(int time);

        /**
         * Returns the player map
         * @return player map
         */
        const std::map<int, Player *>& getPlayers() const;

private:
        ISceneNode* node;
        std::map<int, Player*> players;
        MovingBody* ball;
};

#endif // COURT_H
