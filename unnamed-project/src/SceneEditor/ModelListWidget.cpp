#include "SceneEditor/ModelListWidget.h"

#include "SceneEditor/SceneEditorGame.h"

ModelListWidget::ModelListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    this->setLayout(new QVBoxLayout(this));

    m_listWidget = new QListWidget(this);
    this->layout()->addWidget(m_listWidget);

    QWidget *container = new QWidget(this);
    container->setLayout(new QHBoxLayout());
    container->layout()->setMargin(0);
    this->layout()->addWidget(container);

    m_add = new QPushButton("Add Model", container);
    container->layout()->addWidget(m_add);

    m_remove = new QPushButton("Remove Model", container);
    container->layout()->addWidget(m_remove);
}

ModelListWidget::~ModelListWidget()
{
}
