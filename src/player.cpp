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

void Player::init(const SColor& trajColor, int frameNumber, int framerate, stringw name, const io::path& modelPath, const io::path& texturePath, float scale, const dimension2d<u32>& textureSize, const recti& jerseyTextRectInit, int animFramerate, const std::map<AnimState, vector2di>& stateDates, const std::map<AnimState, float>& stateThreshold)
{
    MovingBody::init(trajColor, frameNumber, framerate, name, modelPath, texturePath, scale);
    process(frameNumber, framerate, animFramerate, stateDates, stateThreshold);

    IVideoDriver* driver = CameraWindow::getInstance().getDriver();
    // Create render texture where we can write the jersey text
    renderTexture = driver->addRenderTargetTexture(textureSize);
    node->setMaterialTexture(0, renderTexture);

    jerseyTextRect = jerseyTextRectInit;
}

void Player::process(int frameNumber, int framerate, int animFramerate, std::map<AnimState, vector2di> stateDates, std::map<AnimState, float> stateThreshold)
{
    CameraWindow& cam = CameraWindow::getInstance();

    // Compute virtual speed
    std::map < int, vector3df > virtualSpeed = computeSpeed(virtualTrajectory, frameNumber, framerate);
    smooth(virtualSpeed, frameNumber);

    // Deduce angle from virtual speed
    for(std::map<int, vector3df>::iterator t = virtualSpeed.begin(); t != virtualSpeed.end(); ++t) {
        int index = t->first;
        vector3df avSpeed = t->second;
        float angle = avSpeed.getHorizontalAngle().Y;
        rotationAngle[index] = vector3df(0, angle + 180, 0);
    }

    // Compute real speed
    std::map < int, vector3df > realTrajectory;
    for(std::map<int, vector3df>::iterator f = virtualTrajectory.begin(); f != virtualTrajectory.end(); ++f) {
        realTrajectory[f->first] = cam.convertToReal(virtualTrajectory[f->first]);
    }
    std::map < int, vector3df > realSpeed = MovingBody::computeSpeed(realTrajectory, frameNumber, framerate);
    MovingBody::smooth(realSpeed, frameNumber);

    // Deduce animation from real speed
    std::map < int, AnimState > animState;
    for(std::map<int, vector3df>::iterator s = realSpeed.begin(); s != realSpeed.end(); ++s) {
        int index = s->first;
        vector3df avSpeed = s->second;
        float magnitude = avSpeed.getLength();
        if(magnitude < stateThreshold[ANIMATION_WALK])
            animState[index] = ANIMATION_STAND;
        else if(magnitude < stateThreshold[ANIMATION_RUN])
            animState[index] = ANIMATION_WALK;
        else
            animState[index] = ANIMATION_RUN;
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
    // Set the right animation
    node->setCurrentFrame(animFrame[time]);
}

const recti& Player::getJerseyTextRect()
{
    return jerseyTextRect;
}

void Player::setJerseyNumber(int number)
{
    jerseyNumber = number;
    jerseyText = "";
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
