#pragma once

#include "iListener.h"
#include <string>

// Event subject
class iEvent
{
public:
    virtual ~iEvent() {};

    // These methods allow listeners to subscribe/unsubscribe to this subject
    virtual void Attach(iListener* pListener) = 0;
    virtual void Dettach(iListener* pListener) = 0;
    // This method allows subjects to send events via the EventManager
    virtual void Notify(iEvent* pEvent) = 0;
};