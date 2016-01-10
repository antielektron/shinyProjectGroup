#ifndef UNNAMED_PROJECT_SCENE_EDITOR_WINDOW_H
#define UNNAMED_PROJECT_SCENE_EDITOR_WINDOW_H

#include <memory>

#include <QMainWindow>
#include <QListView>
#include <QDockWidget>

#include <QHBoxLayout>
#include <QToolBar>

class OpenGLWidget;
class SceneEditorGame;

class ObjectDetailsWidget;
class ObjectListWidget;
class ModelListWidget;
class GlobalDetailsWidget;

class ObjectBase;
class Model;

class AttributeWidget;
class EventWidget;

class GlobalState;

class SceneEditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    SceneEditorWindow(QWidget *parent = 0);
    virtual ~SceneEditorWindow();
    void makeGlWidgetCurrent();
    void doneGlWidgetCurrent();
    Model *getCurrentModel();

protected slots:
    void loadScene();
    void saveScene();
    void newScene();
    void onCurrentModelChanged(QString);

    // will be called if the scene has to rebuild
    // it's vector of objects
    void onUpdateSceneObjectsRequest();

    void onSceneChanged();

signals:
    void globalStateModified(GlobalState *globalState);

private:
    void connectStuff();
    void createActions();
    void createToolbar();

    //Toolbar stuff:
    QToolBar    *m_toolbar;
    QAction     *m_loadScene;
    QAction     *m_saveScene;
    QAction     *m_newScene;

    OpenGLWidget *m_glWidget;

    QDockWidget *m_objectDetailsDock;
    ObjectDetailsWidget *m_objectDetails;

    QDockWidget *m_objectListDock;
    ObjectListWidget *m_objectList;

    QDockWidget *m_modelListDock;
    ModelListWidget *m_modelList;

    QDockWidget *m_globalDetailsDock;
    GlobalDetailsWidget *m_globalDetails;

    QDockWidget *m_attributeWidgetDock;
    AttributeWidget *m_attributeWidget;

    QDockWidget *m_eventWidgetDock;
    EventWidget *m_eventWidget;

    std::shared_ptr<SceneEditorGame> m_game;

    Model *m_currentModel;

};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_WINDOW_H
