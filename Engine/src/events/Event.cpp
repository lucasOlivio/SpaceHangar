#include "events/Event.h"

Event::~Event()
{
}

void Event::Notify(iEvent* pEvent)
{
	// Notify all listeners that was a change
	for (iListener* listener : this->m_pListeners)
	{
		listener->Notify(this);
	}
}

void Event::Attach(iListener* pListener)
{
	this->m_pListeners.push_back(pListener);
}

void Event::Dettach(iListener* pListener)
{
	// Find the iterator to the element you want to remove
	auto it = std::find(this->m_pListeners.begin(), this->m_pListeners.end(), pListener);

	// Check if the element was found before erasing it
	if (it != this->m_pListeners.end()) {
		this->m_pListeners.erase(it); // Remove the element
	}

	return;
}
