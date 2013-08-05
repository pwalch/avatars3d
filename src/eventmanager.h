#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <irrlicht.h>

/**
 * Irrlicht event manager handling mouse and keyboard events in Irrlicht window
 */
class EventManager : public irr::IEventReceiver {

    public:
        virtual bool OnEvent(const irr::SEvent& event);
};

#endif // EVENTMANAGER_H
