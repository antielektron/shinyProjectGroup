#ifndef UNNAMED_PROJECT_SCENE_EDITOR_ATTRIBUTES_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_ATTRIBUTES_WIDGET_H

#include <memory>

#include <QPushButton>
#include <QTableWidget>

#include "SceneEditor/AttributesTableWidget.h"
#include "GameLogic/GlobalState.h"

class SceneEditorGame;

class AttributesWidget : public QWidget
{
    Q_OBJECT
public:
    AttributesWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);

protected slots:
    void onAddValueClicked();
    void onDeleteValueClicked();

protected:
    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;

    AttributesTableWidget *m_attributeTable;

    QPushButton *m_addValue;
    QPushButton *m_deleteValue;
};

#endif // ATTRIBUTEWIDGET_H
