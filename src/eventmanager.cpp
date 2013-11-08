/**
  * 3D Avatars
  * Pierre Walch
  */

#include <irrlicht.h>
#include "camerawindow.h"

using namespace irr;

bool EventManager::OnEvent(const SEvent& event) {
    if(event.EventType == EET_KEY_INPUT_EVENT)
    {
        //CameraWindow& cam = CameraWindow::getInstance();
        if(event.KeyInput.Key == KEY_ESCAPE) {
            return true;
        }
    }

    return false;
}
