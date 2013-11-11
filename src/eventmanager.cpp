/**
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <irrlicht.h>
#include "camerawindow.h"
#include "engine.h"

using namespace irr;

bool EventManager::OnEvent(const SEvent& event) {
    if(event.EventType == EET_KEY_INPUT_EVENT)
    {
        //CameraWindow& cam = CameraWindow::getInstance();
        if(event.KeyInput.Key == KEY_ESCAPE) {
            Engine& e = Engine::getInstance();
            e.stopRecording();
            return true;
        }
    }

    return false;
}
