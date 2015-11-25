#ifndef UNNAMED_PROJECT_SHADER_EDITOR_SHADER_EDITOR_WIDGET_H
#define UNNAMED_PROJECT_SHADER_EDITOR_SHADER_EDITOR_WIDGET_H

#include <memory>

#include <QDockWidget>
#include <QVBoxLayout>
#include <QColor>
#include <QOpenGLFunctions>

#include "ShaderEditor/CodeEditor.h"
#include "ShaderEditor/GLSLHighlighter.h"

#include "utility.h"
#include "IRenderer.h"

class ShaderEditorWidget : public QDockWidget
{
    Q_OBJECT
public:
    ShaderEditorWidget(IRenderer *renderer, QWidget *parent = nullptr);
    virtual ~ShaderEditorWidget();

protected:
    QVBoxLayout *m_layout;
    CodeEditor *m_vsEditor;
    CodeEditor *m_fsEditor;

    GLSLHighlighter *m_highlighterVS;
    GLSLHighlighter *m_highlighterFS;

    QWidget *m_multiWidget;

    static const QColor errorColor;
    static const QColor allFineColor;

    IRenderer *m_renderer;

public slots:
    void createShaderProgram();
    void updateShaderFromRenderer();
};

#endif // UNNAMED_PROJECT_SHADER_EDITOR_SHADER_EDITOR_WIDGET_H
