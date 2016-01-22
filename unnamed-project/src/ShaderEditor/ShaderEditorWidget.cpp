#include <iostream>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QAction>
#include <QLabel>

#include "ShaderEditor/ShaderEditorWidget.h"

const QColor ShaderEditorWidget::allFineColor = QColor::fromRgb(210,255,210);
const QColor ShaderEditorWidget::errorColor = QColor::fromRgb(255,210,210);

//------------------------------------------------------------------------------
ShaderEditorWidget::ShaderEditorWidget(QWidget *parent,
                                       QOpenGLShader::ShaderTypeBit type,
                                       const QString &progName) :
    QDockWidget(parent)
{
    m_multiWidget = new QWidget(this);

    m_layout = new QVBoxLayout(m_multiWidget);
    m_editor = new CodeEditor(m_multiWidget);

    m_applyButton = new QPushButton(this);
    m_saveButton = new QPushButton(this);
    m_loadButton = new QPushButton(this);

    m_applyButton->setText("apply Shader");
    m_saveButton->setText("save Shader");
    m_loadButton->setText("load Shader");

    m_layout->addWidget(m_editor);
    m_layout->addWidget(m_applyButton);
    m_layout->addWidget(m_saveButton);
    m_layout->addWidget(m_loadButton);

    m_multiWidget->setLayout(m_layout);

    this->setWidget(m_multiWidget);

    //set fancy highlighter:
    m_glslHighlighter = new GLSLHighlighter(m_editor->document());

    this->setFeatures(QDockWidget::DockWidgetMovable
                      | QDockWidget::DockWidgetFloatable
                      | QDockWidget::DockWidgetClosable);

    m_shaderType = type;
    m_progName = progName;
    setShaderName(type, progName);

    m_action = new QAction(m_shaderName, this);
    m_action->setCheckable(true);

    m_hasChanged = false;
    m_isSaved = true;

    m_lastFilename = ".";

    connectStuff();

    setHidden(true);

}

//------------------------------------------------------------------------------
ShaderEditorWidget::~ShaderEditorWidget()
{
    //nothing to do here!
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::setColor(const QColor &color)
{
    QPalette p = m_editor->palette();
    p.setColor(QPalette::Base, color);
    m_editor->setPalette(p);
}

//------------------------------------------------------------------------------
QAction *ShaderEditorWidget::getAction()
{
    return m_action;
}

//------------------------------------------------------------------------------
const QString &ShaderEditorWidget::getFilename()
{
    return m_lastFilename;
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::setShaderName(QOpenGLShader::ShaderTypeBit type,
                                       const QString &progName)
{
    m_shaderName = progName + QString::fromStdString(
                shaderTypeToString.at(type)) + " shader";

    this->setWindowTitle(m_shaderName);
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::connectStuff()
{
    connect(m_editor, SIGNAL(textChanged()), this, SLOT(onUserChangedText()));
    connect(m_applyButton,SIGNAL(clicked()), this, SLOT(onApplyClicked()));
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(m_loadButton, SIGNAL(clicked()), this, SLOT(onLoadClicked()));
    connect(m_action, SIGNAL(changed()), this, SLOT(onActionToggled()));
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::onShaderChanged(const QString &code)
{
    m_editor->setText(code);
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::onUpdateRequest()
{
    if (m_hasChanged)
    {
        emit codeChanged(m_editor->toPlainText(), m_shaderType, m_progName);
        m_hasChanged = false;
    }
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::onActionToggled()
{
    if (m_action->isChecked())
    {
        this->setHidden(false);
        this->raise();
    }
    else
    {
        this->setHidden(true);
    }
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::onUserChangedText()
{
    m_hasChanged = true;
    m_isSaved= false;
    // mark title as changed:
    this->setTitleBarWidget(new QLabel(m_shaderName + "*", this));
    m_action->setText(m_shaderName + "*");

}

//------------------------------------------------------------------------------
void ShaderEditorWidget::onApplyClicked()
{
    if (m_hasChanged)
    {
        emit codeChanged(m_editor->toPlainText(), m_shaderType, m_progName);
        m_hasChanged = false;
    }
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::onSaveClicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save Shader"),
                                                    m_lastFilename,
                                                    tr("GLSL files (*.glsl)"));

    if (filename.length() > 0)
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        if (!file.isOpen())
        {
            QMessageBox::critical(this,
                                  "ERROR",
                                  QString("could not open file ") + filename);
            file.close(); //?
            return;
        }
        QTextStream stream(&file);
        stream << m_editor->toPlainText();
        file.close();

        QString relativePath = QDir::current().relativeFilePath(filename);
        m_lastFilename = relativePath;
        emit filenameChanged(relativePath, m_shaderType, m_progName);

        m_isSaved = true;
        this->setTitleBarWidget(new QLabel(m_shaderName, this));
        m_action->setText(m_shaderName);
    }
}

//------------------------------------------------------------------------------
void ShaderEditorWidget::onLoadClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Load Shader"),
                                                    m_lastFilename,
                                                    tr("GLSL files (*.glsl)"));

    if (filename.length() > 0)
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!file.isOpen())
        {
            QMessageBox::critical(this,
                                  "ERROR",
                                  QString("could not open file ") + filename);
            file.close(); //?
            return;
        }
        QString content(file.readAll());
        m_editor->setPlainText(content);

        // force renderer update
        m_hasChanged = true;
        m_isSaved = true;
        this->setTitleBarWidget(new QLabel(m_shaderName, this));

        QString relativePath = QDir::current().relativeFilePath(filename);
        m_lastFilename = relativePath;
        emit filenameChanged(relativePath, m_shaderType, m_progName);

        m_action->setText(m_shaderName);
        onApplyClicked();
    }
}
