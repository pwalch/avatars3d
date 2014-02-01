/*
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
 * @brief Sports player
 *
 * Subclass of MovingBody with supplementary settings for jersey (~= texture) customization
 */
class Player : public MovingBody
{
    public:

        /**
         * Creates render texture and extracts animation from trajectories by computing speed
         * @param movingBodySettings body settings
         * @param playerSettings player settings
         */
        Player(const BodySettings& movingBodySettings,
               const PlayerSettings& playerSettings);

        /**
         * Returns the texture of the player without the jersey number
         * @see CameraWindow::updateScene()
         * @return texture player texture
         */
        ITexture* getTexture();

        /**
         * Returns texture of the player to write the jersey number on it
         * @see CameraWindow::updateScene()
         * @return render texture
         */
        ITexture* getRenderTexture() const;

        /**
         * Returns text displayed on the player jersey
         * @return jersey text
         */
        const stringw& getJerseyText() const;

        /**
         * Moves player to its position and changes model animation frame according to the values processed at
         * instanciation.
         * @see processTrajectories()
         * @param time time index
         */
        virtual void setTime(float time);

        /**
         * Returns player settings
         * @return player settings
         */
        const PlayerSettings& getPlayerSettings() const;

        /**
         * Appends player trajectory chunk and computes corresponding animations
         * @param chunk chunk to append
         */
        void updateWith(TrajectoryChunk *chunk);


    private:

        /**
         * Computes speed and finds the corresponding animation for each frame
         */
        std::map<int, int> computeAnimations(TrajectoryChunk* chunk);

        PlayerSettings mPlayerSettings;
        ITexture* mRenderTexture;

        // Jersey attributes
        stringw mJerseyText;

        // Movement attributes
        std::map < int, int > mTimeToAnimFrame;
};

#endif // PLAYER_H
