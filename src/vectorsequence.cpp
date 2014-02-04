#include "vectorsequence.h"

VectorSequence::VectorSequence()
{
}

void VectorSequence::merge(const VectorSequence &sequence)
{
    for(int i = sequence.getBegin(); i <= sequence.getEnd(); ++i) {
        mTimeToVector[i] = sequence.get(i);
    }
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

    if(mTimeToVector.find(time) != mTimeToVector.end()) {
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
