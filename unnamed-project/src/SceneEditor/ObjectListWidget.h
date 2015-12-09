#ifndef UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H

#include <memory>

#include <QWidget>
#include <QTreeView>
#include <QBoxLayout>
#include <QPushButton>
#include "Scene/ObjectGroup.h"

class SceneEditorGame;
class TreeModel;

class ObjectListWidget : public QWidget
{
    Q_OBJECT
public:
    ObjectListWidget(std::shared_ptr<SceneEditorGame>, QWidget *parent = nullptr);
    virtual ~ObjectListWidget();

public slots:

    void updateModelTree();

private:

    QTreeView *m_treeView;
    TreeModel *m_treeModel;

    QPushButton *m_add;
    QPushButton *m_remove;

    // TODO buttons for adding/removing and moving objects!

    std::shared_ptr<SceneEditorGame> m_game;

    //just a test:
    std::unique_ptr<ObjectGroup> g1;
    std::unique_ptr<ObjectGroup> g2;
    std::unique_ptr<ObjectGroup> g3;
    std::unique_ptr<ObjectGroup> g4;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H
