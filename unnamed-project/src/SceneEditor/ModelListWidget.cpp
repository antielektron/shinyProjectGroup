#include "SceneEditor/ModelListWidget.h"
#include "SceneEditor/TreeModel.h"
#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"

ModelListWidget::ModelListWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    this->setLayout(new QVBoxLayout(this));

    m_treeWidget = new QTreeView();
    this->layout()->addWidget(m_treeWidget);

    QWidget *container = new QWidget(this);
    container->setLayout(new QHBoxLayout());
    container->layout()->setMargin(0);
    this->layout()->addWidget(container);

    m_add = new QPushButton("Add Model", container);
    container->layout()->addWidget(m_add);

    m_remove = new QPushButton("Remove Model", container);
    container->layout()->addWidget(m_remove);

    //fill Tree widget with data:
    m_treeModel = nullptr;
    updateModelTree();
}

ModelListWidget::~ModelListWidget()
{
}

void ModelListWidget::updateModelTree()
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

    obj1->addObjectGroup(obj2);
    obj1->addObjectGroup(obj3);
    obj2->addObjectGroup(obj4);

    g1.reset(obj1);
    g2.reset(obj2);
    g3.reset(obj3);
    g4.reset(obj4);

    m_treeModel = new TreeModel(obj1, this);
    m_treeWidget->setModel(m_treeModel);
}

ObjectBase* ModelListWidget::getCurrentWidget()
{
    return m_currentWidget;
}
