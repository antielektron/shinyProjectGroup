#ifndef UNNAMED_PROJECT_IGAME_H
#define UNNAMED_PROJECT_IGAME_H

#include <memory>

class Scene;

class IGame
{
public:
    virtual ~IGame() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() = 0;
    virtual void resize(int width, int height) = 0;
    virtual void tick() = 0;

    virtual Scene *getScene() = 0;
	virtual void onDoubleClick() = 0;
	virtual void onKeyEvent(int Key) = 0;

    // TODO add interface for mouse movements and key strokes
};

#endif // UNNAMED_PROJECT_IGAME_H
