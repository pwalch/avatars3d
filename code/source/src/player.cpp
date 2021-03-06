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

#include <iostream>
#include <irrlicht.h>
#include <vector>
#include <fstream>
#include "camerawindow.h"
#include "player.h"
#include "movingbody.h"
#include "science.h"
#include "engine.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;

Player::Player(const BodySettings& playerBodySettings,
               const PlayerSettings& playerSettings)
    : MovingBody(playerBodySettings)
{
    this->mPlayerSettings = playerSettings;

    auto driver = Engine::getInstance().getCameraWindow().getDriver();
    // Create render texture where we can write the jersey text
    mRenderTexture = driver->addRenderTargetTexture(mPlayerSettings.mTextureSize);
    mNode->setMaterialTexture(0, mRenderTexture);

    mJerseyText = "";
    mJerseyText += playerSettings.mJerseyNumber;
}

std::map<int, int> Player::computeAnimations(int from) const
{
    auto speed = getTimeToSpeed(from);

    // Deduce animation from real speed
    std::map < int, AnimationAction > frameAction;
    for(std::map<int, float>::const_iterator s = speed.begin(); s != speed.end(); ++s) {
        int index = s->first;
        float magnitude = s->second;
        if(magnitude < mPlayerSettings.mActions.at(AnimationAction::Walk).mThreshold) {
            frameAction[index] = AnimationAction::Stand;
        }
        else if(magnitude < mPlayerSettings.mActions.at(AnimationAction::Run).mThreshold) {
            frameAction[index] = AnimationAction::Walk;
        }
        else {
            frameAction[index] = AnimationAction::Run;
        }
    }

    // Compute video framerate and animation framerate to keep fluency
    float ratioFloat = ((float)Engine::getInstance().getSequenceSettings().mFramerate)
                            / ((float)mPlayerSettings.mAnimFramerate);
    int ratio = irr::core::ceil32(ratioFloat);

    // Initialize state and animation counters
    AnimationAction currentAction = frameAction.begin()->second;

//    // fcounts counts all frames
//    int fcount = 0;
//    // fanim takes account of the ratio and skips frames
//    int fanim = mPlayerSettings.mActions[currentAction].mBegin;

    int fcount = frameAction.begin()->first;
    int fanim = mTimeToAnimFrame.find(fcount - 1) != mTimeToAnimFrame.end() ?
                    mTimeToAnimFrame.at(fcount - 1) : mPlayerSettings.mActions.at(currentAction).mBegin;

    // Store the right animation frames
    std::map<int, int> timeToAnimFrame;
    for(std::map<int, AnimationAction>::iterator a = frameAction.begin(); a != frameAction.end(); ++a) {
        int index = a->first;
        AnimationAction newAction = a->second;

        if(newAction == currentAction) {
            // If the action remains the same, we switch to next animation frame
            ++fcount;

            // If the current animation frame has been repeated sufficiently to keep fluency,
            // we switch to next animation frame
            if(fcount >= ratio) {
                fcount = 0;
                ++fanim;
            }

            // If we reach the end of the animation we go back to its beginning
            if(fanim > mPlayerSettings.mActions.at(currentAction).mEnd) {
                fcount = 0;
                fanim = mPlayerSettings.mActions.at(currentAction).mBegin;
            }
        } else {
            // If animation state changes, we go to the beginning of new state
            fcount = 0;
            fanim = mPlayerSettings.mActions.at(newAction).mBegin;
        }

        currentAction = newAction;
        timeToAnimFrame[index] = fanim;
    }

    return timeToAnimFrame;
}

VectorSequence Player::computeRotations(int from) const
{
    VectorSequence rotations;
    auto timeToAngle = getTimeToAngle(from);
    for(std::map<int,float>::const_iterator i = timeToAngle.begin(); i != timeToAngle.end(); ++i) {
        rotations.set(i->first, vector3df(0, i->second + 180, 0));
    }
    return rotations;
}

ITexture* Player::getTexture() const
{
    return mTexture;
}

ITexture *Player::getRenderTexture() const
{
    return mRenderTexture;
}

void Player::setTime(int time)
{
    MovingBody::setTime(time);
    // Set the right animation
    mNode->setCurrentFrame(mTimeToAnimFrame[time]);
}

const PlayerSettings &Player::getPlayerSettings() const
{
    return mPlayerSettings;
}

void Player::updatePositions(const VectorSequence& positions)
{
    Moveable::updatePositions(positions);

    auto rotations = computeRotations(positions.getBegin());
    Moveable::updateRotations(rotations);

    auto timeToAnimationChunk = computeAnimations(positions.getBegin());
    mTimeToAnimFrame.insert(timeToAnimationChunk.begin(), timeToAnimationChunk.end());
}

const stringw &Player::getJerseyText() const
{
    return mJerseyText;
}
