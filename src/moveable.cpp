/*
  * 3D Avatars
  * Pierre Walch
  */

#include "engine.h"
#include "vectorsequence.h"
#include "science.h"
#include "moveable.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;


Moveable::Moveable()
{

}

Moveable::~Moveable()
{
}

void Moveable::updatePositions(const VectorSequence &positions)
{
    mPosition.merge(positions);

    storeSpeed(mPosition, positions.getBegin(), mVirtualSpeed);
    storeSmoothed(mVirtualSpeed, positions.getBegin(), mSmoothedVirtualSpeed);

    storeRealPosition(positions.getBegin());
    storeSpeed(mRealPosition, positions.getBegin(), mRealSpeed);
    storeSmoothed(mRealSpeed, positions.getBegin(), mSmoothedRealSpeed);
}

void Moveable::updateRotations(const VectorSequence &rotations)
{
    mRotation.merge(rotations);
}

std::map<int, float> Moveable::getTimeToSpeed(int from)
{
    std::map<int, float> timeToSpeed;
    for(int i = from; i <= mSmoothedRealSpeed.getEnd(); ++i) {
        timeToSpeed[i] = mSmoothedRealSpeed.get(i).getLength();
    }
    return timeToSpeed;
}

std::map<int, float> Moveable::getTimeToAngle(int from)
{
    std::map<int, float> timeToAngle;
    for(int i = from; i <= mSmoothedVirtualSpeed.getEnd(); ++i) {
        timeToAngle[i] = mSmoothedVirtualSpeed.get(i).getHorizontalAngle().Y;
    }

    return timeToAngle;
}

const vector3df Moveable::getPosition(int time) const
{
    return mPosition.get(time);
}

const vector3df Moveable::getRotation(int time) const
{
    return mRotation.get(time);
}

void Moveable::storeRealPosition(int from)
{
    AffineTransformation* tfm = Engine::getInstance().getTransformation();
    for (int i = from; i <= mPosition.getEnd(); ++i) {
        mRealPosition.set(i, tfm->convertToReal(mPosition.get(i)));
    }
}

void Moveable::storeSpeed(const VectorSequence& positions, int from, VectorSequence& speeds)
{
    int derivativeInterval = Engine::getInstance().getSequenceSettings().mSpeedInterval;

    int lastNecessaryIndex = derivativeInterval - 1;
    if(from <= lastNecessaryIndex) {
        for(int i = 0; i <= lastNecessaryIndex; ++i) {
            speeds.set(i, vector3df(0, 0, 0));
        }
    }


    int framerate = Engine::getInstance().getSequenceSettings().mFramerate;
    int begin = Science::max(from, derivativeInterval);
    for(int i = begin; i <= positions.getEnd(); ++i) {
        speeds.set(i, ((float)framerate) * (positions.get(i) - positions.get(i - derivativeInterval))
                                / derivativeInterval);
    }
}

void Moveable::storeSmoothed(const VectorSequence &values, int from, VectorSequence &smoothed)
{
    int nbPointsAverager = Engine::getInstance().getSequenceSettings().mNbPointsAverager;

    int begin = Science::max(from, nbPointsAverager - 1);
    for(int i = begin; i <= values.getEnd(); ++i) {
        vector3df sum(0, 0, 0);
        for(int j = 0; j < nbPointsAverager; ++j) {
            sum += values.get(i - j);
        }
        smoothed.set(i, sum);
    }
}



