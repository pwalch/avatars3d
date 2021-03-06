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

#ifndef COLORCURVENODE_H
#define COLORCURVENODE_H

#include <vector>
#include <irrlicht.h>

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

/**
 * @brief Node representing 3D color curves
 *
 * Displays 3D color curves by drawing a fixed number of points on the wanted positions.
 */
class ColorCurveNode : public ISceneNode
{
public:

    /**
     * Sets the lines composing the curve
     * @param linePairs vector containing lines, which are actually 3D-point pairs
     */
    void setLines(const std::vector< std::pair<vector3df, vector3df > > & linePairs);

    /**
     * Creates color curve with the wanted color
     * @param trajColor color of the curve
     * @param parent parent node
     * @param mgr scene manager
     * @param id node ID
     */
    ColorCurveNode(const SColor& trajColor, ISceneNode* parent, ISceneManager* mgr, s32 id = 0);


    /**
     * Mandatory method for custom scene node
     */
    virtual void OnRegisterSceneNode();

    /**
     * Mandatory method for custom scene node
     */
    virtual void render();

    /**
     * Mandatory method for custom scene node
     */
    virtual const aabbox3d<f32>& getBoundingBox() const;

    /**
     * Mandatory method for custom scene node
     */
    u32 getMaterialCount() const;

    /**
     * Mandatory method for custom scene node
     */
    virtual SMaterial& getMaterial(u32 i);

private:

    std::vector< std::pair<vector3df, vector3df > > mLines;
    SColor mColor;

    // Mandatory members for custom scene node
    aabbox3d<f32> mBox;
    SMaterial mMaterial;

};

#endif // COLORCURVENODE_H
