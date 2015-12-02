#ifndef UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H

#include <memory>

#include <QWidget>
#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>

class SceneEditorGame;

class ModelListWidget : public QWidget
{
    Q_OBJECT
public:
    ModelListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~ModelListWidget();

private:
    QListWidget *m_listWidget;

    QPushButton *m_add;
    QPushButton *m_remove;

    std::shared_ptr<SceneEditorGame> m_game;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_MODEL_LIST_WIDGET_H
