/*
 *  Copyright 2014 Pierre Walch
 *  Website : www.pwalch.net
 *
 *  Avatars is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Avatars is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
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

    /**
     * Returns true if the sequence contains the given time index
     * @param time frame index to verify
     * @return boolean
     */
    bool contains(int time) const;

    std::map<int, vector3df>::const_iterator getBeginIterator() const;
    std::map<int, vector3df>::const_iterator getEndIterator() const;

    std::map<int,vector3df> mTimeToVector;
};

#endif // VECTORSEQUENCE_H
