#include "SceneEditor/GameLogic/EventDetailsWidget.h"
#include "GameLogic/GlobalState.h"
#include "SceneEditor/SceneEditorGame.h"

#include "GameLogic/Factories/PreconditionFactory.h"
#include "GameLogic/Factories/ActionFactory.h"
#include "SceneEditor/Factories/PreconditionDetailsWidgetFactory.h"
#include "SceneEditor/Factories/ActionDetailsWidgetFactory.h"

#include <QBoxLayout>
#include <QInputDialog>
#include <iostream>

//------------------------------------------------------------------------------
EventDetailsWidget::EventDetailsWidget(std::shared_ptr<SceneEditorGame> game, Event *event, QWidget *parent) :
    QWidget(parent),
    m_game(game),
    m_event(event)
{
    generateWidgets();
    connectStuff();

    // Update ui
    onEventsChanged();
}

//------------------------------------------------------------------------------
EventDetailsWidget::~EventDetailsWidget()
{
}

//------------------------------------------------------------------------------
void EventDetailsWidget::generateWidgets()
{
    this->setLayout(new QHBoxLayout());

    QWidget *containerLeft = new QWidget(this);
    containerLeft->setLayout(new QVBoxLayout());
    this->layout()->addWidget(containerLeft);


    QWidget *containerRight = new QWidget(this);
    containerRight->setLayout(new QVBoxLayout());
    this->layout()->addWidget(containerRight);


    // Name
    QWidget *containerName = new QWidget(containerLeft);
    containerName->setLayout(new QHBoxLayout());
    containerName->layout()->setMargin(0);
    containerLeft->layout()->addWidget(containerName);

    m_nameEdit = new QLineEdit(containerName);
    containerName->layout()->addWidget(m_nameEdit);

    m_applyName = new QPushButton("Apply Name", containerName);
    containerName->layout()->addWidget(m_applyName);


    // Actions List + Buttons
    m_actions = new QListWidget(containerLeft);
    containerLeft->layout()->addWidget(m_actions);

    QWidget *containerActionsButtons = new QWidget(containerLeft);
    containerActionsButtons->setLayout(new QHBoxLayout());
    containerActionsButtons->layout()->setMargin(0);
    containerLeft->layout()->addWidget(containerActionsButtons);

    m_addAction = new QPushButton("Add Action", containerActionsButtons);
    containerActionsButtons->layout()->addWidget(m_addAction);

    m_removeAction = new QPushButton("Remove Action", containerActionsButtons);
    containerActionsButtons->layout()->addWidget(m_removeAction);

    m_editAction = new QPushButton("Edit Action", containerActionsButtons);
    containerActionsButtons->layout()->addWidget(m_editAction);

    m_moveActionUp = new QPushButton("Up", containerActionsButtons);
    containerActionsButtons->layout()->addWidget(m_moveActionUp);

    m_moveActionDown = new QPushButton("Down", containerActionsButtons);
    containerActionsButtons->layout()->addWidget(m_moveActionDown);


    // Preconditions List + Buttons
    m_preconditions = new QListWidget(containerRight);
    containerRight->layout()->addWidget(m_preconditions);

    QWidget *containerPreconditionsButtons = new QWidget(containerRight);
    containerPreconditionsButtons->setLayout(new QHBoxLayout());
    containerPreconditionsButtons->layout()->setMargin(0);
    containerRight->layout()->addWidget(containerPreconditionsButtons);

    m_addPrecondition = new QPushButton("Add Precondition", containerPreconditionsButtons);
    containerPreconditionsButtons->layout()->addWidget(m_addPrecondition);

    m_removePrecondition = new QPushButton("Remove Precondition", containerPreconditionsButtons);
    containerPreconditionsButtons->layout()->addWidget(m_removePrecondition);

    m_editPrecondition = new QPushButton("Edit Precondition", containerPreconditionsButtons);
    containerPreconditionsButtons->layout()->addWidget(m_editPrecondition);

    m_movePreconditionUp = new QPushButton("Up", containerPreconditionsButtons);
    containerPreconditionsButtons->layout()->addWidget(m_movePreconditionUp);

    m_movePreconditionDown = new QPushButton("Down", containerPreconditionsButtons);
    containerPreconditionsButtons->layout()->addWidget(m_movePreconditionDown);
}

//------------------------------------------------------------------------------
void EventDetailsWidget::connectStuff()
{
    connect(m_applyName, SIGNAL(clicked()), this, SLOT(onApplyNameClicked()));

    connect(m_addAction, SIGNAL(clicked()), this, SLOT(onAddActionClicked()));
    connect(m_removeAction, SIGNAL(clicked()), this, SLOT(onRemoveActionClicked()));
    connect(m_editAction, SIGNAL(clicked()), this, SLOT(onEditActionClicked()));
    connect(m_moveActionUp, SIGNAL(clicked()), this, SLOT(onMoveActionUpClicked()));
    connect(m_moveActionDown, SIGNAL(clicked()), this, SLOT(onMoveActionDownClicked()));

    connect(m_addPrecondition, SIGNAL(clicked()), this, SLOT(onAddPreconditionClicked()));
    connect(m_removePrecondition, SIGNAL(clicked()), this, SLOT(onRemovePreconditionClicked()));
    connect(m_editPrecondition, SIGNAL(clicked()), this, SLOT(onEditPreconditionClicked()));
    connect(m_movePreconditionUp, SIGNAL(clicked()), this, SLOT(onMovePreconditionUpClicked()));
    connect(m_movePreconditionDown, SIGNAL(clicked()), this, SLOT(onMovePreconditionDownClicked()));

    // connect to game
    connect(m_game.get(), SIGNAL(eventsChanged()), this, SLOT(onEventsChanged()));
    connect(m_game.get(), SIGNAL(eventsInvalidated()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(close()));
}


//------------------------------------------------------------------------------
void EventDetailsWidget::onEventsChanged()
{
    // TODO verify that this event has not been deleted!
    auto globalState = m_game->getGlobalState();
    // NOTE: global state has to exist at this point!

    m_nameEdit->setText(m_event->getName());

    m_actions->clear();
    m_preconditions->clear();

    auto actions = m_event->getActions();
    for (auto it = actions.begin(); it != actions.end(); it++)
    {
        QListWidgetItem *item = new QListWidgetItem((*it)->string(), m_actions);
        m_actionsMap[item] = it;
        m_actions->addItem(item);
    }

    auto preconditions = m_event->getPreconditions();
    for (auto it = preconditions.begin(); it != preconditions.end(); it++)
    {
        QListWidgetItem *item = new QListWidgetItem((*it)->string(), m_preconditions);
        m_preconditionsMap[item] = it;
        m_preconditions->addItem(item);
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onApplyNameClicked()
{
    m_event->setName(m_nameEdit->text());
    m_game->notifyEventChanged();
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onAddActionClicked()
{
    bool ok;

    QStringList types;

    auto vecTypes = Factory::getKnownActionTypes();
    std::copy(vecTypes.begin(), vecTypes.end(), std::back_inserter(types));

    // ask user what type he wants to add, and default construct one
    QString type = QInputDialog::getItem(this, "select action type", "type", types, 0, false, &ok);

    if (ok)
    {
        // ask factory to create one
        auto action = Factory::createActionFromType(m_game->getGlobalState(), type);
        m_event->addAction(std::move(action));

        m_game->notifyEventChanged();
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onRemoveActionClicked()
{
    auto it = m_actionsMap.find(m_actions->currentItem());
    if (it != m_actionsMap.end())
    {
        m_event->removeAction(it->second);
        m_game->notifyEventChanged();
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onEditActionClicked()
{
    auto it = m_actionsMap.find(m_actions->currentItem());
    if (it != m_actionsMap.end())
    {
        auto widget = Factory::createActionDetailsWidget(m_game, it->second->get(), nullptr);
        widget->show();
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onMoveActionDownClicked()
{
    int row = m_actions->currentRow();
    if (row < m_actions->count()-1)
    {
        auto item = m_actions->takeItem(row);
        m_actions->insertItem(row+1, item);
        m_actions->setCurrentItem(item);
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onMoveActionUpClicked()
{
    int row = m_actions->currentRow();
    if (row > 0)
    {
        auto item = m_actions->takeItem(row);
        m_actions->insertItem(row-1, item);
        m_actions->setCurrentItem(item);
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onAddPreconditionClicked()
{
    bool ok;

    QStringList types;

    auto vecTypes = Factory::getKnownPreconditionTypes();
    std::copy(vecTypes.begin(), vecTypes.end(), std::back_inserter(types));

    // ask user what type he wants to add, and default construct one
    QString type = QInputDialog::getItem(this, "select precondition type", "type", types, 0, false, &ok);

    if (ok)
    {
        // ask factory to create one
        auto precondition = Factory::createPreconditionFromType(type);
        m_event->addPrecondition(std::move(precondition));

        m_game->notifyEventChanged();
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onRemovePreconditionClicked()
{
    auto it = m_preconditionsMap.find(m_preconditions->currentItem());
    if (it != m_preconditionsMap.end())
    {
        m_event->removePrecondition(it->second);
        m_game->notifyEventChanged();
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onEditPreconditionClicked()
{
    auto it = m_preconditionsMap.find(m_preconditions->currentItem());
    if (it != m_preconditionsMap.end())
    {
        auto widget = Factory::createPreconditionDetailsWidget(m_game, it->second->get(), nullptr);
        widget->show();
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onMovePreconditionDownClicked()
{
    int row = m_preconditions->currentRow();
    if (row < m_preconditions->count()-1)
    {
        auto item = m_preconditions->takeItem(row);
        m_preconditions->insertItem(row+1, item);
        m_preconditions->setCurrentItem(item);
    }
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onMovePreconditionUpClicked()
{
    int row = m_preconditions->currentRow();
    if (row > 0)
    {
        auto item = m_preconditions->takeItem(row);
        m_preconditions->insertItem(row-1, item);
        m_preconditions->setCurrentItem(item);
    }
}

void EventDetailsWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    QWidget::closeEvent(event);
}
