#ifndef UNNAMED_PROJECT_OPENGLWIDGET_H
#define UNNAMED_PROJECT_OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <memory>
#include <chrono>

#include "IGame.h"
#include "IRenderer.h"

class OpenGLWidget : public QOpenGLWidget
{
public:
    OpenGLWidget(QWidget *parent = nullptr);
    OpenGLWidget(std::shared_ptr<IGame> game, QWidget *parent = nullptr);

    void setGame(std::shared_ptr<IGame> game);
    void setRenderer(std::unique_ptr<IRenderer> renderer);

public slots:
    void cleanup();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    std::shared_ptr<IGame> m_game;
    std::unique_ptr<IRenderer> m_renderer;

    QTimer m_timer;
    std::chrono::system_clock::time_point start;
    size_t frame_count;
};

#endif // UNNAMED_PROJECT_OPENGLWIDGET_H
