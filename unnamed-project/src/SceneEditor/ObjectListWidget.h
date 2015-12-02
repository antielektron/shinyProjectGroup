#ifndef UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H

#include <memory>

#include <QWidget>
#include <QTreeView>
#include <QBoxLayout>
#include <QPushButton>

class SceneEditorGame;

class ObjectListWidget : public QWidget
{
    Q_OBJECT
public:
    ObjectListWidget(std::shared_ptr<SceneEditorGame>, QWidget *parent = nullptr);
    virtual ~ObjectListWidget();

private:
    QTreeView *m_treeView;

    QPushButton *m_add;
    QPushButton *m_remove;

    // TODO buttons for adding/removing and moving objects!

    std::shared_ptr<SceneEditorGame> m_game;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H
