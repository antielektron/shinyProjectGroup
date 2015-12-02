#include "SceneEditor/ObjectListWidget.h"

#include "SceneEditor/SceneEditorGame.h"

ObjectListWidget::ObjectListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    this->setLayout(new QVBoxLayout(this));

    m_treeView = new QTreeView(this);
    this->layout()->addWidget(m_treeView);

    QWidget *container = new QWidget(this);
    container->setLayout(new QHBoxLayout());
    container->layout()->setMargin(0);
    this->layout()->addWidget(container);

    m_add = new QPushButton("Add Object", container);
    container->layout()->addWidget(m_add);

    m_remove = new QPushButton("Remove Object", container);
    container->layout()->addWidget(m_remove);
}

ObjectListWidget::~ObjectListWidget()
{
}
