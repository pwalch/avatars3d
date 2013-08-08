#ifndef MOVINGBODY_H
#define MOVINGBODY_H

#include <irrlicht.h>
#include <map>
#include <vector>
#include "colorcurvenode.h"

class MovingBody
{
    public:
        MovingBody();

        virtual void init(irr::core::stringw nameInit, const irr::io::path& modelPath, const irr::io::path& texturePath, float scale);

        void mapTime(int time, irr::core::vector3df position);
        void smoothTrajectory(int frameNumber);
        void computeSpeed(int frameNumber, int framerate);
        std::vector< irr::core::vector2d < irr::core::vector3df > > lastMoves(int from, int samples);

        /**
         * Adapts the position of the model to the wanted time value
         * @param time new date
         */
        virtual void setTime(int time);
        irr::scene::IAnimatedMeshSceneNode *getNode() const;
        irr::video::ITexture* getTexture();


    protected:
        irr::core::stringw name;

        irr::scene::IAnimatedMeshSceneNode* node;
        irr::video::ITexture* texture;

        irr::scene::ITextSceneNode* textNode;
        ColorCurveNode* trajectoryNode;

        std::map < int, irr::core::vector3df > trajectory;
        std::map < int, irr::core::vector3df > speed;
        std::map < int, float > rotationAngle;
};

#endif // MOVINGBODY_H
