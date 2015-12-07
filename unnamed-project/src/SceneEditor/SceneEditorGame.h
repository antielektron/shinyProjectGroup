#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_H

#include <memory>

#include "IGame.h"
#include "Scene/Scene.h"

class ObjectGroup;
class Object;

class SceneEditorGame : public QObject, public IGame
{
    Q_OBJECT
public:
    SceneEditorGame();
    virtual ~SceneEditorGame() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize();
    virtual void resize(int width, int height);
    virtual void tick();

    virtual Scene *getScene();

    virtual void setKeyManager(KeyManager *keyManager) {};

    void getModels(std::vector<Model *> &models);
    ObjectGroup *getRootObject();

    ObjectGroup *getCurrentObjectGroup();
    Object *getCurrentObject();

    void currentObjectModified();

Q_SIGNALS:
    void currentObjectChanged();
    void modelsChanged();

private:
    std::unique_ptr<Scene> m_scene;

    std::vector<Object *> m_objects;
    Object *m_dummyCurrentObject;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
