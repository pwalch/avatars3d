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

#ifndef MOVINGBODYNODE_H
#define MOVINGBODYNODE_H

#include "moveable.h"
#include "bodysettings.h"

/**
 * @brief Concrete moving body with color curve
 *
 * Moveable sub-class with a 3D model and a color curve.
 */
class MovingBody : public Moveable
{

public:

    /**
     * Initializes the 3D model and its animation
     * @param movingBodySettings body settings
     */
    explicit MovingBody(const BodySettings& movingBodySettings);

    /**
     * Changes visibility of 3D model according to trajectory data, to handle time indexes missing a position.
     * Also changes visibility of trajectory color curve node according to MovingBodySettings.
     * @param time time index
     */
    virtual void setTime(int time) override;

protected:

    /**
     * 3D model node
     */
    IAnimatedMeshSceneNode* mNode;

    /**
     * Texture applied on 3D model
     */
    ITexture* mTexture;

private:

    /**
     * Returns a list of the last positions of the body, grouped by consecutive pair (move lines)
     * @param from starting index of the list
     * @param samples number of positions to add to the list
     * @return list of moves
     */
    std::vector< std::pair<vector3df, vector3df > > lastMoves(int from, int samples);

    BodySettings mMovingBodySettings;

    ColorCurveNode* mColorCurveNode;

    // Irrlicht 3D text node snippet
//    stringw name;
//    ITextSceneNode* textNode;

};

#endif // MOVINGBODYNODE_H
