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

using namespace irr;
using namespace irr::core;
using namespace irr::video;

/**
 * Enum value to distinguish players of the teams from other people of the tracking file
 */
enum { NOT_A_PLAYER = -100 };

/**
 * Different animations
 */
enum AnimState{ ANIMATION_STAND, ANIMATION_WALK, ANIMATION_RUN, ANIMATION_JUMP };

/**
 * Represents a sport player. mapTime(), setTeam() and init() must be called in this order before using other methods.
 */
class Player : public MovingBody
{
    public:
        /**
         * Creates a person corresponding to no team.
         */
        Player();

        /**
         * Initializes the player
         * @param trajColor color of trajectory curve
         * @param frameNumber number of frames in the tracking video
         * @param framerate framerate of tracking video
         * @param name name of the player
         * @param modelPath path to model
         * @param texturePath path to texture
         * @param scale model scale
         * @param textureSize texture dimensions
         * @param jerseyTextRectInit rectangle where to put the jersey text on the texture
         * @param trajColor color of trajectory curve
         * @param animFramerate framerate of model animation
         * @param stateDates list of beginFrame-endFrame pairs corresponding to each animation
         * @param stateThreshold list of threshold for animation changing
         */
        void init(bool trajVisible, const SColor& trajColor, int frameNumber, int framerate,
                  stringw name, const io::path& modelPath, const io::path& texturePath, float scale,
                  const dimension2d<u32>& textureSize, const recti& jerseyTextRectInit,
                  int animFramerate, const std::map<AnimState, vector2di>& stateDates,
                  const std::map<AnimState, float>& stateThreshold);

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
        const stringw& getJerseyText() const;

        /**
         * Returns rectangle corresponding to where the jersey text must be written
         * @return jersey text rectangle
         */
        const recti& getJerseyTextRect();

        /**
         * Computes speed and finds right animation
         * @param frameNumber number of frames in the tracking video
         * @param framerate framerate of the tracking video
         * @param animFramerate framerate of the animation
         * @param stateDates list of beginFrame-endFrame pairs corresponding to each animation
         * @param stateThreshold list of threshold for animation changing
         */
        void process(int frameNumber, int framerate, int animFramerate, std::map<AnimState, vector2di> stateDates, std::map<AnimState, float> stateThreshold);

        /**
         * Returns texture of the player to write the jersey number on it
         * @return render texture
         */
        ITexture *getRenderTexture() const;

        void setTime(float time);


    private:
        ITexture* renderTexture;

        // Jersey attributes
        int team;
        recti jerseyTextRect;
        int jerseyNumber;
        stringw jerseyText;

        // Movement attributes
        std::map < int, int > animFrame;
};

#endif // PLAYER_H
