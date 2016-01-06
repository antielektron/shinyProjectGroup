#include "KeyManager.h"
#include <iostream>

KeyManager::KeyManager() :
		m_absoluteX(0),
		m_absoluteY(0),
		m_relativeX(0),
		m_relativeY(0),
        m_currentRelativeX(0),
        m_currentRelativeY(0),
		m_catchMouse(false)
{

}

KeyManager::~KeyManager()
{

}

void KeyManager::pressKey(int keycode)
{
	// set flag
	m_keyMap[keycode] |= IS_DOWN;
}

void KeyManager::releaseKey(int keycode)
{
	// reset flag
	m_keyMap[keycode] &= ~IS_DOWN;
}

bool KeyManager::isKeyDown(int keycode)
{
	return (m_keyMap[keycode] & IS_DOWN) != 0;
}

bool KeyManager::isKeyPressed(int keycode)
{
	return (m_keyMap[keycode] & PRESSED) != 0;
}

bool KeyManager::isKeyReleaseed(int keycode)
{
    return (m_keyMap[keycode] & RELEASED) != 0;
}

void KeyManager::tick()
{
    m_relativeX = m_currentRelativeX;
    m_relativeY = m_currentRelativeY;

    m_currentRelativeX = 0;
    m_currentRelativeY = 0;

	// reset pressed/released state
	for (auto &entry : m_keyMap)
	{
        // update pressed flag
        if ((entry.second & IS_DOWN) != 0 && (entry.second & WAS_DOWN) == 0)
            entry.second |= PRESSED;
        else
            entry.second &= ~PRESSED;

        // update released flag
        if ((entry.second & WAS_DOWN) != 0 && (entry.second & IS_DOWN) == 0)
            entry.second |= RELEASED;
        else
            entry.second &= ~RELEASED;

        // update was down
		if ((entry.second & IS_DOWN) != 0)
			entry.second |= WAS_DOWN;
        else
            entry.second &= ~WAS_DOWN;
	}
}

float KeyManager::getRelativeX()
{
	return m_relativeX;
}

float KeyManager::getRelativeY()
{
	return m_relativeY;
}

float KeyManager::getAbsoluteX()
{
    return m_absoluteX;
}

float KeyManager::getAbsoluteY()
{
    return m_absoluteY;
}

void KeyManager::mouseMoveRelative(float posX, float posY)
{
    // Collect relative mouse movements
    m_currentRelativeX += posX;
    m_currentRelativeY += posY;
}

void KeyManager::mouseMoveAbsolute(float posX, float posY)
{
    // Collect relative mouse movements as well
    mouseMoveRelative(posX - m_absoluteX, posY - m_absoluteY);
    m_absoluteX = posX;
    m_absoluteY = posY;
}

void KeyManager::setCatchMouse(bool catchMouse)
{
	m_catchMouse = catchMouse;
}

bool KeyManager::shouldCatchMouse()
{
	return m_catchMouse;
}
