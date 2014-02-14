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

#include <iostream>
#include <irrlicht.h>
#include "camerawindow.h"
#include "engine.h"

using namespace irr;

bool EventManager::OnEvent(const SEvent& event) {
    if(event.EventType == EET_KEY_INPUT_EVENT)
    {
        if(event.KeyInput.Key == KEY_ESCAPE) {
            Engine& e = Engine::getInstance();
            e.stopRecording();
            return true;
        }
    }

    return false;
}
