#include "events/KeyWrapper.h"

KeyEvent* KeyWrapper::m_pKeyEvent = nullptr;

void KeyWrapper::SetKeyEvent(KeyEvent* pKeyEvent)
{
	KeyWrapper::m_pKeyEvent = pKeyEvent;
}

void KeyWrapper::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!m_pKeyEvent)
	{
		// Event not configured
		return;
	}
	m_pKeyEvent->UpdateKey(key, scancode, action, mods);
}
