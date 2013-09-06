/**
  * 3D Avatars
  * Pierre Walch
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
         * @param linesVal vector containing lines, which are actually 3D-point pairs
         */
        void setLines(const std::vector< vector2d < vector3df > >& linesVal);

        /**
         * Creates color curve with the wanted color
         * @param trajColor color of the curve
         * @param parent parent node
         * @param mgr scene manager
         * @param id node ID
         */
        ColorCurveNode(const SColor& trajColor, ISceneNode* parent, ISceneManager* mgr, s32 id = 0);

        // Mandatory methods for custom scene node
        virtual void OnRegisterSceneNode();
        virtual void render();
        virtual const aabbox3d<f32>& getBoundingBox() const;
        u32 getMaterialCount() const;
        virtual SMaterial& getMaterial(u32 i);

    private:     

        std::vector< vector2d < vector3df > > lines;
        SColor color;

        // Mandatory methods for custom scene node
        aabbox3d<f32> Box;
        SMaterial material;

};

#endif // COLORCURVENODE_H
