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

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <irrlicht.h>

using namespace irr;

/**
 * @brief Irrlicht window event manager
 *
 * Handles mouse and keyboard events in Irrlicht window, particularly
 * to interrupt video recording with keyboard.
 */
class EventManager : public IEventReceiver {

public:
    /**
     * Listens to keyboard events. Calls Engine::stopRecording() if ESCAPE key is pressed.
     * @param event event to process
     * @return whether event has been processed
     */
    virtual bool OnEvent(const SEvent& event);
};

#endif // EVENTMANAGER_H
