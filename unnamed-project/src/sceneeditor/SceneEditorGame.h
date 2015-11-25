#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_H

#include <memory>

#include "IGame.h"
#include "Scene.h"

class SceneEditorGame : public IGame
{
public:
    virtual ~SceneEditorGame() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize();
    virtual void resize(int width, int height);
    virtual void tick();

    virtual Scene *getScene();
    virtual void onDoubleClick();
    virtual void onKeyEvent(int Key);

private:
    std::unique_ptr<Scene> m_scene;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
