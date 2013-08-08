#ifndef COLORCURVENODE_H
#define COLORCURVENODE_H

#include <vector>
#include <irrlicht.h>

class ColorCurveNode : public irr::scene::ISceneNode
{
    public:
        ColorCurveNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id = 0);
        virtual void OnRegisterSceneNode();
        virtual void render();
        virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
        irr::u32 getMaterialCount() const;
        virtual irr::video::SMaterial& getMaterial(irr::u32 i);

        void setLines(std::vector< irr::core::vector2d < irr::core::vector3df > > linesVal);


    private:
        irr::core::aabbox3d<irr::f32> Box;
        std::vector< irr::core::vector2d < irr::core::vector3df > > lines;
        irr::video::SMaterial material;

};

#endif // COLORCURVENODE_H
