#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENT_DETAILS_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENT_DETAILS_WIDGET_H

#include <memory>
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

class SceneEditorGame;
class Event;

class EventDetailsWidget : public QWidget
{
Q_OBJECT
public:
    EventDetailsWidget(std::shared_ptr<SceneEditorGame> game, Event *event, QWidget *parent = nullptr);
    virtual ~EventDetailsWidget();

protected slots:
    void onApplyNameClicked();

    void onAddActionClicked();
    void onRemoveActionClicked();
    void onEditActionClicked();

    void onAddPreconditionClicked();
    void onRemovePreconditionClicked();
    void onEditPreconditionClicked();

private:
    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;
    Event *m_event;

    QLineEdit *m_nameEdit;
    QPushButton *m_applyName;

    QListWidget *m_actions;
    QPushButton *m_addAction;
    QPushButton *m_removeAction;
    QPushButton *m_editAction;

    QListWidget *m_preconditions;
    QPushButton *m_addPrecondition;
    QPushButton *m_removePrecondition;
    QPushButton *m_editPrecondition;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENT_DETAILS_WIDGET_H
