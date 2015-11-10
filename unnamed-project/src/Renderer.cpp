#include <iostream>

#include "Renderer.h"
#include "Scene.h"

#include "utility.h"

void Renderer::initialize()
{
    // initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    std::string vertexShaderSource = loadTextFile("shaders/vertex.glsl");
    std::string fragmentShaderSource = loadTextFile("shaders/fragment.glsl");

    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
    }
    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str()))
    {
        std::cerr << "could not load fragment shader" << std::endl;
    }
    m_program.bindAttributeLocation("position", 0);
    if (!m_program.link())
    {
        std::cerr << "could not link shader program" << std::endl;
    }

    m_program.bind();
    m_mvpMatrixLoc = m_program.uniformLocation("mvpMatrix");

    m_program.release();
}

void Renderer::render(Scene *scene)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    m_program.bind();

    QMatrix4x4 view = scene->getProjection() * scene->getCamera();

    for (auto it = scene->objectsBegin(); it != scene->objectsEnd(); it++)
    {
        auto *object = it->get();
        m_program.setUniformValue(m_mvpMatrixLoc, view * object->getWorld());
        object->getModel()->draw();
    }

    m_program.release();
}

void Renderer::resize(int width, int height)
{
    // hm
    // glViewport(0, 0, width, height);
}
