/**
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <irrlicht.h>
#include <vector>
#include "camerawindow.h"
#include "player.h"
#include "movingbody.h"

using namespace irr;
using namespace irr::core;
using namespace irr::video;

Player::Player()
{
    setJerseyNumber(NOT_A_PLAYER);
}

void Player::init(stringw name, const io::path& modelPath, const io::path& texturePath, float scale, const dimension2d<u32> textureSize, const recti jerseyTextRectInit, const SColor& trajColor, int frameNumber, int framerate, int animFramerate, std::map<AnimState, vector2di> stateDates, std::map<AnimState, float> stateThreshold)
{
    MovingBody::init(name, modelPath, texturePath, scale, trajColor, frameNumber);

    IVideoDriver* driver = CameraWindow::getInstance().getDriver();
    // Create render texture where we can write the jersey text
    renderTexture = driver->addRenderTargetTexture(textureSize);
    node->setMaterialTexture(0, renderTexture);

    jerseyTextRect = jerseyTextRectInit;

    computeSpeed(frameNumber, framerate, animFramerate, stateDates, stateThreshold);
}

void Player::computeSpeed(int frameNumber, int framerate, int animFramerate, std::map<AnimState, vector2di> stateDates, std::map<AnimState, float> stateThreshold)
{
    MovingBody::computeSpeed(frameNumber);

    // Deduce animation and angle from speed
    for(std::map<int, vector3df>::iterator s = speed.begin(); s != speed.end(); ++s) {
        int index = s->first;
        vector3df avSpeed = s->second;
        float magnitude = avSpeed.getLength();
        if(magnitude < stateThreshold[ANIMATION_WALK])
            animState[index] = ANIMATION_STAND;
        else if(magnitude < stateThreshold[ANIMATION_RUN])
            animState[index] = ANIMATION_WALK;
        else
            animState[index] = ANIMATION_RUN;

        float angle = avSpeed.getHorizontalAngle().Y;
        rotationAngle[index] = angle;
    }

    // Compute ratio between video framerate and animation framerate to keep fluency
    int animCountRatio = framerate / animFramerate;

    // Initialize state and animation counters
    AnimState currentState = animState.begin()->second;
    int fcount = 0, fanim = stateDates[currentState].X;

    // Store the right animation frames
    for(std::map<int, AnimState>::iterator a = animState.begin(); a != animState.end(); ++a) {
        int index = a->first;
        AnimState newState = a->second;

        if(newState == currentState) {
            ++fcount;
            if(fcount >= animCountRatio) {
                fcount = 0;
                ++fanim;
            }

            if(fanim > stateDates[currentState].Y) {
                fcount = 0;
                fanim = stateDates[currentState].X;
            }
        } else {
            fcount = 0;
            fanim = stateDates[newState].X;
        }

        currentState = newState;
        animFrame[index] = fanim;
    }
}

ITexture *Player::getRenderTexture() const
{
    return renderTexture;
}

void Player::setTime(float time)
{
    MovingBody::setTime(time);
    // Set the good animation
    node->setCurrentFrame(animFrame[time]);
}

const recti& Player::getJerseyTextRect()
{
    return jerseyTextRect;
}

void Player::setJerseyNumber(int number)
{
    jerseyNumber = number;
    jerseyText = "P";
    jerseyText += number;
}

int Player::getTeam() const
{
    return team;
}

void Player::setTeam(int value)
{
    team = value;
}

int Player::getJerseyNumber() const
{
    return jerseyNumber;
}

const stringw &Player::getJerseyText() const
{
    return jerseyText;
}
