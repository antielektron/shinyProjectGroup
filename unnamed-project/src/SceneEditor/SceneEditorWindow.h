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

class AttributesWidget;
class EventsWidget;
class AnimatorsWidget;

class GlobalState;

class SceneEditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    SceneEditorWindow(QWidget *parent = 0);
    SceneEditorWindow(const QString &scenefile, QWidget *parent = 0);
    virtual ~SceneEditorWindow();

    void makeGlWidgetCurrent();
    void doneGlWidgetCurrent();

protected slots:
    void loadScene();
    void saveScene();
    void newScene();

private:
    void generateWidgets();
    void connectStuff();
    void createActions();
    void createToolbar();

    //Toolbar stuff:
    QToolBar    *m_toolbar;
    QAction     *m_loadScene;
    QAction     *m_saveScene;
    QAction     *m_newScene;
    QAction     *m_play;
    QAction     *m_stop;
    QAction     *m_pause;

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
    AttributesWidget *m_attributeWidget;

    QDockWidget *m_eventWidgetDock;
    EventsWidget *m_eventsWidget;

    std::shared_ptr<SceneEditorGame> m_game;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_WINDOW_H
