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

#include "movingbody.h"
#include "camerawindow.h"
#include "engine.h"

MovingBody::MovingBody(const BodySettings& movingBodySettings) : Moveable()
{
    this->mMovingBodySettings = movingBodySettings;

    CameraWindow& cam = Engine::getInstance().getCameraWindow();
    auto driver = cam.getDriver();
    auto sceneManager = cam.getSceneManager();

    // Create virtualTrajectory color curve
    mColorCurveNode= new ColorCurveNode(movingBodySettings.mTrajColor,
                                        sceneManager->getRootSceneNode(),
                                        sceneManager);

    Engine& engine = Engine::getInstance();

    // Load player model and apply texture if necessary
    auto mesh = sceneManager->getMesh(movingBodySettings.mModelPath);
    if(mesh == nullptr) {
        stringw modelErrorMsg = "Mesh could not be loaded: ";
        modelErrorMsg += movingBodySettings.mModelPath;
        engine.throwError(modelErrorMsg);
    }

    mNode = sceneManager->addAnimatedMeshSceneNode(mesh);
    mNode->setScale(vector3df(movingBodySettings.mScale,
                             movingBodySettings.mScale,
                             movingBodySettings.mScale));
    mNode->setMaterialFlag(EMF_LIGHTING, false);

    // If texture name is "none" we don't apply a texture
    if(strcmp(movingBodySettings.mTexturePath.c_str(), "none") != 0) {
        mTexture = driver->getTexture(movingBodySettings.mTexturePath);

        if(mTexture == nullptr) {
            stringw textureErrorMsg = "Texture could not be loaded: ";
            textureErrorMsg += movingBodySettings.mTexturePath;
            engine.throwError(textureErrorMsg);
        }
        mNode->setMaterialTexture(0, mTexture);
    }

    // Set material settings
    mNode->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
    mNode->setMaterialFlag(EMF_FRONT_FACE_CULLING, false);

    mNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
    mNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
    mNode->setMaterialFlag(EMF_ANTI_ALIASING, true);
    mNode->setLoopMode(false);
    mNode->setAnimationSpeed(0);

    mNode->setVisible(movingBodySettings.mVisible);

    // Irrlicht 3D text node snippet
//    // Color the vertices
//    sceneManager->getMeshManipulator()->setVertexColors(node->getMesh(),
//            SColor(255, 0, 0, 255));

    // Add Irrlicht GUI text scene node containing the name of the body
//    textNode = sceneManager->addTextSceneNode(cam->getGuiFont(),
//                               name.c_str(),
//                               SColor(255, 0, 255, 255),
//                               node);
//    textNode->setVisible(false);
}

void MovingBody::setTime(int time)
{   
    // Displaying or hiding 3D model
    if(mMovingBodySettings.mVisible) {
        mNode->setVisible(true);
        mNode->setPosition(getPosition(time));
        mNode->setRotation(getRotation(time));
    } else {
        mNode->setVisible(false);
    }

    // Displaying or hiding trajectory color curve
    if(mMovingBodySettings.mTrajVisible) {
        mColorCurveNode->setLines(lastMoves(time, mMovingBodySettings.mTrajNbPoints));
        mColorCurveNode->setVisible(true);
    } else {
        mColorCurveNode->setVisible(false);
    }
}

std::vector< std::pair<vector3df, vector3df > > MovingBody::lastMoves(int from, int samples)
{
    std::vector< std::pair<vector3df, vector3df > > lines;
    for(int i = 0; i < samples; ++i) {
        int index = from - i;
        if(index >= 1) {
            // Find both ends of the line
            vector3df start = getPosition(index - 1);
            vector3df end = getPosition(index);
            // Add each position pair to the list
            std::pair<vector3df, vector3df > singleLine(start, end);
            lines.push_back(singleLine);
        }
    }

    return lines;
}

