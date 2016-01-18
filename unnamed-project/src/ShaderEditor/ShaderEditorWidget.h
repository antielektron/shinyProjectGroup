#ifndef UNNAMED_PROJECT_SHADER_EDITOR_SHADER_EDITOR_WIDGET_H
#define UNNAMED_PROJECT_SHADER_EDITOR_SHADER_EDITOR_WIDGET_H

#include <memory>

#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColor>

#include "ShaderEditor/CodeEditor.h"
#include "ShaderEditor/GLSLHighlighter.h"

#include "utility.h"
#include "IRenderer.h"

class ShaderEditorWidget : public QDockWidget
{
    Q_OBJECT
public:
    ShaderEditorWidget(QWidget *parent,
                       QOpenGLShader::ShaderTypeBit type,
                       const QString &progName);
    virtual ~ShaderEditorWidget();

    void setColor(const QColor &color);

    static const QColor errorColor;
    static const QColor allFineColor;

protected:
    QVBoxLayout *m_layout;
    CodeEditor *m_editor;

    GLSLHighlighter *m_glslHighlighter;

    QWidget *m_multiWidget;

    QPushButton *m_applyButton;
    QPushButton *m_saveButton;
    QPushButton *m_loadButton;

    bool m_hasChanged;

    QOpenGLShader::ShaderTypeBit m_shaderType;
    QString m_progName;

    void setShaderName(QOpenGLShader::ShaderTypeBit type,
                       const QString &progName);
    void connectStuff();

public slots:

    /**
     * @brief onShaderChanged   notify editor if code is changed not by itself
     *                          (mainly used to initialize default shaders)
     * @param code              new shader code
     */
    void onShaderChanged(const QString &code);

    /**
     * @brief onUpdateRequest   ask whether changes were made since the last
     *                          request (will emit
     *                          ShaderEditorWidget::codeChanged in this case)
     */
    void onUpdateRequest();

protected slots:
    /**
     * @brief onUserChangedText slot for codeEditor
     */
    void onUserChangedText();

    void onApplyClicked();

    void onSaveClicked();

    void onLoadClicked();

signals:
    /**
     * @brief codeChanged   signal will be emitted if apply button is clicked
     *                      or by onUpdateRequest
     * @param code          new shader code
     */
    void codeChanged(const QString &code,
                     QOpenGLShader::ShaderTypeBit type,
                     const QString &progName);

};

#endif // UNNAMED_PROJECT_SHADER_EDITOR_SHADER_EDITOR_WIDGET_H
