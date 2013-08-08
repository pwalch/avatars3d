#ifndef COURT_H
#define COURT_H

#include <map>
#include <irrlicht.h>
#include "ball.h"
#include "player.h"

/**
 * Court where the players are playing
 */
class Court
{
    public:
        /**
         * Initializes the court using model, texture and list of players
         * @param mapModelPath path to model
         * @param mapTexturePath path to texture
         * @param playerMap initialized player hashmap
         */
        Court(const irr::io::path& scenePath, float scale,
              const std::map<int, Player*>& playerMap, Ball* ballInit,
              int frameNumber, int framerate, int animFramerate,
              std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold);

        ~Court();

        /**
         * Adapts the positions of the players to the new time value
         * @param time new date
         */
        void setTime(const int time);

        std::map<int, Player *> getPlayers() const;

        irr::scene::ISceneNode *getNode() const;

private:
        void processPlayers(int frameNumber, int framerate, int animFramerate,
                            std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold);

        void processBall(int frameNumber);

        irr::scene::ISceneNode* node;

        std::map<int, Player*> players;
        Ball* ball;
};

#endif // COURT_H
