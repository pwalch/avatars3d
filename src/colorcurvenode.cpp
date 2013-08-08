/**
  * 3D Avatars
  * Pierre Walch
  */

#include <irrlicht.h>
#include "colorcurvenode.h"

ColorCurveNode::ColorCurveNode(const irr::video::SColor& trajColor, irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id)
    : irr::scene::ISceneNode(parent, mgr, id)
{
    // Set curve color
    color = trajColor;

    material.Wireframe = false;
    material.Lighting = false;
    // Disable automatic culling to avoid calculating the bounding box
    setAutomaticCulling(irr::scene::EAC_OFF);
}

void ColorCurveNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void ColorCurveNode::render()
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

    driver->setMaterial(material);
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);

    // Draw each line composing the curve
    for(unsigned int l = 0; l < lines.size(); ++l) {
        const irr::core::vector2d < irr::core::vector3df >& singleLine = lines[l];
        driver->setMaterial(material);
        driver->draw3DLine(singleLine.X, singleLine.Y, color);
    }
}

const irr::core::aabbox3d<irr::f32>& ColorCurveNode::getBoundingBox() const
{
    return Box;
}

irr::u32 ColorCurveNode::getMaterialCount() const
{
    return 1;
}

irr::video::SMaterial& ColorCurveNode::getMaterial(irr::u32 i)
{
    return material;
}

void ColorCurveNode::setLines(const std::vector<irr::core::vector2d<irr::core::vector3df> >& linesVal)
{
    lines = linesVal;
}
