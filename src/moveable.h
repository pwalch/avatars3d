/*
  * 3D Avatars
  * Pierre Walch
  */

#ifndef MOVEABLE_H
#define MOVEABLE_H

#include <irrlicht.h>
#include "vectorsequence.h"
#include "colorcurvenode.h"
#include "timeable.h"
#include "moveable.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;


/**
 * @brief Abstract moveable object on the court with its own trajectory and orientation.
 *
 * Contains the actual trajectory data, and can be updated with new parts of sequence
 */
class Moveable : public Timeable
{

public:

    Moveable();
    virtual ~Moveable();

    virtual void updatePositions(const VectorSequence& positions);
    virtual void updateRotations(const VectorSequence& rotations);

    std::map<int, float> getTimeToSpeed(int from);
    std::map<int, float> getTimeToAngle(int from);

    const vector3df getPosition(int time) const;
    const vector3df getRotation(int time) const;

private:

    void storeRealPosition(int from);

    void storeSpeed(const VectorSequence& positions, int from, VectorSequence& speeds);
    void storeSmoothed(const VectorSequence& values, int from, VectorSequence& smoothed);

    VectorSequence mPosition;
    VectorSequence mRotation;

    VectorSequence mRealPosition;

    VectorSequence mVirtualSpeed;
    VectorSequence mRealSpeed;

    VectorSequence mSmoothedVirtualSpeed;
    VectorSequence mSmoothedRealSpeed;
};

#endif // MOVEABLE_H
