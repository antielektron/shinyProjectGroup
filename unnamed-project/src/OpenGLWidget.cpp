#include <iostream>
#include <fstream>

#include "OpenGLWidget.h"

#include "utility.h"

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
}

void OpenGLWidget::setGame(std::shared_ptr<IGame> game)
{
    m_game = std::move(game);
}

void OpenGLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

    m_renderer->initialize();
    m_game->initialize();

    auto version = context()->format().version();
    std::cout << "Using OpenGL Version " << version.first << "." << version.second << std::endl;
}

void OpenGLWidget::paintGL()
{
    if (start < std::chrono::system_clock::now())
    {
        std::cout << frame_count << " fps" << std::endl;
        frame_count = 0;
        start += std::chrono::seconds(1);
    }
    frame_count++;

    m_game->tick();

    m_renderer->render(m_game->getScene());


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
