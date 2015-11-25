#ifndef UNNAMED_PROJECT_SCENE_EDITOR_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_WIDGET_H

#include <memory>

#include <QWidget>
#include <QListView>

#include <QHBoxLayout>

class OpenGLWidget;
class SceneEditorGame;
class ObjectDetailsWidget;

class SceneEditorWidget : public QWidget
{
    Q_OBJECT
public:
    SceneEditorWidget(QWidget *parent = 0);
    virtual ~SceneEditorWidget();

private:
    OpenGLWidget *m_glWidget;
    ObjectDetailsWidget *m_objectDetails;

    QListView *m_objectList;
    QHBoxLayout *m_layout;

    std::shared_ptr<SceneEditorGame> m_game;

};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_WIDGET_H
