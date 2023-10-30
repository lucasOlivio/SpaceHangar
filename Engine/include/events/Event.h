#pragma once

#include "iEvent.h"
#include <vector>

class iListener;

// Event subject
class Event : public iEvent
{
private:
     std::vector<iListener*> m_pListeners;
protected:
    Event() {};
public:
    virtual ~Event();

    // These methods allow listeners to subscribe/unsubscribe to this subject
    virtual void Attach(iListener* pListener);
    virtual void Dettach(iListener* pListener);

    // This method allows subjects to send events via the EventManager
    virtual void Notify(iEvent* pEvent);
};