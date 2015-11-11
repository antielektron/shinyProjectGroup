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
    m_program.bindAttributeLocation("v_position", 0);
    m_program.bindAttributeLocation("v_normal", 1);

    if (!m_program.link())
    {
        std::cerr << "could not link shader program" << std::endl;
    }

    m_program.bind();
    m_modelViewLoc = m_program.uniformLocation("modelView");
    m_projectionLoc = m_program.uniformLocation("projection");
    m_lightDirectionLoc = m_program.uniformLocation("v_lightDirection");
    m_lightColorLoc = m_program.uniformLocation("v_lightColor");
    m_alphaLoc = m_program.uniformLocation("mat_alpha");
    m_specularAmountLoc = m_program.uniformLocation("k_s");
    m_diffuseAmountLoc = m_program.uniformLocation("k_d");

    m_program.release();
}

void Renderer::render(Scene *scene)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    m_program.bind();

    m_program.setUniformValue(m_projectionLoc, scene->getProjection());

    for (auto it = scene->objectsBegin(); it != scene->objectsEnd(); it++)
    {
        auto *object = it->get();
        m_program.setUniformValue(m_modelViewLoc, scene->getCamera() * object->getWorld());
        m_program.setUniformValue(m_lightDirectionLoc, scene->getDirectionalLightDirection());
        m_program.setUniformValue(m_lightColorLoc, scene->getLightColor());
        m_program.setUniformValue(m_alphaLoc, object->getAlpha());
        m_program.setUniformValue(m_specularAmountLoc, object->getSpecularAmount());
        m_program.setUniformValue(m_diffuseAmountLoc, object->getDiffuseAmount());
        object->getModel()->draw();
    }

    m_program.release();
}

void Renderer::resize(int width, int height)
{
    // hm
    // glViewport(0, 0, width, height);
}
