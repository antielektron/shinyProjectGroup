#include <iostream>

#include <QStyle>
#include <QApplication>
#include <QStatusBar>

#include "EditorWindow.h"

//------------------------------------------------------------------------------
EditorWindow::EditorWindow(OpenGLWidget* widget, QWidget *parent) :
    QMainWindow(parent),
    m_glWidget(widget)
{

    setCentralWidget(m_glWidget);
    m_glWidget->setFocus();

    connect(m_glWidget, SIGNAL(glInitEvent()),this, SLOT(onGlInit()));

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::VerticalTabs);
    setMinimumWidth(800);
    setMinimumHeight(600);
}

//------------------------------------------------------------------------------
EditorWindow::~EditorWindow()
{
    //nothing to do here
}

//------------------------------------------------------------------------------
void EditorWindow::createDocks()
{
    IRenderer *renderer = m_glWidget->getRenderer();
    std::vector<std::string> programs;

    renderer->getPrograms(programs);

    ShaderEditorWidget *previousWidget = nullptr;

    for (const auto &prog : programs)
    {
        IRenderer::ShaderSourcesType sources;
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
void EditorWindow::onGlInit()
{
    createDocks();
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
    }
    else
    {
        m_shaderEditorMap[std::make_pair(progName.toStdString(), type)]->setColor(
                    ShaderEditorWidget::errorColor);
    }
}



