#include "events/KeyEvent.h"

KeyEvent::KeyEvent()
{
}

KeyEvent::~KeyEvent()
{
}

void KeyEvent::UpdateKey(int key, int scancode, int action, int mods)
{
	this->m_sKeyInfo.pressedKey = key;
	this->m_sKeyInfo.scanCode = scancode;
	this->m_sKeyInfo.action = action;
	this->m_sKeyInfo.mods = mods;
	this->Event::Notify(this);
}

sKeyInfo KeyEvent::GetKeyInfo()
{
	return this->m_sKeyInfo;
}
