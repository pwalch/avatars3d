/*
 * 3D Avatars
 * Pierre Walch
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
