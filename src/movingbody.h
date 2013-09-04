#ifndef MOVINGBODYNODE_H
#define MOVINGBODYNODE_H

#include "moveable.h"

/**
 * Represents a moving body having its mesh and texture. mapTime() and init() must be called in this order before using other methods.
 */
class MovingBody : public Moveable
{
    public:

        /**
         * Must be called before using the object. Call this method after having filled the positions with mapTime()
         * @param trajColor color of the trajectory curve
         * @param frameNumber number of frames in tracking video
         * @param framerate framerate in tracking video
         * @param nameInit name of the object
         * @param modelPath path to mesh
         * @param texturePath path to texture
         * @param scale scale of the model
         */
        void init(bool trajVisible, const SColor& trajColor, int frameNumber, int framerate, const stringw& nameInit, const io::path& modelPath, const io::path& texturePath, float scale);

        virtual void setTime(int time);

        /**
         * Returns the texture of the model
         * @return texture
         */
        ITexture* getTexture();

    protected:
        // GUI related nodes
        stringw name;
        ITextSceneNode* textNode;

        // Actual moving body node
        IAnimatedMeshSceneNode* node;
        ITexture* texture;

};

#endif // MOVINGBODYNODE_H
