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
#include "RendererBase.h"

class ShaderEditorWidget : public QDockWidget
{
    Q_OBJECT
public:
    ShaderEditorWidget(QWidget *parent,
                       QOpenGLShader::ShaderTypeBit type,
                       const QString &progName);
    virtual ~ShaderEditorWidget();

    void setColor(const QColor &color);

    QAction *getAction();

    const QString &getFilename();

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

    QAction *m_action;

    bool m_hasChanged;
    bool m_isSaved;

    QOpenGLShader::ShaderTypeBit m_shaderType;
    QString m_progName;

    QString m_shaderName;

    QString m_lastFilename;

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

    void onActionToggled();

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

    void filenameChanged(const QString &filename,
                         QOpenGLShader::ShaderTypeBit type,
                         const QString &progName);

};

#endif // UNNAMED_PROJECT_SHADER_EDITOR_SHADER_EDITOR_WIDGET_H
