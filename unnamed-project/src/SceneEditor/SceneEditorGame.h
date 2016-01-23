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
	SceneEditorGame(const QString &scenefile);
    virtual ~SceneEditorGame() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() override;
    virtual void resize(int width, int height) override;
    virtual void tick(float dt = 1.0f/60.0f) override;

    virtual Scene *getScene() override;
    GlobalState *getGlobalState();

	void reset(std::unique_ptr<Scene> scene);

	ObjectGroup *getRootObject();
    ObjectBase *getCurrentObject();


	void addModel(std::unique_ptr<Model> model);
    Model *getModelByName(const std::string &modelName);

	const std::string &getCurrentModel();
	void setCurrentModel(const std::string &modelName);
    void removeCurrentModel();


    Object *createObject(const std::string &modelName, ObjectGroup *parent = nullptr);
    ObjectGroup *createObjectGroup(const std::string &name, ObjectGroup *parent = nullptr);

	void notifyCurrentObjectChanged(ObjectBase *object);
	void removeCurrentObject();


	void addAttribute(const QString &key, const QVariant &value);
	void deleteAttribute(const QString &key);
	void notifyAttributeChanged();


	void addEvent(std::unique_ptr<Event> event);
	void deleteEvent(GlobalState::EventIterator iterator);
	void notifyEventChanged();


	void addAnimation(std::unique_ptr<AnimationBase> animator);
	void deleteAnimation(AnimationBase *animation);

public slots:
	// NOTE: running logic would require to stash the whole scene beforehand!! (does only make sense with player) instead launch bullet game.
    void runLogic();
    void stopLogic();
    void togglePauseLogic();

signals:
    void currentObjectChanged();
    void currentModelChanged();
    void objectsChanged();
    void modelsChanged();
	void sceneChanged(); // emited when "global information" has changed

    void attributesChanged();

    void eventsChanged();
    void eventsInvalidated(); // an event got deleted (needed for closing some widgets)

    void animatorsChanged();
private:
    void createIndicatorObject();

	// scene file only used for first initialization
	QString m_scenefile;
    std::unique_ptr<Scene> m_scene;

    float m_time;

    // if we have non constant framerates, this should
    // be set to the difference of the last 2 render times.
    float m_deltaTime;

    ObjectBase *m_currentObject;
	std::string m_currentModelName;

	QVector3D m_position;

	float m_rotY = 0;
	float m_rotX = 0;

	void updatePosMatrix(QVector3D deltaPos);

    //indicator stuff
    EditorObject *m_indicatorObject;

    float m_indicatorH;
    float m_extraIndicatorH;
    float m_indicatorRotation;
    float m_indicatorScale;

    // for live logic testing:
    bool m_logicRunning;
    bool m_logicPaused;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_H
