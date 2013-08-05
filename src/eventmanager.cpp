#include <irrlicht.h>
#include "camerawindow.h"

bool EventManager::OnEvent(const irr::SEvent& event) {
    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        CameraWindow& cam = CameraWindow::getInstance();
        if(event.KeyInput.Key == irr::KEY_ESCAPE) {
            return true;
        }
    }

    return false;
}
