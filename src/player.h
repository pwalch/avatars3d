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
#include "playersettings.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;


/**
 * @brief Represents a player
 *
 * This class is a sub-part of the model in MVC pattern.
 * It contains player team, jersey, and animation data. mapTime(), setTeam()
 * and init() must be called in this order before using other methods.
 */
class Player : public MovingBody
{
    public:

        /**
         * Initializes player
         * @param moveableSettings
         * @param movingBodySettings
         * @param playerSettings
         */
        Player(TrajectoryData* trajectoryData,
               const MoveableSettings& moveableSettings,
               const MovingBodySettings& movingBodySettings,
               const PlayerSettings& playerSettings);

        /**
         * Returns team ID of the player
         * @return team ID
         */
        int getTeam() const;

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
         * Computes speed and finds right animation
         */
        void process();

        /**
         * Returns texture of the player to write the jersey number on it
         * @return render texture
         */
        ITexture *getRenderTexture() const;

        /**
         * Adapts the position of the player to the wanted time value
         * @param time wanted frame time
         */
        virtual void setTime(float time);

        /**
         * Returns player settings
         * @return player settings
         */
        const PlayerSettings& getPlayerSettings() const;

    private:
        PlayerSettings mPlayerSettings;
        ITexture* mRenderTexture;

        // Jersey attributes
        int mTeam;
        stringw mJerseyText;

        // Movement attributes
        std::map < int, int > mAnimFrame;
};

#endif // PLAYER_H
