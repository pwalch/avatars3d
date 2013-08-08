#include <irrlicht.h>
#include "colorcurvenode.h"

ColorCurveNode::ColorCurveNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id)
    : irr::scene::ISceneNode(parent, mgr, id)
{
    material.Wireframe = false;
    material.Lighting = false;
    setAutomaticCulling(irr::scene::EAC_OFF);

    //vertices[1] = video::S3DVertex(10,0,-10, 1,0,0, irr::video::SColor(255,255,0,255), 1, 1);
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

    const irr::video::SColor lineColor(255, 229, 255, 0);
    for(int l = 0; l < lines.size(); ++l) {
        irr::core::vector2d < irr::core::vector3df > singleLine = lines[l];
        driver->setMaterial(material);
        driver->draw3DLine(singleLine.X, singleLine.Y, lineColor);
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

void ColorCurveNode::setLines(std::vector<irr::core::vector2d<irr::core::vector3df> > linesVal)
{
    lines = linesVal;
}
