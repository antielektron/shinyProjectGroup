#include "SceneEditorWindow.h"

#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/ObjectDetailsWidget.h"
#include "SceneEditor/ObjectListWidget.h"
#include "SceneEditor/ModelListWidget.h"

#include "OpenGLWidget.h"

//------------------------------------------------------------------------------
SceneEditorWindow::SceneEditorWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setMinimumSize(800, 600);

    m_game = std::make_shared<SceneEditorGame>();

    // QObjects free their children, when freed
    m_glWidget = new OpenGLWidget(m_game, this);

    this->setCentralWidget(m_glWidget);

    m_objectDetails = new ObjectDetailsWidget(m_game, this);
    m_objectList = new ObjectListWidget(m_game, this);
    m_modelList = new ModelListWidget(m_game, this);

    m_objectDetailsDock = new QDockWidget("Object Details", this);
    m_objectDetailsDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_objectDetailsDock->setWidget(m_objectDetails);

    m_objectListDock = new QDockWidget("Object List", this);
    m_objectListDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_objectListDock->setWidget(m_objectList);

    m_modelListDock = new QDockWidget("Model List", this);
    m_modelListDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_modelListDock->setWidget(m_modelList);

    this->addDockWidget(Qt::RightDockWidgetArea, m_objectDetailsDock);
    this->addDockWidget(Qt::RightDockWidgetArea, m_objectListDock);
    this->addDockWidget(Qt::RightDockWidgetArea, m_modelListDock);

    connectStuff();
}

//------------------------------------------------------------------------------
SceneEditorWindow::~SceneEditorWindow()
{
}

//------------------------------------------------------------------------------
void SceneEditorWindow::connectStuff()
{
    connect(m_objectList, SIGNAL(currentObjectChanged(ObjectBase*)),
            m_objectDetails, SLOT(currentObjectChanged(ObjectBase*)));

    connect(m_game.get(), SIGNAL(modelsChanged()),
            m_objectDetails, SLOT(modelsChanged()));

    connect(m_game.get(), SIGNAL(objectsChanged()),
            m_objectList, SLOT(updateModelTree()));


}
