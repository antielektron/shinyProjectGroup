#ifndef UNNAMED_PROJECT_KEYMANAGER_H
#define UNNAMED_PROJECT_KEYMANAGER_H

#include <map>


class KeyManager
{
public:
	KeyManager();
	~KeyManager();
	void pressKey(int keycode);
	void releaseKey(int keycode);
	bool isKeyPressed(int keycode);
	void mouseMove(float rotX, float rotY);
	void tick();
	float getRelativeX();
	float getRelativeY();

protected:
	std::map<int, bool> m_keyMap;
	float m_currentPosX = 0;
	float m_currentPosY = 0;
	float relativeX = 0;
	float relativeY = 0;

};

#endif //UNNAMED_PROJECT_KEYMANAGER
