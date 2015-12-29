#ifndef UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H

#include <memory>

#include <QWidget>
#include <QTreeView>
#include <QBoxLayout>
#include <QPushButton>
#include "Scene/ObjectGroup.h"

class SceneEditorWindow;
class SceneEditorGame;
class TreeModel;

class ObjectListWidget : public QWidget
{
    Q_OBJECT
public:
    ObjectListWidget(std::shared_ptr<SceneEditorGame>, SceneEditorWindow *parent);
    virtual ~ObjectListWidget();

public slots:

    void updateModelTree();

    /**
     * will be triggered, if the corresponding treeview item is clicked
     * @param index corresponding Tree Model index
     */
    void setCurrentObject(const QModelIndex &index);

    void onAddObjectClick();
    void onAddGroupClick();
    void onRemoveClick();

signals:
    void updateSceneObjectsRequest();

private:

    void connectStuff();

    QTreeView *m_treeView;
    TreeModel *m_treeModel;

    QPushButton *m_addObject;
    QPushButton *m_addGroup;
    QPushButton *m_remove;

    ObjectBase *m_currentObject;

    // TODO buttons for adding/removing and moving objects!

    std::shared_ptr<SceneEditorGame> m_game;

    SceneEditorWindow *m_parent;

};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H
