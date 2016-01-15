#include "SceneEditor/GameLogic/EventDetailsWidget.h"
#include "GameLogic/GlobalState.h"
#include "SceneEditor/SceneEditorGame.h"

#include "GameLogic/Preconditions/IsEqualPrecondition.h"
#include "GameLogic/Preconditions/IsGreaterPrecondition.h"
#include "GameLogic/Preconditions/IsLessPrecondition.h"

#include "GameLogic/Factories/PreconditionFactory.h"
#include "SceneEditor/Factories/PreconditionDetailsWidgetFactory.h"

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
}

//------------------------------------------------------------------------------
void EventDetailsWidget::connectStuff()
{
    connect(m_applyName, SIGNAL(clicked()), this, SLOT(onApplyNameClicked()));

    connect(m_addAction, SIGNAL(clicked()), this, SLOT(onAddActionClicked()));
    connect(m_removeAction, SIGNAL(clicked()), this, SLOT(onRemoveActionClicked()));
    connect(m_editAction, SIGNAL(clicked()), this, SLOT(onEditActionClicked()));

    connect(m_addPrecondition, SIGNAL(clicked()), this, SLOT(onAddPreconditionClicked()));
    connect(m_removePrecondition, SIGNAL(clicked()), this, SLOT(onRemovePreconditionClicked()));
    connect(m_editPrecondition, SIGNAL(clicked()), this, SLOT(onEditPreconditionClicked()));

    // connect to game
    connect(m_game.get(), SIGNAL(eventsChanged()), this, SLOT(onEventsChanged()));
    connect(m_game.get(), SIGNAL(eventsInvalidated()), this, SLOT(close()));
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
        QListWidgetItem *item = new QListWidgetItem((*it)->toQString(), m_actions);
        m_actionsMap[item] = it;
        m_actions->addItem(item);
    }

    auto preconditions = m_event->getPreconditions();
    for (auto it = preconditions.begin(); it != preconditions.end(); it++)
    {
        std::cout << (*it)->string().toStdString() << std::endl;
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
    // TODO
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onRemoveActionClicked()
{
    // TODO
}

//------------------------------------------------------------------------------
void EventDetailsWidget::onEditActionClicked()
{
    // TODO
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

void EventDetailsWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    QWidget::closeEvent(event);
}
