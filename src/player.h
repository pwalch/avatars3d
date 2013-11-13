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
 * @brief Sports player
 *
 * Subclass of MovingBody with supplementary settings for jersey (~= texture) customization
 */
class Player : public MovingBody
{
    public:

        /**
         * Creates render texture and extracts animation from trajectories by computing speed
         * @param trajectoryData trajectory
         * @param movingBodySettings body settings
         * @param playerSettings player settings
         */
        Player(TrajectoryData* trajectoryData,
               const BodySettings& movingBodySettings,
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
        ITexture *getRenderTexture() const;

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


    private:

        /**
         * Computes speed and finds corresponding animation for each frame, stores the result in mFrameToAnim
         */
        void processTrajectories();

        PlayerSettings mPlayerSettings;
        ITexture* mRenderTexture;

        // Jersey attributes
        stringw mJerseyText;

        // Movement attributes
        std::map < int, int > mFrameToAnim;
};

#endif // PLAYER_H
