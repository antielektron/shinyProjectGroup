#include "SceneEditor/ObjectListWidget.h"
#include "SceneEditor/TreeModel.h"
#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/SceneEditorWindow.h"
#include "Scene/Object.h"
#include <QInputDialog>
#include <QMessageBox>

//DEBUG
#include <iostream>
#include <cassert>

//------------------------------------------------------------------------------
ObjectListWidget::ObjectListWidget(std::shared_ptr<SceneEditorGame> game, SceneEditorWindow *parent) :
    QWidget(parent),
    m_game(game),
    m_parent(parent)
{
    this->setLayout(new QVBoxLayout(this));

    m_treeView = new QTreeView(this);
    this->layout()->addWidget(m_treeView);

    QWidget *container = new QWidget(this);
    container->setLayout(new QHBoxLayout());
    container->layout()->setMargin(0);
    this->layout()->addWidget(container);

    m_addObject = new QPushButton("Add Object", container);
    container->layout()->addWidget(m_addObject);

    m_addGroup = new QPushButton("Add Object Group", container);
    container->layout()->addWidget(m_addGroup);

    m_remove = new QPushButton("Remove Object", container);
    container->layout()->addWidget(m_remove);

    //fill Tree widget with data:
    m_treeModel = nullptr;

    connectStuff();

    onObjectsChanged();
}

//------------------------------------------------------------------------------
ObjectListWidget::~ObjectListWidget()
{
}

//------------------------------------------------------------------------------
void ObjectListWidget::connectStuff()
{
    connect(m_treeView, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(onTreeViewClicked(const QModelIndex &)));
    connect(m_addObject, SIGNAL(clicked()), this, SLOT(onAddObjectClick()));
    connect(m_addGroup, SIGNAL(clicked()), this, SLOT(onAddGroupClick()));
    connect(m_remove, SIGNAL(clicked()), this, SLOT(onRemoveClick()));

    // Connect to game
    connect(m_game.get(), SIGNAL(currentObjectChanged()), this, SLOT(onCurrentObjectChanged()));
    connect(m_game.get(), SIGNAL(objectsChanged()), this, SLOT(onObjectsChanged()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(onObjectsChanged()));
}

//------------------------------------------------------------------------------
void ObjectListWidget::onTreeViewClicked(const QModelIndex &index)
{
    auto currentObject = m_treeModel->getGameObject(index);
    m_game->notifyCurrentObjectChanged(currentObject);

    //debug output
    std::cout << "Current Object changed to: "
              << currentObject->getName().toStdString()
              << std::endl;
}

//------------------------------------------------------------------------------
void ObjectListWidget::onAddObjectClick()
{
    // get current Model from parent:
    auto currentModelName = m_game->getCurrentModel();

    if (currentModelName.empty())
    {
        QMessageBox::information(this,
                                 "error while creating object",
                                 "please select a model first");
        return;
    }

    // get new object name by user:
    bool ok;
    QString objectName =
            QInputDialog::getText(this,
                                  "Create object",
                                  "Object name:",
                                  QLineEdit::Normal,
                                  QString::fromStdString(currentModelName),
                                  &ok);

    if (ok)
    {
        auto *currentObject = m_game->getCurrentObject();
        ObjectGroup *parent = dynamic_cast<ObjectGroup *>(currentObject);

        m_game->createObject(currentModelName, parent)->setName(objectName);
    }
}

//------------------------------------------------------------------------------
void ObjectListWidget::onAddGroupClick()
{
    // get new object name by user:
    bool ok;
    QString groupName =
            QInputDialog::getText(this,
                                  "Create object",
                                  "Object name:",
                                  QLineEdit::Normal,
                                  "New Object Group",
                                  &ok);

    if (ok)
    {
        // check if selected object is an objectGroup.
        // in this case we create the object as it's child

        auto *currentObject = m_game->getCurrentObject();
        ObjectGroup *parent = dynamic_cast<ObjectGroup *>(currentObject);

        m_game->createObjectGroup(groupName.toStdString(), parent);
    }
}

//------------------------------------------------------------------------------
void ObjectListWidget::onRemoveClick()
{
    m_game->removeCurrentObject();
}

//------------------------------------------------------------------------------
void ObjectListWidget::onObjectsChanged()
{
    if (m_game->isInitialized())
    {
        ObjectGroup *root = m_game->getRootObject();
        assert(root);
        m_treeModel = new TreeModel(root, this);
        m_treeView->setModel(m_treeModel);

        onCurrentObjectChanged();
    }
    else
    {
        //TODO: maybe a default root?
    }
}

//------------------------------------------------------------------------------
void ObjectListWidget::onCurrentObjectChanged()
{
    // TODO update selection!
}
