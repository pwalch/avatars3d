#include <iostream>
#include <irrlicht.h>
#include <vector>
#include "camerawindow.h"
#include "player.h"
#include "movingbody.h"

Player::Player()
{
    setJerseyNumber(NOT_A_PLAYER);
}

Player::~Player()
{

}

void Player::init(irr::core::stringw name, const irr::io::path& modelPath, const irr::io::path& texturePath, float scale, const irr::core::dimension2d<irr::u32> textureSize, const irr::core::recti jerseyNumberRectInit)
{
    MovingBody::init(name, modelPath, texturePath, scale);

    irr::video::IVideoDriver* driver = CameraWindow::getInstance().getDriver();
    renderTexture = driver->addRenderTargetTexture(textureSize);
    node->setMaterialTexture(0, renderTexture);

    jerseyNumberRect = jerseyNumberRectInit;
}

void Player::computeSpeed(int frameNumber, int framerate, int animFramerate, std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold)
{
    MovingBody::computeSpeed(frameNumber, framerate);

    // Deduce animation and angle from speed
    for(std::map<int, irr::core::vector3df>::iterator s = speed.begin(); s != speed.end(); ++s) {
        int index = s->first;
        irr::core::vector3df avSpeed = s->second;
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

irr::video::ITexture *Player::getRenderTexture() const
{
    return renderTexture;
}

void Player::setTime(float time)
{
    MovingBody::setTime(time);
    node->setCurrentFrame(animFrame[time]);
}

irr::core::recti Player::getJerseyNumberRect()
{
    return jerseyNumberRect;
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

const irr::core::stringw &Player::getJerseyText() const
{
    return jerseyText;
}
