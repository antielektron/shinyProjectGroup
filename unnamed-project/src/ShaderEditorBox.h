#ifndef SHADEREDITORBOX_H
#define SHADEREDITORBOX_H


#include <QDockWidget>
#include <QVBoxLayout>
#include <QColor>
#include <QOpenGLFunctions>
#include "utility.h"
#include "CodeEditor.h"
#include "GLSLHighlighter.h"
#include "IRenderer.h"
#include <memory>


class ShaderEditorBox : public QDockWidget
{

    Q_OBJECT
public:

    ShaderEditorBox(IRenderer *renderer, QWidget *parent = nullptr);
    ~ShaderEditorBox();

protected:

    std::unique_ptr<QVBoxLayout> m_layout;
    std::unique_ptr<CodeEditor> m_vsEditor;
    std::unique_ptr<CodeEditor> m_fsEditor;

    std::unique_ptr<GLSLHighlighter> m_highlighterVS;
    std::unique_ptr<GLSLHighlighter> m_highlighterFS;

    std::unique_ptr<QWidget> m_multiWidget;

    static const QColor errorColor;
    static const QColor allFineColor;

    IRenderer *m_renderer;

public slots:
    void createShaderProgram();
    void updateShaderFromRenderer();
};

#endif // SHADEREDITORBOX_H
