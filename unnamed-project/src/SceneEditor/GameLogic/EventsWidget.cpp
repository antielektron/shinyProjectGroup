#include "SceneEditor/GameLogic/EventsWidget.h"
#include "SceneEditor/SceneEditorGame.h"

#include "GameLogic/GameLogicUtility.h"

#include "SceneEditor/GameLogic/EventDetailsWidget.h"

#include "GameLogic/GlobalState.h"
#include "Scene/Scene.h"
#include "GameLogic/Event.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <QListWidgetItem>

#include "GameLogic/Actions/CopyAttributeAction.h"
#include "GameLogic/Actions/InvertBooleanAction.h"
#include "GameLogic/Actions/ArithmeticalAction.h"

//------------------------------------------------------------------------------
EventsWidget::EventsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) :
    QWidget(parent),
    m_game(game)
{
    generateWidgets();
    connectStuff();

    // build content
    onEventsChanged();
}

//------------------------------------------------------------------------------
void EventsWidget::generateWidgets()
{
    this->setLayout(new QVBoxLayout());

    m_events = new QListWidget(this);
    this->layout()->addWidget(m_events);

    QWidget *containerButtons = new QWidget(this);
    containerButtons->setLayout(new QHBoxLayout());
    containerButtons->layout()->setMargin(0);
    this->layout()->addWidget(containerButtons);

    m_addEvent = new QPushButton("Add Event", containerButtons);
    containerButtons->layout()->addWidget(m_addEvent);

    m_removeEvent = new QPushButton("Remove Event", containerButtons);
    containerButtons->layout()->addWidget(m_removeEvent);

    m_editEvent = new QPushButton("Edit Event", containerButtons);
    containerButtons->layout()->addWidget(m_editEvent);
}

//------------------------------------------------------------------------------
void EventsWidget::connectStuff()
{
    connect(m_addEvent, SIGNAL(clicked()), this, SLOT(onAddEventClicked()));
    connect(m_removeEvent, SIGNAL(clicked()), this, SLOT(onRemoveEventClicked()));
    connect(m_editEvent, SIGNAL(clicked()), this, SLOT(onEditEventClicked()));

    // TODO connecto to game
}

//------------------------------------------------------------------------------
void EventsWidget::onEventsChanged()
{
    m_events->clear();
    m_eventMap.clear();

    auto globalState = m_game->getGlobalState();
    if (globalState)
    {
        auto events = globalState->getEvents();
        for (auto it = events.begin(); it != events.end(); it++)
        {
            // TODO also information about preconditons n stuff
            QListWidgetItem *item = new QListWidgetItem((*it)->getName(), m_events);
            m_eventMap[item] = it;
            m_events->addItem(item);
        }
    }
}

//------------------------------------------------------------------------------
void EventsWidget::onAddEventClicked()
{
    if (!m_game->getGlobalState())
    {
        QMessageBox::warning(this, "Warning", "Scene not initialized yet");
        return;
    }

    bool ok;

    QString eventName =
            QInputDialog::getText(this,
                                  "enter new event name",
                                  "name",
                                  QLineEdit::Normal,
                                  "empty",
                                  &ok);

    if (!ok)
    {
        return;
    }

    m_game->addEvent(std::unique_ptr<Event>(new Event(eventName)));
}

//------------------------------------------------------------------------------
void EventsWidget::onRemoveEventClicked()
{
    auto it = m_eventMap.find(m_events->currentItem());
    if (it != m_eventMap.end())
        m_game->deleteEvent(it->second);
}

//------------------------------------------------------------------------------
void EventsWidget::onEditEventClicked()
{
    auto it = m_eventMap.find(m_events->currentItem());
    if (it != m_eventMap.end())
    {
        auto *widget = new EventDetailsWidget(m_game, it->second->get());
        widget->show();
    }
}

//------------------------------------------------------------------------------

