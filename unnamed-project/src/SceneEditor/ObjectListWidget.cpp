#include "SceneEditor/ObjectListWidget.h"
#include "SceneEditor/TreeModel.h"
#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"

//DEBUG
#include <iostream>
#include <cassert>

//------------------------------------------------------------------------------
ObjectListWidget::ObjectListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    m_currentObject = nullptr; //maybe should be set to root by default

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

    //fill Tree widget with data:
    m_treeModel = nullptr;
    updateModelTree();

    //connect stuff:
    connect(m_treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(setCurrentObject(QModelIndex)));
}

//------------------------------------------------------------------------------
ObjectListWidget::~ObjectListWidget()
{
}

//------------------------------------------------------------------------------
ObjectBase *ObjectListWidget::getCurrentObject()
{
    return m_currentObject;
}

//------------------------------------------------------------------------------
void ObjectListWidget::updateModelTree()
{
    if (m_game->isInitialized())
    {

        ObjectGroup *root = m_game->getRootObject();
        assert(root);
        m_treeModel = new TreeModel(root, this);
        m_treeView->setModel(m_treeModel);
    }
    else{
        //TODO: maybe a default root?
    }
}

//------------------------------------------------------------------------------
void ObjectListWidget::setCurrentObject(const QModelIndex &index)
{
    m_currentObject = m_treeModel->getGameObject(index);
    emit currentObjectChanged(m_currentObject);

    //debug output
    std::cout << "Current Object changed to: "
              << m_currentObject->getName().toStdString()
              << std::endl;
}

