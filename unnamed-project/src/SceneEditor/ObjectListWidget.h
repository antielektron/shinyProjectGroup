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
    ObjectListWidget(std::shared_ptr<SceneEditorGame> game, SceneEditorWindow *parent);
    virtual ~ObjectListWidget();

public slots:
    /**
     * will be triggered, if the corresponding treeview item is clicked
     * @param index corresponding Tree Model index
     */
    void onTreeViewClicked(const QModelIndex &index);

    void onAddObjectClick();
    void onAddGroupClick();
    void onRemoveClick();

    void onObjectsChanged();
    void onCurrentObjectChanged();

private:
    void connectStuff();

    // TODO use QTreeWidget!
    QTreeView *m_treeView;
    TreeModel *m_treeModel;

    QPushButton *m_addObject;
    QPushButton *m_addGroup;
    QPushButton *m_remove;

    // TODO buttons for adding/removing and moving objects!

    std::shared_ptr<SceneEditorGame> m_game;

    SceneEditorWindow *m_parent;

};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_OBJECT_LIST_WIDGET_H
