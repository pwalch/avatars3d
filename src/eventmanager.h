/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <irrlicht.h>

using namespace irr;

/**
 * Irrlicht event manager handling mouse and keyboard events in Irrlicht window
 */
class EventManager : public IEventReceiver {

    public:
        virtual bool OnEvent(const SEvent& event);
};

#endif // EVENTMANAGER_H
