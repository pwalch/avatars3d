/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef MOVINGBODYNODE_H
#define MOVINGBODYNODE_H

#include "moveable.h"
#include "bodysettings.h"

/**
 * @brief Represents a moving body having its mesh and texture
 *
 * This class is a sub-part of the model in MVC pattern. It is simply
 * a Moveable sub-class with a 3D model. mapTime() and init()
 * must be called in this order before using other methods.
 */
class MovingBody : public Moveable
{
    public:

        /**
         * Must be called before using the object. Call this method after
         * having filled the positions with mapTime()
         * @param moveableSettings
         * @param movingBodySettings
         */
        MovingBody(TrajectoryData* trajectoryData, const BodySettings& movingBodySettings);

        /**
         * Returns the texture of the model
         * @return texture
         */
        ITexture* getTexture();

        /**
         * Returns a list of the last positions of the body,
         * grouped by consecutive pair (move lines)
         * @param from starting index of the list
         * @param samples number of positions to add to the list
         * @return list of moves
         */
        std::vector< vector2d < vector3df > > lastMoves(int from, int samples);

        void setTime(int time);

    protected:
        BodySettings mMovingBodySettings;

        // GUI related nodes
//        stringw name;
//        ITextSceneNode* textNode;

        // Actual moving body node
        IAnimatedMeshSceneNode* node;
        ITexture* texture;

        // Color curve
        ColorCurveNode* mTrajectoryNode;

};

#endif // MOVINGBODYNODE_H
