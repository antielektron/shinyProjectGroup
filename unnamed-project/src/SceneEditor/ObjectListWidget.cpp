#include "SceneEditor/ObjectListWidget.h"
#include "SceneEditor/TreeModel.h"
#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"

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
}

ObjectListWidget::~ObjectListWidget()
{
}

ObjectBase *ObjectListWidget::getCurrentObject()
{
    return m_currentObject;
}

void ObjectListWidget::updateModelTree()
{

    //just a Test so far:
    ObjectGroup* obj1 = new ObjectGroup();
    ObjectGroup* obj2 = new ObjectGroup();
    ObjectGroup* obj3 = new ObjectGroup();
    ObjectGroup* obj4 = new ObjectGroup();

    obj1->setName("obj1");
    obj2->setName("obj2");
    obj3->setName("obj3");
    obj4->setName("obj4");

    obj1->addObjectGroup(std::unique_ptr<ObjectGroup>(obj2));
    obj1->addObjectGroup(std::unique_ptr<ObjectGroup>(obj3));
    obj2->addObjectGroup(std::unique_ptr<ObjectGroup>(obj4));

    g1.reset(obj1);
    g2.reset(obj2);
    g3.reset(obj3);
    g4.reset(obj4);

    m_treeModel = new TreeModel(obj1, this);
    m_treeView->setModel(m_treeModel);
}

void ObjectListWidget::setCurrentObject(ObjectBase *object)
{
    m_currentObject = object;
}

