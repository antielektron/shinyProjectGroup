#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_H

#include <memory>

#include "IGame.h"
#include "Scene/Scene.h"
#include "KeyManager.h"

class ObjectBase;
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

    virtual void setKeyManager(KeyManager *keyManager);

    virtual bool isInitialized();

    ObjectGroup *getRootObject();
    void getModels(std::vector<Model *> &models);

    ObjectGroup *getCurrentObjectGroup();
    Object *getCurrentObject();

    void currentObjectModified();

    void addModel(std::unique_ptr<Model> model);

    Object *createObject(const std::string &name);
    ObjectGroup *createObjectGroup(const std::string &name, ObjectGroup *parent = nullptr);

Q_SIGNALS:
    void currentObjectChanged();
    void modelsChanged();
    void objectsChanged();

private:
    std::unique_ptr<Scene> m_scene;

    std::vector<Object *> m_objects;

    Object *m_dummyCurrentObject;

	float rotY = 0;
	float rotX = 0;
	float pos_height = 0;


	QVector3D m_position;
	QVector3D m_centre;
	QMatrix4x4 posMatrix;

	KeyManager *m_keyManager;
	bool m_wasEscDown;
    bool m_initialized;

	void updatePosMatrix(QVector3D deltaPos);

};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
