#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QToolBar>
#include <QAction>

#include "OpenGLWidget.h"


class EditorWindow : public QMainWindow
{

    Q_OBJECT
public:
    EditorWindow(OpenGLWidget* widget, QWidget *parent = nullptr);
    ~EditorWindow();

protected:
    void createActions();
    void createToolbar();
    void createDocks();


    std::unique_ptr<QToolBar> m_toolbar;
    OpenGLWidget* m_glWidget;

    std::unique_ptr<QAction> m_saveScene;
    std::unique_ptr<QAction> m_loadScene;
    std::unique_ptr<QAction> m_newScene;

protected slots:
    void loadScene();
    void saveScene();
    void newScene();


};

#endif // EDITORWINDOW_H
