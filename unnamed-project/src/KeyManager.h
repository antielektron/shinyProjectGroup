#ifndef UNNAMED_PROJECT_KEYMANAGER_H
#define UNNAMED_PROJECT_KEYMANAGER_H

#include <map>


class KeyManager
{
public:
	KeyManager();
	~KeyManager();

	void tick();

	void pressKey(int keycode);
	void releaseKey(int keycode);

	bool isKeyPressed(int keycode);

	void mouseMoveRelative(float posX, float posY);
    void mouseMoveAbsolute(float posX, float posY);

	float getRelativeX();
	float getRelativeY();

    float getAbsoluteX();
    float getAbsoluteY();

	void setCatchMouse(bool catchMouse);
	bool shouldCatchMouse();

protected:
	std::map<int, bool> m_keyMap;

	float m_absoluteX;
	float m_absoluteY;

	float m_relativeX;
	float m_relativeY;

    float m_currentRelativeX;
    float m_currentRelativeY;

	bool m_catchMouse;
};

#endif // UNNAMED_PROJECT_KEYMANAGER
