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

	bool isKeyDown(int keycode);
	// check if key was released/pressed during last frame
	bool isKeyPressed(int keycode);
	bool isKeyReleaseed(int keycode);

	void mouseMoveRelative(float posX, float posY);
    void mouseMoveAbsolute(float posX, float posY);

	float getRelativeX();
	float getRelativeY();

    float getAbsoluteX();
    float getAbsoluteY();

	void setCatchMouse(bool catchMouse);
	bool shouldCatchMouse();

protected:
    enum KeyState
    {
        IS_DOWN = 1,
        WAS_DOWN = 2,
        PRESSED = 4,
        RELEASED = 8
    };
	std::map<int, int> m_keyMap;

	float m_absoluteX;
	float m_absoluteY;

	float m_relativeX;
	float m_relativeY;

    float m_currentRelativeX;
    float m_currentRelativeY;

	bool m_catchMouse;
};

#endif // UNNAMED_PROJECT_KEYMANAGER
