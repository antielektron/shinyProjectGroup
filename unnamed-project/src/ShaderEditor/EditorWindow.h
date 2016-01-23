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

class EditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    EditorWindow(OpenGLWidget* widget, QWidget *parent = nullptr);
    ~EditorWindow();

public slots:
    void onGlInit();
    void onShaderChanged(const QString & src,
                         QOpenGLShader::ShaderTypeBit type,
                         const QString &progName);

    void onPauseRenderingClicked();
    void onResumeRenderingClicked();
    void onSingleFrameRequestedClicked();
    void onLoadShaderConfigClicked();
    void onSaveShaderConfigClicked();
    void onFilenameChanged(const QString &filename,
                           QOpenGLShader::ShaderTypeBit type,
                           const QString &progName);

signals:
    void updateRequest();

protected:
    void createDocks();
    void createActions();
    void createToolbar();
    void connectStuff();

    OpenGLWidget *m_glWidget;

    QToolBar    *m_toolbar;
    QAction     *m_pauseRenderingAction;
    QAction     *m_resumeRenderingAction;
    QAction     *m_requestSingleFrameAction;
    QAction     *m_saveShaderConfig;
    QAction     *m_loadShaderConfig;

    std::map<RendererBase::ShaderSourcesKeyType,
             ShaderEditorWidget*> m_shaderEditorMap;

    // for actions:


};

#endif // UNNAMED_PROJECT_EDITOR_WINDOW_H
