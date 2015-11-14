#include "ShaderEditorBox.h"
#include <iostream>
#include <QLabel>

const QColor ShaderEditorBox::allFineColor = QColor::fromRgb(210,255,210);
const QColor ShaderEditorBox::errorColor = QColor::fromRgb(255,210,210);

//=============================================================================

ShaderEditorBox::ShaderEditorBox(IRenderer *renderer, QWidget *parent):
    QDockWidget(parent),
    m_renderer(renderer)
{
    m_multiWidget = std::unique_ptr<QWidget>(new QWidget(this));

    m_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(m_multiWidget.get()));
    m_vsEditor = std::unique_ptr<CodeEditor>(new CodeEditor(m_multiWidget.get()));
    m_fsEditor = std::unique_ptr<CodeEditor>(new CodeEditor(m_multiWidget.get()));


    m_layout->addWidget(m_vsEditor.get());
    m_layout->addWidget(m_fsEditor.get());

    m_multiWidget->setLayout(m_layout.get());

    this->setWidget(m_multiWidget.get());

    //set fancy highlighter:
    m_highlighterVS = std::unique_ptr<GLSLHighlighter>(
                new GLSLHighlighter(m_vsEditor->document()));
    m_highlighterFS = std::unique_ptr<GLSLHighlighter>(
                new GLSLHighlighter(m_fsEditor->document()));


    m_vsEditor->setPlainText("//vertexShader");
    m_fsEditor->setPlainText("//fragmentShader");

    m_vsEditor->setToolTip("Vertex Shader");
    m_fsEditor->setToolTip("Fragment Shader");

    this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    this->setTitleBarWidget(new QLabel("shader Editor"));

}

//=============================================================================

ShaderEditorBox::~ShaderEditorBox()
{
    //nothing to do here!
}

//=============================================================================

void ShaderEditorBox::createShaderProgram()
{
    OpenglErrorType errCode;
    errCode = m_renderer->createShaderProgram(
                m_vsEditor->toPlainText().toStdString(),
                m_fsEditor->toPlainText().toStdString());
    switch(errCode)
    {
    case OpenglErrorType::noError:
    {
        QPalette p = m_vsEditor->palette();
        p.setColor(QPalette::Base, allFineColor);
        m_vsEditor->setPalette(p);
        m_fsEditor->setPalette(p);
        break;
    }
    case OpenglErrorType::vertexShaderError:
    {
        QPalette p = m_vsEditor->palette();
        p.setColor(QPalette::Base, errorColor);
        m_vsEditor->setPalette(p);
        break;
    }
    case OpenglErrorType::fragmentShaderError:
    {
        QPalette p = m_fsEditor->palette();
        p.setColor(QPalette::Base, errorColor);
        m_fsEditor->setPalette(p);
        break;
    }
    case OpenglErrorType::linkingError:
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

void ShaderEditorBox::updateShaderFromRenderer()
{
    std::string vs = m_renderer->getVertexShader();
    std::string fs = m_renderer->getFragmentShader();

    m_vsEditor->setPlainText(QString::fromStdString(vs));
    m_fsEditor->setPlainText(QString::fromStdString(fs));
}

