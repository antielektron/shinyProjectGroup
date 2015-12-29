#ifndef UNNAMED_PROJECT_IGAME_H
#define UNNAMED_PROJECT_IGAME_H

#include <memory>

class Scene;
class KeyManager;

// not only a plain interface anymore
class IGame
{
public:
    IGame();
    virtual ~IGame() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize();
    virtual void resize(int width, int height) = 0;
    virtual void tick(float dt = 1.0f/60.0f) = 0;

    virtual Scene *getScene() = 0;

	void setKeyManager(KeyManager *keyManager);
    bool isInitialized();

protected:
    bool m_initialized;
    KeyManager *m_keyManager;
};

#endif // UNNAMED_PROJECT_IGAME_H
