#include <iostream>
#include <fstream>

#include "OpenGLWidget.h"
#include "Renderer.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_renderer(new Renderer()),
    m_rendererDebugWidget(m_renderer->createDebugWidget()),
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
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    m_timer.start();

    start = std::chrono::system_clock::now() + std::chrono::seconds(1);
    frame_count = 0;

    this->setMouseTracking(true);
}

OpenGLWidget::OpenGLWidget(std::shared_ptr<IGame> game, QWidget *parent) :
    QOpenGLWidget(parent),
    m_renderer(new Renderer()),
    m_rendererDebugWidget(m_renderer->createDebugWidget()),
    m_game(std::move(game)),
    m_initialized(false)
{
    // render as fast as possible!
    m_timer.setInterval(16); // ~60fps
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
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
    m_rendererDebugWidget.reset(m_renderer->createDebugWidget());
}

RendererBase *OpenGLWidget::getRenderer()
{
    return m_renderer.get();
}

float OpenGLWidget::getFps()
{
    return m_fps;
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

    // update shader ( results in double shader program initialization,
    // (but the shader config can also just replace specific shaders)
    m_renderer->loadConfiguration(
                m_game->getScene()->getShaderConfigFile().toStdString());

    // auto version = context()->format().version();
    // std::cout << "Using OpenGL Version " << version.first << "." << version.second << std::endl;

    emit glInitEvent();
}

void OpenGLWidget::paintGL()
{
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

void OpenGLWidget::nextFrame()
{
    // TODO do somewhere else!
    if (m_keyManager->shouldCatchMouse())
        this->setCursor(Qt::BlankCursor);
    else
        this->unsetCursor();

    m_game->tick();

    if (start < std::chrono::system_clock::now())
    {
        m_fps = frame_count;
        emit fpsUpdate(m_fps);
        frame_count = 0;
        start += std::chrono::seconds(1);
    }
    frame_count++;

    // repaint using opengl
    update();
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
        QPoint center = rect().center();

        // This is a "reset" event
        if (event->globalPos() == mapToGlobal(center))
            return;

        // TODO check if this is fiirst should catch mouse event..
        if (m_prevShouldCatchMouse)
        {
            // "Relative" mouse position
            m_keyManager->mouseMoveRelative(event->pos().x() - center.x(), event->pos().y() - center.y());
        }

        // Reset mouse position
        QCursor::setPos(mapToGlobal(center));
    }
    else
    {
        m_keyManager->mouseMoveAbsolute(event->pos().x(), event->pos().y());
    }

    m_prevShouldCatchMouse = m_keyManager->shouldCatchMouse();
}

void OpenGLWidget::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_F1)
    {
        m_rendererDebugWidget->show();
    }

	m_keyManager->pressKey(event->key());
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
	m_keyManager->releaseKey(event->key());
}
