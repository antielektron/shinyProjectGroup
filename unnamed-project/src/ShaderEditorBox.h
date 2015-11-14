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

    QVBoxLayout* m_layout;
    CodeEditor* m_vsEditor;
    CodeEditor* m_fsEditor;

    GLSLHighlighter* m_highlighterVS;
    GLSLHighlighter* m_highlighterFS;

    QWidget* m_multiWidget;

    static const QColor errorColor;
    static const QColor allFineColor;

    IRenderer *m_renderer;

public slots:
    void createShaderProgram();
    void updateShaderFromRenderer();
};

#endif // SHADEREDITORBOX_H
