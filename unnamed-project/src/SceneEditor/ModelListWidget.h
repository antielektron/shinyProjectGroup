#ifndef UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H

#include <memory>

#include <QWidget>
#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>
#include "Scene/ObjectGroup.h"

class SceneEditorGame;
class ObjectBase;
class TreeModel;
class SceneEditorWindow;

class ModelListWidget : public QWidget
{
    Q_OBJECT
public:
    ModelListWidget(std::shared_ptr<SceneEditorGame> game, SceneEditorWindow *parent = nullptr);
    virtual ~ModelListWidget();

public slots:
    void updateModelList();
    void onListClick(QModelIndex index);
    void onRemoveClick();
    void onAddClick();

private:
    void connectStuff();

    QListWidget *m_listView;

    QString m_currentModel;

    QPushButton *m_add;
    QPushButton *m_remove;

    std::shared_ptr<SceneEditorGame> m_game;
    SceneEditorWindow *m_parent;



};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H
