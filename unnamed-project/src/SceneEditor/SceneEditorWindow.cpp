#include "SceneEditorWindow.h"

#include <QApplication>
#include <QAction>
#include <QFileDialog>
#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/ObjectDetailsWidget.h"
#include "SceneEditor/ObjectListWidget.h"
#include "SceneEditor/ModelListWidget.h"

#include "OpenGLWidget.h"

//DEBUG
#include <iostream>

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

    createActions();
    createToolbar();

    connectStuff();
}

//------------------------------------------------------------------------------
SceneEditorWindow::~SceneEditorWindow()
{
}

//------------------------------------------------------------------------------
void SceneEditorWindow::createActions()
{
    QStyle *style = QApplication::style();

    m_loadScene = new QAction(style->standardIcon(QStyle::SP_DirOpenIcon),
                              "load Scene",
                              this);
    m_saveScene = new QAction(style->standardIcon(QStyle::SP_DialogSaveButton),
                              "save scene",
                              this);
    m_newScene = new QAction(style->standardIcon(QStyle::SP_FileIcon),
                             "new Scene",
                             this);

    m_loadScene->setShortcut(QKeySequence::Open);
    m_saveScene->setShortcut(QKeySequence::Save);
    m_newScene->setShortcut(QKeySequence::New);
}

//------------------------------------------------------------------------------
void SceneEditorWindow::createToolbar()
{
    m_toolbar = addToolBar("Toolbar");
    m_toolbar->setMovable(true);
    m_toolbar->addAction(m_newScene);
    m_toolbar->addAction(m_loadScene);
    m_toolbar->addAction(m_saveScene);
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

    //connect Actions:
    connect(m_loadScene, SIGNAL(triggered()), this, SLOT(loadScene()));
    connect(m_saveScene, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_newScene, SIGNAL(triggered()), this, SLOT(newScene()));
}

//------------------------------------------------------------------------------
void SceneEditorWindow::loadScene()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("open Level"),
                                                    ".",
                                                    tr("Xml files (*.xml)"));
    if (filename.length() > 0)
    {
        Scene *scene = Scene::loadFromFile(filename).second;
        m_game->initialize(std::unique_ptr<Scene>(scene));
    }

}

//------------------------------------------------------------------------------
void SceneEditorWindow::saveScene()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save Level"),
                                                    ".",
                                                    tr("Xml files (*.xml)"));
    if (filename.length() > 0)
    {
        Scene::SceneInfo info = std::make_tuple("shinyscene","42","norby");
        Scene::saveToFile(m_game->getScene(),filename,info);
    }

}

//------------------------------------------------------------------------------
void SceneEditorWindow::newScene()
{
    m_game->initialize();

}


