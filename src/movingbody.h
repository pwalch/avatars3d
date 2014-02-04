/*
  * 3D Avatars
  * Pierre Walch
  */


#ifndef MOVINGBODYNODE_H
#define MOVINGBODYNODE_H

#include "moveable.h"
#include "bodysettings.h"

/**
 * @brief Concrete moving body with color curve
 *
 * Moveable sub-class with a 3D model and a color curve.
 */
class MovingBody : public Moveable
{

public:

    /**
     * Initializes the 3D model and its animation
     * @param movingBodySettings body settings
     */
    MovingBody(const BodySettings& movingBodySettings);

    /**
     * Changes visibility of 3D model according to trajectory data, to handle time indexes missing a position.
     * Also changes visibility of trajectory color curve node according to MovingBodySettings.
     * @param time time index
     */
    void setTime(int time);

protected:

    /**
     * 3D model node
     */
    IAnimatedMeshSceneNode* mNode;

    /**
     * Texture applied on 3D model
     */
    ITexture* mTexture;

private:

    /**
     * Returns a list of the last positions of the body, grouped by consecutive pair (move lines)
     * @param from starting index of the list
     * @param samples number of positions to add to the list
     * @return list of moves
     */
    std::vector< std::pair<vector3df, vector3df > > lastMoves(int from, int samples);

    BodySettings mMovingBodySettings;

    ColorCurveNode* mColorCurveNode;

    // Irrlicht 3D text node snippet
//    stringw name;
//    ITextSceneNode* textNode;

};

#endif // MOVINGBODYNODE_H
