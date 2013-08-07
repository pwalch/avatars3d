#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <map>

enum { NOT_A_PLAYER = -100 };

enum AnimState{ ANIMATION_STAND, ANIMATION_WALK, ANIMATION_RUN, ANIMATION_JUMP };

/**
 * Sport player, who's trajectory is obtained by saving position in each frame
 */
class Player
{
    public:
        /**
         * Does nothing but create text node, use init after having written the position
         */
        Player();

        ~Player();

        void mapTime(int time, irr::core::vector3df position);

        /**
         * Initializes player
         * @param modelPath path to model
         * @param texturePath path to texture
         * @param jerseyNumberInit player jersey number
         */
        void init(const irr::io::path& modelPath, const irr::io::path& texturePath, const irr::core::dimension2d<irr::u32> textureSize, const irr::core::recti jerseyNumberRectInit, float scale);

        /**
         * Adapts the position of the model to the wanted time value
         * @param time new date
         */
        void setTime(int time);

        std::map<int, irr::core::vector3df> lastPositions(int from, int samples);

        int getTeam() const;
        void setTeam(int value);
        void setJerseyNumber(int number);
        int getJerseyNumber() const;
        const irr::core::stringw& getJerseyText() const;
        irr::core::recti getJerseyNumberRect();

        void smoothTrajectory(int frameNumber);
        void computeSpeed(int frameNumber, int framerate, int animFramerate, std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold);

        irr::video::ITexture* getTexture();
        irr::video::ITexture *getRenderTexture() const;

private:

        irr::scene::IAnimatedMeshSceneNode* node;
        irr::video::ITexture* renderTexture;
        irr::video::ITexture* texture;

        irr::core::recti jerseyNumberRect;
        irr::scene::ITextSceneNode* textNode;
        int jerseyNumber;
        irr::core::stringw jerseyText;
        int team;

        std::map < int, irr::core::vector3df > trajectory;
        std::map < int, irr::core::vector3df > speed;
        std::map < int, float > rotationAngle;
        std::map < int, AnimState > animState;
        std::map < int, int > animFrame;
};

#endif // PLAYER_H
