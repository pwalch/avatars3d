/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef COURT_H
#define COURT_H

#include <map>
#include <irrlicht.h>
#include "ball.h"
#include "player.h"

/**
 * Court containing players
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
        Court(const irr::io::path& scenePath, float scale,
              const std::map<int, Player*>& playerMap, Ball* ballInit);

        /**
         * Destroys the players and the ball
         */
        ~Court();

        /**
         * Adapts the positions of the players and of the ball to the new time value
         * @param time new frame time
         */
        void setTime(int time);

        /**
         * Returns the player map
         * @return player map
         */
        const std::map<int, Player *>& getPlayers() const;

private:
        irr::scene::ISceneNode* node;

        std::map<int, Player*> players;
        Ball* ball;
};

#endif // COURT_H
