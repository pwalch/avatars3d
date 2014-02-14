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

#include "vectorsequence.h"

VectorSequence::VectorSequence()
{

}

void VectorSequence::merge(const VectorSequence &sequence)
{
    mTimeToVector.insert(sequence.getBeginIterator(), sequence.getEndIterator());
//    for(int i = sequence.getBegin(); i <= sequence.getEnd(); ++i) {
//        set(i, sequence.get(i));
//    }
}

void VectorSequence::set(int time, vector3df vector)
{
    mTimeToVector[time] = vector;
}

const vector3df VectorSequence::get(int time) const
{
    if(time < 0) {
        return vector3df(0, 0, 0);
    }

    if(contains(time)) {
        return mTimeToVector.at(time);
    } else {
        return get(time - 1);
    }
}

int VectorSequence::getEnd() const
{
    int end = mTimeToVector.end()->first - 1;
    return end;
}

int VectorSequence::getBegin() const
{
    int begin = mTimeToVector.begin()->first;
    return begin;
}

bool VectorSequence::contains(int time) const
{
    return mTimeToVector.find(time) != mTimeToVector.end();
}

std::map<int, vector3df>::const_iterator VectorSequence::getBeginIterator() const
{
    return mTimeToVector.begin();
}

std::map<int, vector3df>::const_iterator VectorSequence::getEndIterator() const
{
    return mTimeToVector.end();
}
