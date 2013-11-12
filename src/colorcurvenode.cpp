/**
  * 3D Avatars
  * Pierre Walch
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
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void ColorCurveNode::render()
{
    IVideoDriver* driver = SceneManager->getVideoDriver();

    driver->setMaterial(mMaterial);
    driver->setTransform(ETS_WORLD, AbsoluteTransformation);

    // Draw each line composing the curve
    for(unsigned int l = 0; l < mLines.size(); ++l) {
        const vector2d < vector3df >& singleLine = mLines[l];
        driver->setMaterial(mMaterial);
        driver->draw3DLine(singleLine.X, singleLine.Y, mColor);
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

void ColorCurveNode::setLines(const std::vector< vector2d<vector3df> >& linesVal)
{
    mLines = linesVal;
}
