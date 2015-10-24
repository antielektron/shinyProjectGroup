#ifndef QTESTCLION_OPENGLWIDGET_H
#define QTESTCLION_OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <memory>
#include <chrono>

#include "Model.h"

class OpenGLWidget : public QOpenGLWidget
{
public:
    OpenGLWidget(QWidget *parent = nullptr);

public slots:
    void cleanup();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    QTimer m_timer;
    std::chrono::system_clock::time_point start;
    size_t frame_count;

    // Renderer
    QOpenGLShaderProgram m_program;
    int m_mvpMatrixLoc;

    // Scene?
    QMatrix4x4 m_proj;
    QMatrix4x4 m_world;
    QMatrix4x4 m_camera;

    std::unique_ptr<Model> m_object;
};

#endif // QTESTCLION_OPENGLWIDGET_H
