#include "SceneEditor/GameLogic/UnaryBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"

#include <QBoxLayout>

UnaryBaseWidget::UnaryBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) :
        QWidget(parent),
        m_game(game)
{
    generateWidgets();
    connectStuff();
}

UnaryBaseWidget::~UnaryBaseWidget()
{}

void UnaryBaseWidget::generateWidgets()
{
    this->setLayout(new QHBoxLayout());

    m_type = new QLabel(this);
    this->layout()->addWidget(m_type);

    m_key = new QLineEdit(this);
    this->layout()->addWidget(m_key);

    m_apply = new QPushButton("Apply", this);
    this->layout()->addWidget(m_apply);
}

void UnaryBaseWidget::connectStuff()
{
    connect(m_apply, SIGNAL(clicked()), this, SLOT(onApplyClicked()));

    // connect to game (close on any change)
    connect(m_game.get(), SIGNAL(eventsChanged()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(eventsInvalidated()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(close()));
}

void UnaryBaseWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    QWidget::closeEvent(event);
}
