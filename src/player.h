/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef PLAYER_H
#define PLAYER_H

#include <irrlicht.h>
#include <map>
#include <vector>
#include "movingbody.h"

/**
 * Enum value to distinguish players of the teams from other people of the tracking file
 */
enum { NOT_A_PLAYER = -100 };

/**
 * Different animations
 */
enum AnimState{ ANIMATION_STAND, ANIMATION_WALK, ANIMATION_RUN, ANIMATION_JUMP };

/**
 * Sport player
 */
class Player : public MovingBody
{
    public:
        /**
         * Does nothing but create text node, use init after having written the position
         */
        Player();

        /**
         * Initializes the player
         * @param name name of the player
         * @param modelPath path to model
         * @param texturePath path to texture
         * @param scale model scale
         * @param textureSize texture dimensions
         * @param jerseyTextRectInit rectangle where to put the jersey text on the texture
         * @param trajColor color of trajectory curve
         * @param frameNumber number of frames in the tracking video
         * @param framerate framerate of tracking video
         * @param animFramerate framerate of model animation
         * @param stateDates list of beginFrame-endFrame pairs corresponding to each animation
         * @param stateThreshold list of threshold for animation changing
         */
        void init(irr::core::stringw name, const irr::io::path& modelPath, const irr::io::path& texturePath, float scale, const irr::core::dimension2d<irr::u32> textureSize, const irr::core::recti jerseyTextRectInit, const irr::video::SColor& trajColor, int frameNumber, int framerate, int animFramerate, std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold);

        /**
         * Returns team ID of the player
         * @return team ID
         */
        int getTeam() const;

        /**
         * Sets team ID of the player
         * @param value team ID value
         */
        void setTeam(int value);

        /**
         * Sets jersey number of the player
         * @param number jersey number
         */
        void setJerseyNumber(int number);

        /**
         * Returns jersey number of the player
         * @return jersey number
         */
        int getJerseyNumber() const;

        /**
         * Returns text displayed on the player jersey
         * @return jersey text
         */
        const irr::core::stringw& getJerseyText() const;

        /**
         * Returns rectangle corresponding to where the jersey text must be written
         * @return jersey text rectangle
         */
        const irr::core::recti& getJerseyTextRect();

        void computeSpeed(int frameNumber, int framerate, int animFramerate, std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold);

        /**
         * Returns texture of the player to write the jersey number on it
         * @return render texture
         */
        irr::video::ITexture *getRenderTexture() const;

        /**
         * Adapts player's position and rotation to the frame time value
         * @param time frame time value
         */
        void setTime(float time);


    private:
        irr::video::ITexture* renderTexture;

        irr::core::recti jerseyTextRect;
        int jerseyNumber;
        irr::core::stringw jerseyText;
        int team;

        std::map < int, AnimState > animState;
        std::map < int, int > animFrame;
};

#endif // PLAYER_H
