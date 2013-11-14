/**
  * 3D Avatars
  * Pierre Walch
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
