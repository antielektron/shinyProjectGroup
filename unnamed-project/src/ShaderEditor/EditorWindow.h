#ifndef UNNAMED_PROJECT_EDITOR_WINDOW_H
#define UNNAMED_PROJECT_EDITOR_WINDOW_H

#include <memory>

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <map>

#include "OpenGLWidget.h"
#include "ShaderEditor/ShaderEditorWidget.h"
#include "ShaderEditor/ObjectToolbox.h"

namespace std
{
    template<>
    struct less<std::pair<std::string, QOpenGLShader::ShaderTypeBit>>
    {
        typedef std::pair<std::string, QOpenGLShader::ShaderTypeBit> type;

        bool operator () (const type &lhs, const type &rhs)
        {
            if (lhs.first == rhs.first)
            {
                return lhs.second < rhs.second;
            }
            else
            {
                return lhs.first < rhs.first;
            }
        }
    };
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    EditorWindow(OpenGLWidget* widget, QWidget *parent = nullptr);
    ~EditorWindow();

protected slots:
    void onGlInit();
    void onShaderChanged(const QString &src,
                         QOpenGLShader::ShaderTypeBit type,
                         const QString &prog);

signals:
    void updateRequest();

protected:
    void createDocks();

    OpenGLWidget* m_glWidget;

    std::map<IRenderer::ShaderSourcesKeyType,
             ShaderEditorWidget*> m_shaderEditorMap;


};

#endif // UNNAMED_PROJECT_EDITOR_WINDOW_H
