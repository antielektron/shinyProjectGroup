#include "EditorWindow.h"

#include <iostream>

#include <QStyle>
#include <QApplication>

EditorWindow::EditorWindow(OpenGLWidget* widget, QWidget *parent) :
    QMainWindow(parent),
    m_glWidget(widget)
{
    createActions();
    createToolbar();
    setCentralWidget(m_glWidget);
    m_glWidget->setFocus();
    createDocks();
}

void EditorWindow::createActions()
{
    //create Actions:
    QStyle* style = QApplication::style();

    m_loadScene = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_DirOpenIcon),"load Scene", this));
    m_saveScene = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_DialogSaveButton), "save Scene", this));
    m_newScene = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_FileIcon), "new Scene", this));

    m_loadScene->setShortcut(QKeySequence::Open);
    m_saveScene->setShortcut(QKeySequence::Save);
    m_newScene->setShortcut(QKeySequence::New);

    connect(m_loadScene.get(), SIGNAL(triggered()), this, SLOT(loadScene()));
    connect(m_saveScene.get(), SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_newScene.get(), SIGNAL(triggered()), this, SLOT(newScene()));
}

void EditorWindow::createToolbar()
{
    m_toolbar = std::unique_ptr<QToolBar>(addToolBar("Toolbar"));
    m_toolbar->addAction(m_newScene.get());
    m_toolbar->addAction(m_saveScene.get());
    m_toolbar->addAction(m_loadScene.get());
}

void EditorWindow::createDocks()
{
    //TODO:
    std::cout << "TODO: create dockable glsl code editor widgets" << std::endl;
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

