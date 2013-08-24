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

using namespace irr;
using namespace irr::core;
using namespace irr::video;

/**
 * Represents a moving body on the court with its own trajectory and orientation
 */
class MovingBody
{
    public:
        MovingBody();

        virtual ~MovingBody();

        /**
         * Initializes the moving body. Call this method after having given all the positions
         * @param nameInit name to display on Irrlicht user interface
         * @param modelPath path to model
         * @param texturePath path to texture (or -none- if no texture)
         * @param scale scale of the model
         * @param trajColor color of the trajectory
         * @param frameNumber number of frames in the tracking video
         */
        virtual void init(const stringw& nameInit, const io::path& modelPath, const io::path& texturePath, float scale, const SColor& trajColor, int frameNumber);

        /**
         * Maps frame times to positions
         * @param time time corresponding to position
         * @param position corresponding to time
         */
        void mapTime(int time, vector3df position);

        /**
         * Smooths the speed using a n-points averager
         * @param frameNumber number of frames in the tracking video
         */
        void smoothSpeed(int frameNumber);

        /**
         * Computes speed values using positions
         * @param frameNumber frame number in the tracking video
         */
        void computeSpeed(int frameNumber, int framerate);

        /**
         * Returns a list of the last positions of the body, grouped by consecutive pair (move lines)
         * @param from starting index of the list
         * @param samples number of positions to add to the list
         * @return list of moves
         */
        std::vector< vector2d < vector3df > > lastMoves(int from, int samples);

        /**
         * Adapts the position of the model to the wanted time value
         * @param time wanted frame time
         */
        virtual void setTime(int time);

        /**
         * Returns the texture of the model
         * @return texture
         */
        ITexture* getTexture();


    protected:
        stringw name;

        IAnimatedMeshSceneNode* node;
        ITexture* texture;

        // Side nodes
        ITextSceneNode* textNode;
        ColorCurveNode* trajectoryNode;

        // Movement attributes
        std::map < int, vector3df > virtualTrajectory;
        std::map < int, vector3df > virtualSpeed;
        std::map < int, vector3df > realSpeed;
        std::map < int, float > rotationAngle;
};

#endif // MOVINGBODY_H
