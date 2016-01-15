#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENT_DETAILS_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENT_DETAILS_WIDGET_H

#include <memory>
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

#include "GameLogic/Event.h"

class SceneEditorGame;

class EventDetailsWidget : public QWidget
{
Q_OBJECT
public:
    EventDetailsWidget(std::shared_ptr<SceneEditorGame> game, Event *event, QWidget *parent = nullptr);
    virtual ~EventDetailsWidget();

protected slots:
    void onEventsChanged();

    void onApplyNameClicked();

    void onAddActionClicked();
    void onRemoveActionClicked();
    void onEditActionClicked();
    void onMoveActionUpClicked();
    void onMoveActionDownClicked();

    void onAddPreconditionClicked();
    void onRemovePreconditionClicked();
    void onEditPreconditionClicked();
    void onMovePreconditionUpClicked();
    void onMovePreconditionDownClicked();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;
    Event *m_event;

    QLineEdit *m_nameEdit;
    QPushButton *m_applyName;

    QListWidget *m_actions;
    std::map<QListWidgetItem *, Event::ActionIterator> m_actionsMap;
    QPushButton *m_addAction;
    QPushButton *m_removeAction;
    QPushButton *m_editAction;
    QPushButton *m_moveActionUp;
    QPushButton *m_moveActionDown;

    QListWidget *m_preconditions;
    std::map<QListWidgetItem *, Event::PreconditionIterator> m_preconditionsMap;
    QPushButton *m_addPrecondition;
    QPushButton *m_removePrecondition;
    QPushButton *m_editPrecondition;
    QPushButton *m_movePreconditionUp;
    QPushButton *m_movePreconditionDown;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENT_DETAILS_WIDGET_H
