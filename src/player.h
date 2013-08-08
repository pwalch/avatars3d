#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <map>
#include <vector>
#include "movingbody.h"

enum { NOT_A_PLAYER = -100 };

enum AnimState{ ANIMATION_STAND, ANIMATION_WALK, ANIMATION_RUN, ANIMATION_JUMP };

/**
 * Sport player, who's trajectory is obtained by saving position in each frame
 */
class Player : public MovingBody
{
    public:
        /**
         * Does nothing but create text node, use init after having written the position
         */
        Player();

        virtual ~Player();

        void init(irr::core::stringw name, const irr::io::path& modelPath, const irr::io::path& texturePath, float scale, const irr::core::dimension2d<irr::u32> textureSize, const irr::core::recti jerseyNumberRectInit);

        int getTeam() const;
        void setTeam(int value);
        void setJerseyNumber(int number);
        int getJerseyNumber() const;
        const irr::core::stringw& getJerseyText() const;
        irr::core::recti getJerseyNumberRect();

        void computeSpeed(int frameNumber, int framerate, int animFramerate, std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold);

        irr::video::ITexture *getRenderTexture() const;

        void setTime(float time);


private:
        irr::video::ITexture* renderTexture;

        irr::core::recti jerseyNumberRect;
        int jerseyNumber;
        irr::core::stringw jerseyText;
        int team;

        std::map < int, AnimState > animState;
        std::map < int, int > animFrame;
};

#endif // PLAYER_H
