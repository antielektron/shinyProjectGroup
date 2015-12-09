#include "SceneEditor/ModelListWidget.h"

#include "SceneEditor/SceneEditorGame.h"


ModelListWidget::ModelListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    this->setLayout(new QVBoxLayout(this));

    m_listView = new QListWidget();
    this->layout()->addWidget(m_listView);

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

ObjectBase* ModelListWidget::getCurrentWidget()
{
    return m_currentWidget;
}
