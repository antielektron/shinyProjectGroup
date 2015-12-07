#include <iostream>
#include <fstream>

#include "OpenGLWidget.h"
#include "Renderer.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_renderer(new Renderer())
{
    // enformce opengl version
    // QSurfaceFormat format;
    // format.setMajorVersion(4);
    // format.setMinorVersion(1);
    // format.setProfile(QSurfaceFormat::CoreProfile);
    // this->setFormat(format);

    // render as fast as possible!
    m_timer.setInterval(16); // ~60fps
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer.start();

    start = std::chrono::system_clock::now() + std::chrono::seconds(1);
    frame_count = 0;

    this->setMouseTracking(true);
}

OpenGLWidget::OpenGLWidget(std::shared_ptr<IGame> game, QWidget *parent) :
    QOpenGLWidget(parent),
    m_renderer(new Renderer()),
    m_game(std::move(game))
{
    // render as fast as possible!
    m_timer.setInterval(16); // ~60fps
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer.start();

    start = std::chrono::system_clock::now() + std::chrono::seconds(1);
    frame_count = 0;

    this->setMouseTracking(true);

	m_keyManager = std::unique_ptr<KeyManager>(new KeyManager());
	m_game->setKeyManager(m_keyManager.get());
}

void OpenGLWidget::setGame(std::shared_ptr<IGame> game)
{
    m_game = std::move(game);
	m_keyManager = std::unique_ptr<KeyManager>(new KeyManager());
	m_game->setKeyManager(m_keyManager.get());
}

void OpenGLWidget::setRenderer(std::unique_ptr<IRenderer> renderer)
{
    m_renderer = std::move(renderer);
}

IRenderer *OpenGLWidget::getRenderer()
{
    return m_renderer.get();
}

void OpenGLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

    m_renderer->initialize();
    m_game->initialize();

    auto version = context()->format().version();
    std::cout << "Using OpenGL Version " << version.first << "." << version.second << std::endl;

    emit glInitEvent();
}

void OpenGLWidget::paintGL()
{
    if (start < std::chrono::system_clock::now())
    {
        // std::cout << frame_count << " fps" << std::endl;
        frame_count = 0;
        start += std::chrono::seconds(1);
    }
    frame_count++;

    m_game->tick();

    m_renderer->render(this->defaultFramebufferObject(), m_game->getScene());
}

void OpenGLWidget::resizeGL(int width, int height)
{
    m_game->resize(width, height);
    m_renderer->resize(width, height);
}

void OpenGLWidget::cleanup()
{
    makeCurrent();
    // m_logoVbo.destroy();
    doneCurrent();
}

void OpenGLWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
	m_game->onDoubleClick();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_game->onMouseButtonDown(event->button());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_game->onMouseButtonUp(event->button());
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // This is a "reset" event
    if (event->globalPos() == mapToGlobal(rect().center()))
        return;

    std::cout << event->globalPos().x() - mapToGlobal(rect().center()).x() << " " << event->globalPos().y() - mapToGlobal(rect().center()).y() << std::endl;

    // Reset mouse position
    // this->cursor().setPos(mapToGlobal(QPoint(this->width()/2, this->height()/2)));
    m_keyManager->mouseMove(event->pos().x() - this->width()/2, event->pos().y() - this->height()/2);

    QCursor::setPos(mapToGlobal(rect().center()));
}

void OpenGLWidget::keyPressEvent(QKeyEvent * event)
{
	m_keyManager->pressKey(event->key());
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
	m_keyManager->releaseKey(event->key());
}
