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
class Moveable : public ITimeable
{

public:

    /**
     * Creates an empty object
     */
    Moveable();

    /**
     * Detroys object
     */
    virtual ~Moveable();

    /**
     * Updates positions with a new chunk
     * @param positionChunk chunk of positions to append
     */
    virtual void updatePositions(const VectorSequence& positionChunk);

    /**
     * Updates rotations with a new chunk
     * @param rotationChunk chunk of rotations to append
     */
    virtual void updateRotations(const VectorSequence& rotationChunk);

    /**
     * Returns a map of speeds (in m/s) starting from an index and finishing
     * at the end of the sequence
     * @param from start index
     * @return map from frame to speed
     */
    std::map<int, float> getTimeToSpeed(int from);

    /**
     * Returns a map of angles (in degrees) starting from an index and finishing
     * at the end of the sequence
     * @param from start index
     * @return map from frame to angle
     */
    std::map<int, float> getTimeToAngle(int from);

    /**
     * Returns position at given frame
     * @param time frame index from which to get the position
     * @return position vector
     */
    const vector3df getPosition(int time) const;

    /**
     * Returns position at given frame
     * @param time frame index from which to get the rotation
     * @return rotation vector
     */
    const vector3df getRotation(int time) const;

private:

    /**
     * Stores real position values in mRealPosition
     * from a start index to the end of the sequence
     * @param from start index
     */
    void storeRealPosition(int from);

    /**
     * Computes the derivative of given position sequence
     * (from a start index to the end of the sequence)
     * and stores the result in the given speed sequence
     * @param positions positions from which to compute speed
     * @param from start index
     * @param speeds vector where to store the speed
     */
    void storeSpeed(const VectorSequence& positions, int from, VectorSequence& speeds);

    /**
     * Computes the moving average of the given values
     * (from a start index to the end of the sequence) and stores
     * the result in the "smoothed" sequence
     * @param values values to smooth
     * @param from start index
     * @param smoothed sequence where to store the smoothed values
     */
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
