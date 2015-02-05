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

void Moveable::updatePositions(const VectorSequence &positionChunk)
{
    mPosition.merge(positionChunk);

    // Store virtual speed for angle
    storeSpeed(mPosition, positionChunk.getBegin(), mVirtualSpeed);
    storeSmoothed(mVirtualSpeed, positionChunk.getBegin(), mSmoothedVirtualSpeed);

    // Store real speed for speed float value
    storeRealPosition(positionChunk.getBegin());
    storeSpeed(mRealPosition, positionChunk.getBegin(), mRealSpeed);
    storeSmoothed(mRealSpeed, positionChunk.getBegin(), mSmoothedRealSpeed);

    mRealSpeed.get(0);
}

void Moveable::updateRotations(const VectorSequence &rotationChunk)
{
    mRotation.merge(rotationChunk);
}

std::map<int, float> Moveable::getTimeToSpeed(int from) const
{
    // Compute magnitude of speed vector
    std::map<int, float> timeToSpeed;
    for(int i = from; i <= mSmoothedRealSpeed.getEnd(); ++i) {
        timeToSpeed[i] = mSmoothedRealSpeed.get(i).getLength();
    }
    return timeToSpeed;
}

std::map<int, float> Moveable::getTimeToAngle(int from) const
{
    // Compute angle from speed vector
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
    auto tfm = Engine::getInstance().getAffineTransformation();
    for (int i = from; i <= mPosition.getEnd(); ++i) {
        mRealPosition.set(i, tfm.convertToReal(mPosition.get(i)));
    }
}

void Moveable::storeSpeed(const VectorSequence& positions, int from, VectorSequence& speeds)
{
    int derivativeInterval = Engine::getInstance().getSequenceSettings().mSpeedInterval;

    // Store uncomputable values
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

    // Store uncomputable values
    int lastNecessaryIndex = nbPointsAverager - 2;
    if(from <= lastNecessaryIndex) {
        for(int i = 0; i <= lastNecessaryIndex; ++i) {
            smoothed.set(i, vector3df(0, 0, 0));
        }
    }

    int begin = Science::max(from, nbPointsAverager - 1);
    for(int i = begin; i <= values.getEnd(); ++i) {
        vector3df sum(0, 0, 0);
        for(int j = 0; j < nbPointsAverager; ++j) {
            sum += values.get(i - j);
        }
        smoothed.set(i, sum / nbPointsAverager);
    }
}



