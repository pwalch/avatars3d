#ifndef VECTORSEQUENCE_H
#define VECTORSEQUENCE_H

#include <map>
#include <irrlicht.h>

using namespace irr::core;

class VectorSequence
{
public:
    VectorSequence();

    void merge(const VectorSequence& sequence);
    void set(int time, vector3df vector);


    const vector3df get(int time) const;
    int getEnd() const;
    int getBegin() const;

private:

    std::map<int,vector3df> mTimeToVector;
};

#endif // VECTORSEQUENCE_H
