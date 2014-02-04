/*
 * 3D Avatars
 * Pierre Walch
 */

#ifndef VECTORSEQUENCE_H
#define VECTORSEQUENCE_H

#include <map>
#include <irrlicht.h>

using namespace irr::core;

/**
 * Represents a sequence of 3D vectors.
 * Facade for std::map<int, vector3df> to simplify the interface.
 */
class VectorSequence
{

public:
    /**
     * Creates an empty sequence
     */
    VectorSequence();

    /**
     * Merges the given sequence with the object
     * @param sequence sequence to merge
     */
    void merge(const VectorSequence& sequence);

    /**
     * Sets the value of the sequence at a given time
     * @param time frame index
     * @param vector 3D vector to set
     */
    void set(int time, vector3df vector);


    /**
     * Returns 3D vector at given time
     * @param time frame index
     * @return 3D vector
     */
    const vector3df get(int time) const;

    /**
     * Returns end of sequence
     * @return end index
     */
    int getEnd() const;

    /**
     * Returns beginning of sequence
     * @return begin index
     */
    int getBegin() const;

private:

    std::map<int,vector3df> mTimeToVector;
};

#endif // VECTORSEQUENCE_H
