#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QLabel>

#include "OpenGLWidget.h"
#include "ShaderEditor/ShaderEditorWidget.h"
#include "ObjectToolbox.h"


class EditorWindow : public QMainWindow
{

    Q_OBJECT
public:
    EditorWindow(OpenGLWidget* widget, QWidget *parent = nullptr);
    ~EditorWindow();

protected:
    void createActions();
    void createToolbar();
    void createStatusbar();
    void createDocks();

    QToolBar *m_toolbar;
    OpenGLWidget* m_glWidget;

    QAction *m_saveScene;
    QAction *m_loadScene;
    QAction *m_newScene;
    QAction *m_updateShader;
    QStatusBar *m_statusBar;
    QLabel *m_fpsLabel;
    ShaderEditorWidget *m_shaderEditorWidget;
    ObjectToolbox *m_objectToolbox;

protected slots:
    void loadScene();
    void saveScene();
    void newScene();


};

#endif // EDITORWINDOW_H
