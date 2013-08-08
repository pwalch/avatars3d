/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef COLORCURVENODE_H
#define COLORCURVENODE_H

#include <vector>
#include <irrlicht.h>

/**
 * Node representing 3D color curves
 */
class ColorCurveNode : public irr::scene::ISceneNode
{
    public:

        /**
         * Sets the lines composing the curve
         * @param linesVal vector containing lines, which are actually 3D-point pairs
         */
        void setLines(const std::vector< irr::core::vector2d < irr::core::vector3df > >& linesVal);

        /**
         * Creates color curve with the wanted color
         * @param trajColor color of the curve
         */
        ColorCurveNode(const irr::video::SColor& trajColor, irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id = 0);

        // Mandatory methods for custom scene node
        virtual void OnRegisterSceneNode();
        virtual void render();
        virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
        irr::u32 getMaterialCount() const;
        virtual irr::video::SMaterial& getMaterial(irr::u32 i);

    private:     

        std::vector< irr::core::vector2d < irr::core::vector3df > > lines;
        irr::video::SColor color;

        // Mandatory methods for custom scene node
        irr::core::aabbox3d<irr::f32> Box;
        irr::video::SMaterial material;

};

#endif // COLORCURVENODE_H
