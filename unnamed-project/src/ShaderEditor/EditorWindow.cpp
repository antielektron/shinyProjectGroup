#include <iostream>

#include <QStyle>
#include <QApplication>
#include <QStatusBar>

#include "EditorWindow.h"

EditorWindow::EditorWindow(OpenGLWidget* widget, QWidget *parent) :
    QMainWindow(parent),
    m_glWidget(widget)
{

    createStatusbar();
    setCentralWidget(m_glWidget);
    m_glWidget->setFocus();

    createDocks();

    createActions();
    createToolbar();

    connect(m_glWidget, SIGNAL(glInitEvent()),m_shaderEditorWidget, SLOT(updateShaderFromRenderer()));

    setMinimumWidth(800);
    setMinimumHeight(600);
}

void EditorWindow::createActions()
{
    //create Actions:
    QStyle* style = QApplication::style();

    m_loadScene = new QAction(style->standardIcon(QStyle::SP_DirOpenIcon),"load Scene", this);
    m_saveScene = new QAction(style->standardIcon(QStyle::SP_DialogSaveButton), "save Scene", this);
    m_newScene = new QAction(style->standardIcon(QStyle::SP_FileIcon), "new Scene", this);
    m_updateShader = new QAction(style->standardIcon(QStyle::SP_BrowserReload), "update Shader",this);

    m_loadScene->setShortcut(QKeySequence::Open);
    m_saveScene->setShortcut(QKeySequence::Save);
    m_newScene->setShortcut(QKeySequence::New);
    m_updateShader->setShortcut(QKeySequence::Refresh);

    connect(m_loadScene, SIGNAL(triggered()), this, SLOT(loadScene()));
    connect(m_saveScene, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_newScene, SIGNAL(triggered()), this, SLOT(newScene()));
    connect(m_updateShader, SIGNAL(triggered()), m_shaderEditorWidget, SLOT(createShaderProgram()));
}

void EditorWindow::createToolbar()
{
    m_toolbar = addToolBar("Toolbar");
    m_toolbar->setMovable(false);
    m_toolbar->addAction(m_newScene);
    m_toolbar->addAction(m_saveScene);
    m_toolbar->addAction(m_loadScene);
    m_toolbar->addAction(m_updateShader);
}

void EditorWindow::createDocks()
{
    m_shaderEditorWidget = new ShaderEditorWidget(m_glWidget->getRenderer(), this);
    m_shaderEditorWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_shaderEditorWidget);

    //create object Toolbox:
    m_objectToolbox = new ObjectToolbox(this);
    m_objectToolbox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_objectToolbox);

    this->tabifyDockWidget(m_shaderEditorWidget, m_objectToolbox);
}

void EditorWindow::createStatusbar()
{
    m_statusBar = new QStatusBar(this);
    m_fpsLabel = new QLabel("fps: TODO", this);
    m_statusBar->addWidget(m_fpsLabel);
    setStatusBar(m_statusBar);
}

void EditorWindow::loadScene()
{
    std::cout << "TODO: load Scene" << std::endl;
}

void EditorWindow::saveScene()
{
    std::cout << "TODO: save the current Scene" << std::endl;
}

void EditorWindow::newScene()
{
    std::cout << "TODO: create a new Scene" << std::endl;
}

EditorWindow::~EditorWindow()
{
    //nothing to do here
}

