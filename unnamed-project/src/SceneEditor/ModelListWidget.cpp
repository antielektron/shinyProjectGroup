#include "SceneEditor/ModelListWidget.h"

#include "SceneEditor/SceneEditorGame.h"
#include <iostream>
#include <cassert>

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

    updateModelList();

    connect(m_game.get(),SIGNAL(modelsChanged()), this, SLOT(updateModelList()));
}
//------------------------------------------------------------------------------

ModelListWidget::~ModelListWidget()
{

}

//------------------------------------------------------------------------------
void ModelListWidget::updateModelList()
{

    m_listView->clear();
    Scene *scene = m_game->getScene();

    if (scene)
    {
        for (auto &model : scene->getModels())
        {
            m_listView->addItem(QString::fromStdString(model.first));
        }
    }
}

