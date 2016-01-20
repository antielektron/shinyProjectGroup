#include <iostream>
#include <fstream>

#include "OpenGLWidget.h"
#include "Renderer.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_renderer(new Renderer()),
    m_initialized(false)
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
    m_game(std::move(game)),
    m_initialized(false)
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

    if (m_initialized)
    {
        // TODO makeCurrent!
        m_game->initialize();
    }
}

void OpenGLWidget::setRenderer(std::unique_ptr<RendererBase> renderer)
{
    m_renderer = std::move(renderer);
}

RendererBase *OpenGLWidget::getRenderer()
{
    return m_renderer.get();
}

void OpenGLWidget::initializeGL()
{
    char *glVersion = (char*)glGetString(GL_VERSION);
    if (glVersion)
    {
        std::cout << "OpenGL version: " << glVersion << "\n";
    }

    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        std::cout << "GLEW init failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    }

    GLuint buf;
    glGenRenderbuffers(1, &buf);

    // connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

    m_initialized = true;

    m_renderer->initialize();
    m_game->initialize();

    // auto version = context()->format().version();
    // std::cout << "Using OpenGL Version " << version.first << "." << version.second << std::endl;

    emit glInitEvent();
}

void OpenGLWidget::paintGL()
{
    // TODO do somewhere else!
    if (m_keyManager->shouldCatchMouse())
        this->setCursor(Qt::BlankCursor);
    else
        this->unsetCursor();

    if (start < std::chrono::system_clock::now())
    {
        std::cout << frame_count << " fps" << std::endl;
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

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    // m_game->onMouseButtonDown(event->button());
	this->setFocus();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // m_game->onMouseButtonUp(event->button());
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_keyManager->shouldCatchMouse())
    {
        // This is a "reset" event
        if (event->globalPos() == mapToGlobal(rect().center()))
            return;

        // TODO check if this is fiirst should catch mouse event..
        if (m_prevShouldCatchMouse)
        {
            // "Relative" mouse position
            m_keyManager->mouseMoveRelative(event->pos().x() - this->width() / 2, event->pos().y() - this->height() / 2);
        }

        // Reset mouse position
        QCursor::setPos(mapToGlobal(rect().center()));
    }
    else
    {
        m_keyManager->mouseMoveAbsolute(event->pos().x(), event->pos().y());
    }

    m_prevShouldCatchMouse = m_keyManager->shouldCatchMouse();
}

void OpenGLWidget::keyPressEvent(QKeyEvent * event)
{
	m_keyManager->pressKey(event->key());
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
	m_keyManager->releaseKey(event->key());
}
