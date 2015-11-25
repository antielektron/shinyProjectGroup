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

protected:
	std::map<int, bool> m_keyMap;


};

#endif //UNNAMED_PROJECT_KEYMANAGER