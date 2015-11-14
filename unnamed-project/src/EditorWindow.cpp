#include "EditorWindow.h"

#include <iostream>

#include <QStyle>
#include <QApplication>
#include <QStatusBar>

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

    connect(m_glWidget, SIGNAL(glInitEvent()),m_shaderEditorBox.get(), SLOT(updateShaderFromRenderer()));

    setMinimumWidth(800);
    setMinimumHeight(600);
}

void EditorWindow::createActions()
{
    //create Actions:
    QStyle* style = QApplication::style();

    m_loadScene = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_DirOpenIcon),"load Scene", this));
    m_saveScene = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_DialogSaveButton), "save Scene", this));
    m_newScene = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_FileIcon), "new Scene", this));
    m_updateShader = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_BrowserReload), "update Shader",this));

    m_loadScene->setShortcut(QKeySequence::Open);
    m_saveScene->setShortcut(QKeySequence::Save);
    m_newScene->setShortcut(QKeySequence::New);
    m_updateShader->setShortcut(QKeySequence::Refresh);

    connect(m_loadScene.get(), SIGNAL(triggered()), this, SLOT(loadScene()));
    connect(m_saveScene.get(), SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_newScene.get(), SIGNAL(triggered()), this, SLOT(newScene()));
    connect(m_updateShader.get(), SIGNAL(triggered()), m_shaderEditorBox.get(), SLOT(createShaderProgram()));
}

void EditorWindow::createToolbar()
{
    m_toolbar = std::unique_ptr<QToolBar>(addToolBar("Toolbar"));
    m_toolbar->setMovable(false);
    m_toolbar->addAction(m_newScene.get());
    m_toolbar->addAction(m_saveScene.get());
    m_toolbar->addAction(m_loadScene.get());
    m_toolbar->addAction(m_updateShader.get());
}

void EditorWindow::createDocks()
{
    m_shaderEditorBox = std::unique_ptr<ShaderEditorBox>(
                new ShaderEditorBox(m_glWidget->getRenderer()));
    m_shaderEditorBox->setAllowedAreas(
                Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea,m_shaderEditorBox.get());
}

void EditorWindow::createStatusbar()
{
    m_statusBar = std::unique_ptr<QStatusBar>(new QStatusBar(this));
    m_fpsLabel = std::unique_ptr<QLabel>(new QLabel("fps: TODO", this));
    m_statusBar->addWidget(m_fpsLabel.get());
    setStatusBar(m_statusBar.get());
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

