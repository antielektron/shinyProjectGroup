#include "SceneEditor/GameLogic/BinaryBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"

#include <QBoxLayout>

BinaryBaseWidget::BinaryBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) :
        QWidget(parent),
        m_game(game)
{
    generateWidgets();
    connectStuff();
}

BinaryBaseWidget::~BinaryBaseWidget()
{
}

void BinaryBaseWidget::generateWidgets()
{
    this->setLayout(new QVBoxLayout());

    QWidget *container = new QWidget(this);
    container->setLayout(new QHBoxLayout());
    this->layout()->addWidget(container);

    m_lhs = new QLineEdit(container);
    container->layout()->addWidget(m_lhs);

    m_type = new QLabel(container);
    container->layout()->addWidget(m_type);

    m_rhs = new QLineEdit(container);
    container->layout()->addWidget(m_rhs);

    m_apply = new QPushButton("Apply", this);
    this->layout()->addWidget(m_apply);
}

void BinaryBaseWidget::connectStuff()
{
    connect(m_apply, SIGNAL(clicked()), this, SLOT(onApplyClicked()));

    // connect to game, always close! (even when apply was clicked)
    connect(m_game.get(), SIGNAL(eventsChanged()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(eventsInvalidated()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(close()));
}

void BinaryBaseWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    QWidget::closeEvent(event);
}
