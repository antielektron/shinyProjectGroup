#include "SceneEditor/ModelListWidget.h"

#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/SceneEditorWindow.h"
#include <iostream>
#include <cassert>
#include <QFileDialog>

ModelListWidget::ModelListWidget(std::shared_ptr<SceneEditorGame> game, SceneEditorWindow *parent) :
    QWidget(parent),
    m_game(game),
    m_parent(parent)
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

    connectStuff();

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

//------------------------------------------------------------------------------
void ModelListWidget::onListClick(QModelIndex index)
{
    if (index.isValid())
    {
        m_currentModel = m_listView->model()->data(index).toString();
        //DEBUG:
        std::cout << "selected Model: " << m_currentModel.toStdString()
                  << std::endl;
    }
}

//------------------------------------------------------------------------------
void ModelListWidget::onRemoveClick()
{
    m_game->removeModel(m_currentModel.toStdString());
}

//------------------------------------------------------------------------------
void ModelListWidget::onAddClick()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("open Model"),
                                                    ".",
                                                    tr("obj Files (*.obj)"));

    if (filename.length() > 0)
    {
        m_parent->makeGlWidgetCurrent();
        Model *model = new Model(filename.toStdString());
        m_game->addModel(std::unique_ptr<Model>(model));
        m_parent->doneGlWidgetCurrent();
    }
}

//------------------------------------------------------------------------------
void ModelListWidget::connectStuff()
{
    connect(m_add, SIGNAL(clicked()),
            this, SLOT(onAddClick()));
    connect(m_remove, SIGNAL(clicked()),
            this, SLOT(onRemoveClick()));
    connect(m_game.get(),SIGNAL(modelsChanged()),
            this, SLOT(updateModelList()));
    connect(m_listView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onListClick(QModelIndex)));
}
