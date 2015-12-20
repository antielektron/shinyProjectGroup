#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_H

#include <memory>

#include "IGame.h"
#include "Scene/Scene.h"
#include "KeyManager.h"
#include "Scene/Model.h"

class ObjectBase;
class ObjectGroup;
class Object;
class EditorObject;

class SceneEditorGame : public QObject, public IGame
{
    Q_OBJECT
public:
    SceneEditorGame();
    virtual ~SceneEditorGame() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() override;
    virtual void resize(int width, int height) override;
    virtual void tick(float dt = 1.0f/60.0f) override;

    virtual Scene *getScene() override;

    virtual void setKeyManager(KeyManager *keyManager) override;

    virtual bool isInitialized() override;

	void reset(std::unique_ptr<Scene> scene);

	ObjectGroup *getRootObject();

    //wrapper stuff
    void getModels(std::vector<Model *> &models);

    Model *getModelByName(const std::string &modelName);

    void addModel(std::unique_ptr<Model> model);
    void removeModel(const std::string &modelName);

	void currentObjectModified(ObjectBase* object);

    Object *createObject(const std::string &modelName, ObjectGroup *parent = nullptr);
    ObjectGroup *createObjectGroup(const std::string &name, ObjectGroup *parent = nullptr);

public slots:
	void onCurrentObjectChanged(ObjectBase * object);



Q_SIGNALS:
    void currentObjectChanged();
    void modelsChanged();
    void objectsChanged();
    void sceneReloaded();

private:
    void createIndicatorObject();

    std::unique_ptr<Scene> m_scene;

    float m_time;

    // if we have non constant framerates, this should
    // be set to the difference of the last 2 render times.
    float m_deltaTime;

    ObjectBase *m_currentObject;

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

    //indicator stuff
    EditorObject *m_indicatorObject;

    float m_indicatorH;
    float m_extraIndicatorH;
    float m_indicatorRotation;
    float m_indicatorScale;


};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
