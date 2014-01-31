/*
  * 3D Avatars
  * Pierre Walch
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

Player::Player(TrajectoryData* trajectoryData,
               const BodySettings& playerBodySettings,
               const PlayerSettings& playerSettings)
    : MovingBody(trajectoryData, playerBodySettings)
{
    this->mPlayerSettings = playerSettings;

    processTrajectories();

    IVideoDriver* driver = Engine::getInstance().getCameraWindow()->getDriver();
    // Create render texture where we can write the jersey text
    mRenderTexture = driver->addRenderTargetTexture(mPlayerSettings.mTextureSize);
    mNode->setMaterialTexture(0, mRenderTexture);

    mJerseyText = "";
    mJerseyText += playerSettings.mJerseyNumber;
}

void Player::processTrajectories()
{
    Engine& engine = Engine::getInstance();
    int framerate = engine.getSequenceSettings().mFramerate;

    // Compute real speed and smooth it to compute animation
    std::map<int, vector3df> realPositions;
    AffineTransformation* tfm = engine.getTransformation();
    const std::map<int, vector3df> virtualPositions = mTrajectoryData->getVirtualPositions();
    for(std::map<int, vector3df>::const_iterator i = virtualPositions.begin();
        i != virtualPositions.end();
        ++i) {
        realPositions[i->first] = tfm->convertToReal(i->second);
    }

    std::map < int, vector3df > realSpeed = Science::computeSpeed(realPositions,
                                                                     mPlayerSettings.mSpeedInterval,
                                                                     framerate);
    realSpeed = Science::smooth(realSpeed, mPlayerSettings.mNbPointsAverager);

    // Deduce animation from real speed
    std::map < int, AnimationAction > frameAction;
    for(std::map<int, vector3df>::iterator s = realSpeed.begin(); s != realSpeed.end(); ++s) {
        int index = s->first;
        vector3df avSpeed = s->second;
        float magnitude = avSpeed.getLength();
        if(magnitude < mPlayerSettings.mActions[ANIMATION_WALK].mThreshold) {
            frameAction[index] = ANIMATION_STAND;
        }
        else if(magnitude < mPlayerSettings.mActions[ANIMATION_RUN].mThreshold) {
            frameAction[index] = ANIMATION_WALK;
        }
        else {
            frameAction[index] = ANIMATION_RUN;
        }
    }

    // Compute video framerate and animation framerate to keep fluency
    float ratioFloat = ((float)Engine::getInstance().getSequenceSettings().mFramerate)
                            / ((float)mPlayerSettings.mAnimFramerate);
    int ratio = irr::core::ceil32(ratioFloat);

    // Initialize state and animation counters
    AnimationAction currentAction = frameAction.begin()->second;

    // fcounts counts all frames
    int fcount = 0;
    // fanim takes account of the ratio and skips frames
    int fanim = mPlayerSettings.mActions[currentAction].mBegin;

    // Store the right animation frames
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
            if(fanim > mPlayerSettings.mActions[currentAction].mEnd) {
                fcount = 0;
                fanim = mPlayerSettings.mActions[currentAction].mBegin;
            }
        } else {
            // If animation state changes, we go to the beginning of new state
            fcount = 0;
            fanim = mPlayerSettings.mActions[newAction].mBegin;
        }

        currentAction = newAction;
        mTimeToAnimFrame[index] = fanim;
    }
}

ITexture* Player::getTexture()
{
    return mTexture;
}

ITexture *Player::getRenderTexture() const
{
    return mRenderTexture;
}

void Player::setTime(float time)
{
    MovingBody::setTime(time);
    // Set the right animation
    mNode->setCurrentFrame(mTimeToAnimFrame[time]);
}

const PlayerSettings &Player::getPlayerSettings() const
{
    return mPlayerSettings;
}

const stringw &Player::getJerseyText() const
{
    return mJerseyText;
}
