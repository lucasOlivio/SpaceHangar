#pragma once

#include "KeyEvent.h"
#include "common/opengl.h"

// Wrapper to send callbacks to events from GLFW
class KeyWrapper
{
private:
	static KeyEvent* m_pKeyEvent;
public:
	static void SetKeyEvent(KeyEvent* pKeyEvent);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};