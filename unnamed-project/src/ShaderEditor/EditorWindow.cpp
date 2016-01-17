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

            connect(editorWidget, SIGNAL(codeChanged(const QString &code,
                                                     QOpenGLShader::ShaderTypeBit type,
                                                     const QString &progName)),
                    this, SLOT(onShaderChanged(const QString &src,
                                               QOpenGlShader::ShaderTypeBit type,
                                               const QString &progName)));

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
                                   const QString &prog)
{
    m_glWidget->getRenderer()->setShaderSource(src.toStdString(),
                                               prog.toStdString(),
                                               type);
    m_glWidget->getRenderer()->createProgram(prog.toStdString());
}



