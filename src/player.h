/*
 *  Copyright 2014 Pierre Walch
 *  Website : www.pwalch.net
 *
 *  Avatars is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Avatars is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
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
    ITexture* getTexture() const;

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
    virtual void setTime(int time) override;

    /**
     * Returns player settings
     * @return player settings
     */
    const PlayerSettings& getPlayerSettings() const;

    /**
     * Appends player trajectory chunk, computes corresponding animations and rotations
     * @param positions positions to append
     */
    void updatePositions(const VectorSequence& positions);


private:

    /**
     * Returns the mapping from time to 3D model frame index, from a start index
     * to the end of the sequence
     * @param from start index
     * @return map from time to 3D model frame index
     */
    std::map<int, int> computeAnimations(int from) const;

    /**
     * Returns sequence of rotations from a start index to the end of the sequence
     * @param from start index
     * @return rotation sequence
     */
    VectorSequence computeRotations(int from) const;

    PlayerSettings mPlayerSettings;
    ITexture* mRenderTexture;

    // Jersey attributes
    stringw mJerseyText;

    // Movement attributes
    std::map < int, int > mTimeToAnimFrame;
};

#endif // PLAYER_H
