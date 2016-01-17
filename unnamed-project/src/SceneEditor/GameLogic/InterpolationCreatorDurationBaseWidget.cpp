#include "SceneEditor/GameLogic/InterpolationCreatorDurationBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"

InterpolationCreatorDurationBaseWidget::InterpolationCreatorDurationBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) :
        QWidget(parent),
        m_game(game)
{
    generateWidgets();
    connectStuff();
}

void InterpolationCreatorDurationBaseWidget::generateWidgets()
{
    m_layout = new QFormLayout(this);
    this->setLayout(m_layout);

    m_type = new QLabel(this);
    m_layout->addRow("Type", m_type);

    m_duration = new QLineEdit(this);
    m_layout->addRow("Duration", m_duration);

    m_apply = new QPushButton("Apply", this);
    m_layout->addRow(m_apply);
}

void InterpolationCreatorDurationBaseWidget::connectStuff()
{
    connect(m_apply, SIGNAL(clicked()), this, SLOT(onApplyClicked()));

    // connect to game (close on any change)
    connect(m_game.get(), SIGNAL(eventsChanged()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(eventsInvalidated()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(close()));
}

void InterpolationCreatorDurationBaseWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    QWidget::closeEvent(event);
}
