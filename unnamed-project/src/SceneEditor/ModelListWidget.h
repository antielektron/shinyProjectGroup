#ifndef UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H

#include <memory>

#include <QWidget>
#include <QTreeWidget>
#include <QBoxLayout>
#include <QPushButton>
#include "Scene/ObjectGroup.h"

class SceneEditorGame;
class ObjectBase;
class TreeModel;

class ModelListWidget : public QWidget
{
    Q_OBJECT
public:
    ModelListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~ModelListWidget();

    ObjectBase* getCurrentWidget();

public slots:

    void updateModelTree();

private:
    QTreeView *m_treeWidget;
    TreeModel *m_treeModel;


    QPushButton *m_add;
    QPushButton *m_remove;

    std::shared_ptr<SceneEditorGame> m_game;

    ObjectBase* m_currentWidget;

    //just a test:
    std::unique_ptr<ObjectGroup> g1;
    std::unique_ptr<ObjectGroup> g2;
    std::unique_ptr<ObjectGroup> g3;
    std::unique_ptr<ObjectGroup> g4;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H
