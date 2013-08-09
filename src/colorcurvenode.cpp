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
    color = trajColor;

    material.Wireframe = false;
    material.Lighting = false;
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

    driver->setMaterial(material);
    driver->setTransform(ETS_WORLD, AbsoluteTransformation);

    // Draw each line composing the curve
    for(unsigned int l = 0; l < lines.size(); ++l) {
        const vector2d < vector3df >& singleLine = lines[l];
        driver->setMaterial(material);
        driver->draw3DLine(singleLine.X, singleLine.Y, color);
    }
}

const aabbox3d<f32>& ColorCurveNode::getBoundingBox() const
{
    return Box;
}

u32 ColorCurveNode::getMaterialCount() const
{
    return 1;
}

SMaterial& ColorCurveNode::getMaterial(u32 i)
{
    return material;
}

void ColorCurveNode::setLines(const std::vector< vector2d<vector3df> >& linesVal)
{
    lines = linesVal;
}
