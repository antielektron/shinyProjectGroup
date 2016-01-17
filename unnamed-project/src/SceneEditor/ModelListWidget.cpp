#include "SceneEditor/ModelListWidget.h"

#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/SceneEditorWindow.h"
#include <iostream>
#include <cassert>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

ModelListWidget::ModelListWidget(std::shared_ptr<SceneEditorGame> game, SceneEditorWindow *parent) :
    QWidget(parent),
    m_game(game),
    m_parent(parent),
    m_modelsLocked(false)
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

    connectStuff();

    onModelsChanged();
}

//------------------------------------------------------------------------------
ModelListWidget::~ModelListWidget()
{}

//------------------------------------------------------------------------------
void ModelListWidget::connectStuff()
{
    connect(m_add, SIGNAL(clicked()), this, SLOT(onAddClicked()));
    connect(m_remove, SIGNAL(clicked()), this, SLOT(onRemoveClicked()));
    connect(m_listView, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(onCurrentItemChanged(QListWidgetItem *, QListWidgetItem *)));

    // connect to game
    connect(m_game.get(), SIGNAL(currentModelChanged()), this, SLOT(onCurrentModelChanged()));
    connect(m_game.get(), SIGNAL(modelsChanged()), this, SLOT(onModelsChanged()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(onModelsChanged()));
}

//------------------------------------------------------------------------------
void ModelListWidget::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current)
    {
        QString currentModel = current->text();
        m_game->setCurrentModel(currentModel.toStdString());
    }
}

//------------------------------------------------------------------------------
void ModelListWidget::onRemoveClicked()
{
    auto currentModelName = m_game->getCurrentModel();

    if (currentModelName.empty())
        return;

    // test if model is used:
    for (Object *objects : m_game->getScene()->getObjects())
    {
        if (objects->getModel()->getName() == currentModelName)
        {
            QMessageBox::information(this,
                                     "Error",
                                     "Model is still used by objects");
            return;
        }
    }

    m_game->removeCurrentModel();
}

//------------------------------------------------------------------------------
void ModelListWidget::onAddClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Model"),
                                                    ".",
                                                    tr("obj Files (*.obj)"));

    // create relative filepath from current working directory
    QString relativePath = QDir::current().relativeFilePath(filename);

    if (filename.length() > 0)
    {
        m_parent->makeGlWidgetCurrent();
        Model *model = new Model(relativePath.toStdString());
        m_game->addModel(std::unique_ptr<Model>(model));
        m_parent->doneGlWidgetCurrent();
    }
}

//------------------------------------------------------------------------------
void ModelListWidget::onModelsChanged()
{
    m_listView->clear();
    m_modelNameMap.clear();

    Scene *scene = m_game->getScene();

    if (scene)
    {
        for (auto &model : scene->getModels())
        {
            auto item = new QListWidgetItem(QString::fromStdString(model.first));
            m_modelNameMap[model.first] = item;
            m_listView->addItem(item);
        }
        // TODO select current model
    }

    // select the correct model..
    onCurrentModelChanged();
}

//------------------------------------------------------------------------------
void ModelListWidget::onCurrentModelChanged()
{
    // update selection
    auto &currentModelName = m_game->getCurrentModel();
    m_listView->setCurrentItem(m_modelNameMap[currentModelName]);
}

