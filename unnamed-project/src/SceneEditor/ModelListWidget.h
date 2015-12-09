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

class ModelListWidget : public QWidget
{
    Q_OBJECT
public:
    ModelListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~ModelListWidget();

    ObjectBase* getCurrentWidget();

private:
    QListWidget *m_listView;


    QPushButton *m_add;
    QPushButton *m_remove;

    std::shared_ptr<SceneEditorGame> m_game;

    ObjectBase* m_currentWidget;

};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H
