#include "SceneEditor/GameLogic/TriggerActionWidget.h"
#include "SceneEditor/SceneEditorGame.h"
#include "GameLogic/Actions/TriggerAction.h"
#include "GameLogic/Expressions/Expression.h"
#include "GameLogic/Factories/ExpressionFactory.h"

//------------------------------------------------------------------------------
TriggerActionWidget::TriggerActionWidget(std::shared_ptr<SceneEditorGame> game, TriggerAction *action, QWidget *parent) :
        QWidget(parent),
        m_game(game),
        m_action(action)
{
    generateWidgets();
    connectStuff();

    m_name->setText(m_action->getName());
    m_delay->setText(m_action->getDelay()->string());
}

//------------------------------------------------------------------------------
TriggerActionWidget::~TriggerActionWidget()
{}

//------------------------------------------------------------------------------
void TriggerActionWidget::generateWidgets()
{
    m_layout = new QFormLayout(this);
    this->setLayout(m_layout);

    m_name = new QLineEdit(this);
    m_layout->addRow("Event Name", m_name);

    m_delay = new QLineEdit(this);
    m_layout->addRow("Delay", m_delay);

    m_apply = new QPushButton("Apply", this);
    m_layout->addRow(m_apply);
}

//------------------------------------------------------------------------------
void TriggerActionWidget::connectStuff()
{
    connect(m_apply, SIGNAL(clicked()), this, SLOT(onApplyClicked()));

    // connect to game
    connect(m_game.get(), SIGNAL(eventsChanged()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(eventsInvalidated()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(close()));
}

//------------------------------------------------------------------------------
void TriggerActionWidget::onApplyClicked()
{
    m_action->setName(m_name->text());

    auto delay = m_delay->text();
    m_action->setDelay(Factory::createExpressionFromString<double>(m_game->getGlobalState(), delay));

    m_game->notifyEventChanged();
}

//------------------------------------------------------------------------------
void TriggerActionWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    QWidget::closeEvent(event);
}
