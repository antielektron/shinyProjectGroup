#include "KeyManager.h"

KeyManager::KeyManager()
{

}

KeyManager::~KeyManager()
{

}

void KeyManager::pressKey(int keycode)
{
	m_keyMap[keycode] = true;
}

void KeyManager::releaseKey(int keycode)
{
	m_keyMap[keycode] = false;
}

bool KeyManager::isKeyPressed(int keycode)
{
	return m_keyMap[keycode];
}