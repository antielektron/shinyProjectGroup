#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENTS_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENTS_WIDGET_H

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "GameLogic/GlobalState.h"

class Scene;
class SceneEditorGame;

class EventsWidget : public QWidget
{
    Q_OBJECT
public:
    EventsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);

public slots:
    void onEventsChanged();

protected slots:
    void onAddEventClicked();
    void onRemoveEventClicked();
    void onEditEventClicked();

protected:
    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;

    // TreeView?
    QListWidget *m_events;
    std::map<QListWidgetItem *, GlobalState::EventIterator> m_eventMap;

    QPushButton *m_addEvent;
    QPushButton *m_removeEvent;
    QPushButton *m_editEvent;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_EVENTS_WIDGET_H
