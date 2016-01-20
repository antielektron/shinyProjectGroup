#include <iostream>

#include <QStyle>
#include <QApplication>
#include <QStatusBar>
#include <QFileDialog>

#include "EditorWindow.h"

//------------------------------------------------------------------------------
EditorWindow::EditorWindow(OpenGLWidget* widget, QWidget *parent) :
    QMainWindow(parent),
    m_glWidget(widget)
{

    setCentralWidget(m_glWidget);
    m_glWidget->setFocus();

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::VerticalTabs);
    setMinimumWidth(800);
    setMinimumHeight(600);

    createActions();
    createToolbar();
    connectStuff();
}

//------------------------------------------------------------------------------
EditorWindow::~EditorWindow()
{
    //nothing to do here
}

//------------------------------------------------------------------------------
void EditorWindow::createDocks()
{
    RendererBase *renderer = m_glWidget->getRenderer();
    std::vector<std::string> programs;

    renderer->getPrograms(programs);

    ShaderEditorWidget *previousWidget = nullptr;

    for (const auto &prog : programs)
    {
        RendererBase::ShaderSourcesType sources;
        renderer->getShadersForProgram(prog, sources);

        for (const auto &pair : sources)
        {
            ShaderEditorWidget *editorWidget
                    = new ShaderEditorWidget(this,
                                             pair.first,
                                             QString::fromStdString(prog));
            m_shaderEditorMap[std::make_pair(prog, pair.first)] = editorWidget;

            editorWidget->onShaderChanged(QString::fromStdString(pair.second));

            connect(editorWidget, SIGNAL(codeChanged(const QString &,
                                                     QOpenGLShader::ShaderTypeBit,
                                                     const QString &)),
                    this, SLOT(onShaderChanged(const QString &,
                                               QOpenGLShader::ShaderTypeBit,
                                               const QString &)));

            connect(this, SIGNAL(updateRequest()),
                    editorWidget, SLOT(onUpdateRequest()));

            editorWidget->setAllowedAreas(Qt::LeftDockWidgetArea
                                          | Qt::RightDockWidgetArea);

            addDockWidget(Qt::RightDockWidgetArea, editorWidget);
            if (previousWidget)
            {
                this->tabifyDockWidget(previousWidget, editorWidget);
            }
            previousWidget = editorWidget;
        }
    }
}

//------------------------------------------------------------------------------
void EditorWindow::createActions()
{
    QStyle *style = QApplication::style();
    m_pauseRenderingAction = new QAction(style->standardIcon(QStyle::SP_MediaPause),
                                         "pause rendering",
                                         this);
    m_resumeRenderingAction = new QAction(style->standardIcon(QStyle::SP_MediaPlay),
                                          "resume rendering",
                                          this);
    m_requestSingleFrameAction = new QAction(style->standardIcon(QStyle::SP_MediaSkipForward),
                                             "render Single Frame",
                                             this);
    m_saveShaderConfig = new QAction(style->standardIcon(QStyle::SP_DialogSaveButton),
                                     "save render configuration",
                                     this);
    m_loadShaderConfig = new QAction(style->standardIcon(QStyle::SP_DialogOpenButton),
                                     "load render configuration",
                                     this);
}

//------------------------------------------------------------------------------
void EditorWindow::createToolbar()
{
    m_toolbar = addToolBar("Toolbar");
    m_toolbar->setMovable(true);
    m_toolbar->addAction(m_pauseRenderingAction);
    m_toolbar->addAction(m_resumeRenderingAction);
    m_toolbar->addAction(m_requestSingleFrameAction);
    m_toolbar->addAction(m_loadShaderConfig);
    m_toolbar->addAction(m_saveShaderConfig);
}

//------------------------------------------------------------------------------
void EditorWindow::connectStuff()
{
    connect(m_glWidget, SIGNAL(glInitEvent()),this, SLOT(onGlInit()));

    connect(m_pauseRenderingAction, SIGNAL(triggered()),
            this, SLOT(onPauseRenderingClicked()));
    connect(m_resumeRenderingAction, SIGNAL(triggered()),
            this, SLOT(onResumeRenderingClicked()));
    connect(m_requestSingleFrameAction, SIGNAL(triggered()),
            this, SLOT(onSingleFrameRequestedClicked()));
    connect(m_loadShaderConfig, SIGNAL(triggered()),
            this, SLOT(onLoadShaderConfigClicked()));
    connect(m_saveShaderConfig, SIGNAL(triggered()),
            this, SLOT(onSaveShaderConfigClicked()));
}

//------------------------------------------------------------------------------
void EditorWindow::onGlInit()
{
    createDocks();
}

//------------------------------------------------------------------------------
void EditorWindow::onPauseRenderingClicked()
{
    m_glWidget->getRenderer()->pauseRendering();
}

//------------------------------------------------------------------------------
void EditorWindow::onResumeRenderingClicked()
{
    m_glWidget->getRenderer()->resumeRendering();
}

//------------------------------------------------------------------------------
void EditorWindow::onSingleFrameRequestedClicked()
{
    m_glWidget->getRenderer()->requestSingleFrameRendering();
}

//------------------------------------------------------------------------------
void EditorWindow::onLoadShaderConfigClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "select shader configuration",
                                                    ".",
                                                    tr("xml files (*.xml)"));

    if (filename.length() > 0)
    {
        m_glWidget->getRenderer()->loadConfiguration(filename.toStdString());
    }
}

//------------------------------------------------------------------------------
void EditorWindow::onSaveShaderConfigClicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "save shader configuration",
                                                    ".",
                                                    tr("xml files (*.xml)"));
    if (filename.length() > 0)
    {
        m_glWidget->getRenderer()->loadConfiguration(filename.toStdString());
    }
}

//------------------------------------------------------------------------------
void EditorWindow::onShaderChanged(const QString &src,
                                   QOpenGLShader::ShaderTypeBit type,
                                   const QString &progName)
{
    m_glWidget->getRenderer()->setShaderSource(src.toStdString(),
                                               progName.toStdString(),
                                               type);
    auto res = m_glWidget->getRenderer()->createProgram(progName.toStdString());

    if (res == ShaderErrorType::NoError)
    {
        m_shaderEditorMap[std::make_pair(progName.toStdString(), type)]->setColor(
                    ShaderEditorWidget::allFineColor);

        // just in case rendering is paused: update one frame to see changes
        onSingleFrameRequestedClicked();
    }
    else
    {
        m_shaderEditorMap[std::make_pair(progName.toStdString(), type)]->setColor(
                    ShaderEditorWidget::errorColor);
    }
}



