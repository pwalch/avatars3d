#include <iostream>
#include <irrlicht.h>
#include <vector>
#include "camerawindow.h"
#include "player.h"

Player::Player()
{
    setJerseyNumber(NOT_A_PLAYER);
}

Player::~Player()
{

}

void Player::mapTime(int time, irr::core::vector3df position)
{
    trajectory[time] = position;
}

void Player::init(const irr::io::path& modelPath, const irr::io::path& texturePath, const irr::core::dimension2d<irr::u32> textureSize, const irr::core::recti jerseyNumberRectInit, float scale)
{
    CameraWindow& cam = CameraWindow::getInstance();
    irr::scene::ISceneManager* sceneManager = cam.getSceneManager();
    irr::video::IVideoDriver* driver = cam.getDriver();

    // Load player model and apply texture
    node = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh(modelPath));
    node->setScale(irr::core::vector3df(scale, scale, scale));
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    texture = driver->getTexture(texturePath);

    renderTexture = driver->addRenderTargetTexture(textureSize);
    node->setMaterialTexture(0, renderTexture);

    jerseyNumberRect = jerseyNumberRectInit;

//    // Color the vertices
//    sceneManager->getMeshManipulator()->setVertexColors(node->getMesh(), irr::video::SColor(255, 0, 0, 255));

    textNode = sceneManager->addTextSceneNode(cam.getGuiFont(), jerseyText.c_str(), irr::video::SColor(255, 0, 255, 255), node);
    textNode->setVisible(false);

    // Initialize position
    setTime(0);

}

void Player::setTime(int time)
{
    if(trajectory.find(time) != trajectory.end())
    {
        node->setVisible(true);
        node->setPosition(trajectory[time]);
        irr::core::vector3df rotation = node->getRotation();
        node->setRotation(irr::core::vector3df(rotation.X, rotationAngle[time] + 180, rotation.Z));
        node->setCurrentFrame(animFrame[time]);
    }
    else
        node->setVisible(false);
}

std::map<int, irr::core::vector3df> Player::lastPositions(int from , int samples)
{
    for(int i = 0; i < samples; ++i) {

    }
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

irr::video::ITexture* Player::getTexture()
{
    return texture;
}


void Player::smoothTrajectory(int frameNumber)
{
    // Computing n-points average
    const int nbPoints = 15;
    std::map<int, irr::core::vector3df> smoothed;
    for(int f = nbPoints; f <= frameNumber; ++f) {
        irr::core::vector3df sum(0, 0, 0);
        for(int n = 1; n <= nbPoints; ++n) {
            sum += trajectory[f - n];
        }

        sum /= (float)nbPoints;
        smoothed[f] = sum;
    }

    // Applying the averager on the positions
    for(std::map<int, irr::core::vector3df>::iterator t = smoothed.begin(); t != smoothed.end(); ++t) {
        mapTime(t->first, t->second);
    }
}

void Player::computeSpeed(int frameNumber, int framerate, int animFramerate, std::map<AnimState, irr::core::vector2di> stateDates, std::map<AnimState, float> stateThreshold)
{
    // Choose an interval for trajectory derivative
    const int speedInterval = 20;
    // Compute all the speeds
    for(int f = speedInterval; f <= frameNumber; ++f) {
        speed[f] = (trajectory[f] - trajectory[f - speedInterval]) / speedInterval;
    }
    // Compute first speeds that were not computable before
    for(int f = 0; f < speedInterval; ++f)
        speed[f] = speed[speedInterval];

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

irr::core::recti Player::getJerseyNumberRect()
{
    return jerseyNumberRect;
}
