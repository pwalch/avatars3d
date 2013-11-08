/**
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
#include "engine.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;

Player::Player()
{
    setJerseyNumber(NOT_A_PLAYER);
}

void Player::init(const MoveableSettings& moveableSettings,
                  const MovingBodySettings& movingBodySettings,
                  const PlayerSettings& playerSettings)
{
    MovingBody::init(moveableSettings, movingBodySettings);
    this->mPlayerSettings = playerSettings;

    process();

    IVideoDriver* driver = CameraWindow::getInstance().getDriver();
    // Create render texture where we can write the jersey text
    mRenderTexture = driver->addRenderTargetTexture(mPlayerSettings.mTextureSize);
    node->setMaterialTexture(0, mRenderTexture);
}

void Player::process()
{    
    // Compute virtual speed
    std::map < int, vector3df > virtualSpeed
            = computeSpeed(mVirtualTrajectory, mPlayerSettings.mSpeedInterval);
    smooth(virtualSpeed, mPlayerSettings.mNbPointsAverager);

    // Deduce angle from virtual speed
    for(std::map<int, vector3df>::iterator t = virtualSpeed.begin();
            t != virtualSpeed.end(); ++t) {
        int index = t->first;
        vector3df avSpeed = t->second;
        float angle = avSpeed.getHorizontalAngle().Y;
        mRotationAngle[index] = vector3df(0, angle + 180, 0);
    }

    // Compute real speed
    std::map < int, vector3df > realTrajectory;
    for(std::map<int, vector3df>::iterator f = mVirtualTrajectory.begin();
            f != mVirtualTrajectory.end();
            ++f) {
        realTrajectory[f->first] = Engine::getInstance().getTransformation()
                            ->convertToReal(mVirtualTrajectory[f->first]);
    }
    std::map < int, vector3df > realSpeed =
            MovingBody::computeSpeed(realTrajectory,
                                     mPlayerSettings.mSpeedInterval);
    MovingBody::smooth(realSpeed, mPlayerSettings.mNbPointsAverager);

    // Deduce animation from real speed
    std::map < int, AnimationAction > frameAction;
    for(std::map<int, vector3df>::iterator s = realSpeed.begin();
            s != realSpeed.end();
            ++s) {
        int index = s->first;
        vector3df avSpeed = s->second;
        float magnitude = avSpeed.getLength();
        if(magnitude < mPlayerSettings.mActions[ANIMATION_WALK].mThreshold)
            frameAction[index] = ANIMATION_STAND;
        else if(magnitude < mPlayerSettings.mActions[ANIMATION_RUN].mThreshold)
            frameAction[index] = ANIMATION_WALK;
        else
            frameAction[index] = ANIMATION_RUN;
    }

    // Compute video framerate and animation framerate to keep fluency
    float ratioFloat
        = ((float)Engine::getInstance().getSequenceSettings().mFramerate)
            / ((float)mPlayerSettings.mAnimFramerate);
    int ratio = irr::core::ceil32(ratioFloat);

    // Initialize state and animation counters
    AnimationAction currentAction = frameAction.begin()->second;
    int fcount = 0;
    int fanim = mPlayerSettings.mActions[currentAction].mBegin;

    // Store the right animation frames
    for(std::map<int, AnimationAction>::iterator a = frameAction.begin();
            a != frameAction.end();
            ++a) {
        int index = a->first;
        AnimationAction newAction = a->second;

        if(newAction == currentAction) {
            // Switch to next animation frame
            ++fcount;

            // If the current animation frame has been repeated
            // sufficiently to keep fluency we switch to next animation frame
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
        mAnimFrame[index] = fanim;
    }
}

ITexture *Player::getRenderTexture() const
{
    return mRenderTexture;
}

void Player::setTime(float time)
{
    MovingBody::setTime(time);
    // Set the right animation
    node->setCurrentFrame(mAnimFrame[time]);
}

const PlayerSettings &Player::getPlayerSettings() const
{
    return mPlayerSettings;
}

void Player::setJerseyNumber(int number)
{
    mJerseyNumber = number;
    mJerseyText = "";
    mJerseyText += number;
}

int Player::getTeam() const
{
    return mTeam;
}

void Player::setTeam(int value)
{
    mTeam = value;
}

int Player::getJerseyNumber() const
{
    return mJerseyNumber;
}

const stringw &Player::getJerseyText() const
{
    return mJerseyText;
}
