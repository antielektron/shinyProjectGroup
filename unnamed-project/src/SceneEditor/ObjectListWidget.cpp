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
    m_currentObject = nullptr; //maybe should be set to root by default

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
    updateModelTree();

    connectStuff();

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

//------------------------------------------------------------------------------
void ObjectListWidget::onAddObjectClick()
{
    // get current Model from parent:
    Model *model = m_parent->getCurrentModel();

    if (!model)
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
                                  QString::fromStdString(model->getName()),
                                  &ok);

    if (ok)
    {
        ObjectGroup *parent = nullptr;

        // check if selected object is an objectGroup.
        // in this case we create the object as it's child
        if (m_currentObject)
        {
            if (m_currentObject->getObjectType() == ObjectType::ObjectGroup)
            {
                // downcasting ObjectBase to ObjectGroup
                parent = static_cast<ObjectGroup *>(m_currentObject);
            }
        }
        m_game->createObject(model->getName(),parent)->setName(objectName);
        updateModelTree();
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
                                  "new Object Group",
                                  &ok);

    if (ok)
    {
        ObjectGroup *parent = nullptr;

        // check if selected object is an objectGroup.
        // in this case we create the object as it's child
        if (m_currentObject)
        {
            if (m_currentObject->getObjectType() == ObjectType::ObjectGroup)
            {
                // downcasting ObjectBase to ObjectGroup
                parent = static_cast<ObjectGroup *>(m_currentObject);
            }
        }
        m_game->createObjectGroup(groupName.toStdString(),parent);
        updateModelTree();
    }
}

//------------------------------------------------------------------------------
void ObjectListWidget::onRemoveClick()
{
    if (m_currentObject)
    {
        m_currentObject->destroy();
        m_currentObject = nullptr;
        updateModelTree();
        emit updateSceneObjectsRequest();
    }
}

//------------------------------------------------------------------------------
void ObjectListWidget::connectStuff()
{
    connect(m_treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(setCurrentObject(QModelIndex)));
    connect(m_addObject, SIGNAL(clicked()),
            this, SLOT(onAddObjectClick()));
    connect(m_addGroup, SIGNAL(clicked()),
            this, SLOT(onAddGroupClick()));
    connect(m_remove, SIGNAL(clicked()),
            this, SLOT(onRemoveClick()));
}

