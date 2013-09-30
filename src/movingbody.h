#ifndef MOVINGBODYNODE_H
#define MOVINGBODYNODE_H

#include "moveable.h"
#include "movingbodysettings.h"

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
        void init(const MoveableSettings& moveableSettings,
                  const MovingBodySettings& movingBodySettings);

        virtual void setTime(int time);

        /**
         * Returns the texture of the model
         * @return texture
         */
        ITexture* getTexture();

    protected:
        MovingBodySettings movingBodySettings;

        // GUI related nodes
        stringw name;
        ITextSceneNode* textNode;

        // Actual moving body node
        IAnimatedMeshSceneNode* node;
        ITexture* texture;

};

#endif // MOVINGBODYNODE_H
