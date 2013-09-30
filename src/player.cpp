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
    this->playerSettings = playerSettings;

    process();

    IVideoDriver* driver = CameraWindow::getInstance().getDriver();
    // Create render texture where we can write the jersey text
    renderTexture = driver->addRenderTargetTexture(playerSettings.textureSize);
    node->setMaterialTexture(0, renderTexture);
}

void Player::process()
{    
    // Compute virtual speed
    std::map < int, vector3df > virtualSpeed
            = computeSpeed(virtualTrajectory, playerSettings.speedInterval);
    smooth(virtualSpeed, playerSettings.nbPointsAverager);

    // Deduce angle from virtual speed
    for(std::map<int, vector3df>::iterator t = virtualSpeed.begin();
            t != virtualSpeed.end(); ++t) {
        int index = t->first;
        vector3df avSpeed = t->second;
        float angle = avSpeed.getHorizontalAngle().Y;
        rotationAngle[index] = vector3df(0, angle + 180, 0);
    }

    // Compute real speed
    std::map < int, vector3df > realTrajectory;
    for(std::map<int, vector3df>::iterator f = virtualTrajectory.begin();
            f != virtualTrajectory.end();
            ++f) {
        realTrajectory[f->first] = Engine::getInstance().getTransformation()
                            ->convertToReal(virtualTrajectory[f->first]);
    }
    std::map < int, vector3df > realSpeed =
            MovingBody::computeSpeed(realTrajectory,
                                     playerSettings.speedInterval);
    MovingBody::smooth(realSpeed, playerSettings.nbPointsAverager);

    // Deduce animation from real speed
    std::map < int, AnimationAction > frameAction;
    for(std::map<int, vector3df>::iterator s = realSpeed.begin();
            s != realSpeed.end();
            ++s) {
        int index = s->first;
        vector3df avSpeed = s->second;
        float magnitude = avSpeed.getLength();
        if(magnitude < playerSettings.actions[ANIMATION_WALK].threshold)
            frameAction[index] = ANIMATION_STAND;
        else if(magnitude < playerSettings.actions[ANIMATION_RUN].threshold)
            frameAction[index] = ANIMATION_WALK;
        else
            frameAction[index] = ANIMATION_RUN;
    }

    // Compute video framerate and animation framerate to keep fluency
    float ratioFloat
        = ((float)Engine::getInstance().getSequenceSettings().framerate)
            / ((float)playerSettings.animFramerate);
    int ratio = irr::core::ceil32(ratioFloat);

    // Initialize state and animation counters
    AnimationAction currentAction = frameAction.begin()->second;
    int fcount = 0;
    int fanim = playerSettings.actions[currentAction].begin;

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
            if(fanim > playerSettings.actions[currentAction].end) {
                fcount = 0;
                fanim = playerSettings.actions[currentAction].begin;
            }
        } else {
            // If animation state changes, we go to the beginning of new state
            fcount = 0;
            fanim = playerSettings.actions[newAction].begin;
        }

        currentAction = newAction;
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

const PlayerSettings &Player::getPlayerSettings() const
{
    return playerSettings;
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
