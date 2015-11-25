#include <iostream>

#include "shadereditor/ShaderEditorWidget.h"

const QColor ShaderEditorWidget::allFineColor = QColor::fromRgb(210,255,210);
const QColor ShaderEditorWidget::errorColor = QColor::fromRgb(255,210,210);

//=============================================================================

ShaderEditorWidget::ShaderEditorWidget(IRenderer *renderer, QWidget *parent):
    QDockWidget(parent),
    m_renderer(renderer)
{
    m_multiWidget = new QWidget(this);

    m_layout = new QVBoxLayout(m_multiWidget);
    m_vsEditor = new CodeEditor(m_multiWidget);
    m_fsEditor = new CodeEditor(m_multiWidget);


    m_layout->addWidget(m_vsEditor);
    m_layout->addWidget(m_fsEditor);

    m_multiWidget->setLayout(m_layout);

    this->setWidget(m_multiWidget);

    //set fancy highlighter:
    m_highlighterVS = new GLSLHighlighter(m_vsEditor->document());
    m_highlighterFS = new GLSLHighlighter(m_fsEditor->document());



    m_vsEditor->setPlainText("//vertexShader");
    m_fsEditor->setPlainText("//fragmentShader");

    m_vsEditor->setToolTip("Vertex Shader");
    m_fsEditor->setToolTip("Fragment Shader");

    this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    this->setWindowTitle("Shader Editor");

}

//=============================================================================

ShaderEditorWidget::~ShaderEditorWidget()
{
    //nothing to do here!
}

//=============================================================================

void ShaderEditorWidget::createShaderProgram()
{
    auto errCode = m_renderer->createShaderProgram(
                m_vsEditor->toPlainText().toStdString(),
                m_fsEditor->toPlainText().toStdString());
    switch (errCode)
    {
    case ShaderErrorType::NoError:
    {
        QPalette p = m_vsEditor->palette();
        p.setColor(QPalette::Base, allFineColor);
        m_vsEditor->setPalette(p);
        m_fsEditor->setPalette(p);
        break;
    }
    case ShaderErrorType::VertexShaderError:
    {
        QPalette p = m_vsEditor->palette();
        p.setColor(QPalette::Base, errorColor);
        m_vsEditor->setPalette(p);
        break;
    }
    case ShaderErrorType::FragmentShaderError:
    {
        QPalette p = m_fsEditor->palette();
        p.setColor(QPalette::Base, errorColor);
        m_fsEditor->setPalette(p);
        break;
    }
    case ShaderErrorType::LinkingError:
    {
        std::cout << "LinkingError" << std::endl;
        //TODO: status bar text
    }
    default:
    {
        //TODO: status bar text
        QPalette p = m_vsEditor->palette();
        p.setColor(QPalette::Base, errorColor);
        m_vsEditor->setPalette(p);
        m_fsEditor->setPalette(p);
        break;
    }
    }
}

//=============================================================================

void ShaderEditorWidget::updateShaderFromRenderer()
{
    std::string vs = m_renderer->getVertexShader();
    std::string fs = m_renderer->getFragmentShader();

    m_vsEditor->setPlainText(QString::fromStdString(vs));
    m_fsEditor->setPlainText(QString::fromStdString(fs));
}
