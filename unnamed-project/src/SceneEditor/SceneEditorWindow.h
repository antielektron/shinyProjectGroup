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

class ObjectBase;

class SceneEditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    SceneEditorWindow(QWidget *parent = 0);
    virtual ~SceneEditorWindow();
    void makeGlWidgetCurrent();
    void doneGlWidgetCurrent();

protected slots:
    void loadScene();
    void saveScene();
    void newScene();

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

    std::shared_ptr<SceneEditorGame> m_game;

};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_WINDOW_H
