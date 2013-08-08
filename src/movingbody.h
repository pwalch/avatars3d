/**
  * 3D Avatars
  * Pierre Walch
  */

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

        /**
         * Initializes the moving body. Call this method after having given all the positions
         * @param nameInit name to display on Irrlicht user interface
         * @param modelPath path to model
         * @param texturePath path to texture (or -none- if no texture)
         * @param scale scale of the model
         * @param trajColor color of the trajectory
         * @param frameNumber number of frames in the tracking video
         */
        virtual void init(const irr::core::stringw& nameInit, const irr::io::path& modelPath, const irr::io::path& texturePath, float scale, const irr::video::SColor& trajColor, int frameNumber);

        /**
         * Maps frame times to positions
         * @param time time corresponding to position
         * @param position corresponding to time
         */
        void mapTime(int time, irr::core::vector3df position);

        /**
         * Smooths the trajectory using numerical analysis
         * @param frameNumber number of frames in the tracking video
         */
        void smoothTrajectory(int frameNumber);

        /**
         * Computes speed values using positions
         * @param frameNumber frame number in the tracking video
         */
        void computeSpeed(int frameNumber);

        /**
         * Returns a list of the last positions of the body, grouped by consecutive pair (move lines)
         * @param from starting index of the list
         * @param samples number of positions to add to the list
         * @return
         */
        std::vector< irr::core::vector2d < irr::core::vector3df > > lastMoves(int from, int samples);

        /**
         * Adapts the position of the model to the wanted time value
         * @param time wanted frame time
         */
        virtual void setTime(int time);

        /**
         * Returns the texture of the model
         * @return texture
         */
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
