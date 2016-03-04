#ifndef UNNAMED_PROJECT_OPENGLWIDGET_H
#define UNNAMED_PROJECT_OPENGLWIDGET_H

#include <GL/glew.h>

#include <QOpenGLWidget>
#include <QTimer>
#include <QKeyEvent>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <memory>
#include <chrono>

#include "IGame.h"
#include "RendererBase.h"
#include "KeyManager.h"

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget *parent = nullptr);
    OpenGLWidget(std::shared_ptr<IGame> game, QWidget *parent = nullptr);

    void setGame(std::shared_ptr<IGame> game);
    void setRenderer(std::unique_ptr<RendererBase> renderer);
    RendererBase* getRenderer();

    float getFps();

signals:
    void fpsUpdate(float fps);

public slots:
    void cleanup();

protected slots:
	void nextFrame();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

	virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

	virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	virtual void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	 
private:
	bool m_initialized;

    std::shared_ptr<IGame> m_game;
    std::unique_ptr<RendererBase> m_renderer;
	std::unique_ptr<KeyManager> m_keyManager;

	std::unique_ptr<QWidget> m_rendererDebugWidget;

	bool m_prevShouldCatchMouse;

    float m_fps;

    QTimer m_timer;
    std::chrono::system_clock::time_point start;
    size_t frame_count;

signals:
    void glInitEvent();

};

#endif // UNNAMED_PROJECT_OPENGLWIDGET_H
