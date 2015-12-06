#include "KeyManager.h"
#include <iostream>

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

void KeyManager::tick()
{
	relativeX = m_currentPosX - m_oldPosX;
	relativeY = m_currentPosY - m_oldPosY;

	m_oldPosX = m_currentPosX;
	m_oldPosY = m_currentPosY;
	
	//std::cout << "X: " << m_oldPosX << "\n" << "Y: " << m_oldPosX << "\n";
	//std::cout << "X: " << relativeX << "\n" << "Y: " << relativeY << "\n";

}

float KeyManager::getRelativeX()
{
	return relativeX;
}

float KeyManager::getRelativeY()
{
	return relativeY;
}


void KeyManager::mouseMove(float posX, float posY)
{
	m_currentPosX = posX;
	m_currentPosY = posY;
	
}

