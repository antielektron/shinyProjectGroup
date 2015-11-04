#include <iostream>
#include <fstream>

#include "OpenGLWidget.h"

#include "utility.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    // enformce opengl 3.0
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(format);

    // render as fast as possible!
    m_timer.setInterval(16); // ~60fps
    connect(&m_timer, &QTimer::timeout, this, &OpenGLWidget::paintGL);
    m_timer.start();

    start = std::chrono::system_clock::now() + std::chrono::seconds(1);
    frame_count = 0;
}

void OpenGLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);

    // initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    std::string vertexShaderSource = loadTextFile("shader/vertex.glsl");
    std::string fragmentShaderSource = loadTextFile("shader/fragment.glsl");

    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
    }
    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str()))
    {
        std::cerr << "could not link" << std::endl;
    }
    m_program.bindAttributeLocation("position", 0);
    if (!m_program.link())
    {
        std::cerr << "could not link" << std::endl;
    }

    m_program.bind();
    m_mvpMatrixLoc = m_program.uniformLocation("mvpMatrix");

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -10);

    m_program.release();

    m_object.reset(new Model("test.obj"));

    auto version = context()->format().version();
    std::cout << "Using OpenGL Version " << version.first << "." << version.second << std::endl;
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    if (start < std::chrono::system_clock::now())
    {
        std::cout << frame_count << " fps" << std::endl;
        frame_count = 0;
        start += std::chrono::seconds(1);
    }
    frame_count++;

    m_world.setToIdentity();
    //m_world.rotate(180.0f - (0 / 16.0f), 1, 0, 0); // X
    //m_world.rotate(0 / 16.0f, 0, 1, 0); // Y
    //m_world.rotate(0 / 16.0f, 0, 0, 1); // Z

    // QOpenGLVertexArrayObject::Binder vaoBinder(&m_object->getVao());
    m_program.bind();
    m_program.setUniformValue(m_mvpMatrixLoc, m_proj * m_camera * m_world);

    // glDrawArrays(GL_TRIANGLES, 0, m_object.vertexCount());
    // glDrawArrays(GL_TRIANGLES, 0, m_object.vertexCount());
    m_object->draw();

    m_program.release();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    // glViewport(0, 0, width, height);
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(width) / height, 0.01f, 100.0f);
}

void OpenGLWidget::cleanup()
{
    makeCurrent();
    // m_logoVbo.destroy();
    doneCurrent();
}
