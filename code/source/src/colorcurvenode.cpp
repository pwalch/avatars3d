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

#include <irrlicht.h>
#include "colorcurvenode.h"


using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

ColorCurveNode::ColorCurveNode(const SColor& trajColor, ISceneNode* parent, ISceneManager* mgr, s32 id)
    : ISceneNode(parent, mgr, id)
{
    // Set curve color
    mColor = trajColor;

    mMaterial.Wireframe = false;
    mMaterial.Lighting = false;
    // Disable automatic culling to avoid calculating the bounding box
    setAutomaticCulling(EAC_OFF);
}

void ColorCurveNode::OnRegisterSceneNode()
{
    SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}

void ColorCurveNode::render()
{
    auto driver = SceneManager->getVideoDriver();

    driver->setMaterial(mMaterial);
    driver->setTransform(ETS_WORLD, AbsoluteTransformation);

    // Draw each line composing the curve
    for(unsigned int l = 0; l < mLines.size(); ++l) {
        const std::pair<vector3df, vector3df >& singleLine = mLines[l];
        driver->setMaterial(mMaterial);
        driver->draw3DLine(singleLine.first, singleLine.second, mColor);
    }
}

const aabbox3d<f32>& ColorCurveNode::getBoundingBox() const
{
    return mBox;
}

u32 ColorCurveNode::getMaterialCount() const
{
    return 1;
}

SMaterial& ColorCurveNode::getMaterial(u32 i)
{
    return mMaterial;
}

void ColorCurveNode::setLines(const std::vector< std::pair<vector3df, vector3df > >& linePairs)
{
    mLines = linePairs;
}
