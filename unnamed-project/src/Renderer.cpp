// for glDrawBuffers
#define GL_GLEXT_PROTOTYPES 1

#include <iostream>
#include <QtGui/qopenglframebufferobject.h>

#include "Renderer.h"
#include "Scene.h"

#include "utility.h"

Renderer::Renderer() :
    m_frameBuffer(0),
    m_renderTexture(0),
    m_depthBuffer(0)
{
}

Renderer::~Renderer()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Delete the manually created objects!
    f->glDeleteFramebuffers(1, &m_frameBuffer);
    f->glDeleteTextures(1, &m_renderTexture);
    f->glDeleteRenderbuffers(1, &m_depthBuffer);
}

OpenglErrorType Renderer::createShaderProgram(std::string vs, std::string fs)
{
    OpenglErrorType retVal = OpenglErrorType::noError;

    std::string vertexShaderSource = vs;
    std::string fragmentShaderSource = fs;

    std::unique_ptr<QOpenGLShaderProgram> prog(
                new QOpenGLShaderProgram());

    if (!prog->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
        retVal = OpenglErrorType::vertexShaderError;
    }
    if (!prog->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str()))
    {
        std::cerr << "could not load fragment shader" << std::endl;
        retVal = OpenglErrorType::fragmentShaderError;
    }
    prog->bindAttributeLocation("v_position", 0);
    prog->bindAttributeLocation("v_normal", 1);

    if (!prog->link())
    {
        std::cerr << "could not link shader program" << std::endl;
        retVal = OpenglErrorType::linkingError;
    }

    if (retVal == OpenglErrorType::noError)
    {
        m_program = std::move(prog);
        m_currentVertexShader = vertexShaderSource;
        m_currentFragmentShader = fragmentShaderSource;

        m_program->bind();
        m_modelViewLoc = m_program->uniformLocation("modelView");
        m_projectionLoc = m_program->uniformLocation("projection");
        m_lightDirectionLoc = m_program->uniformLocation("v_lightDirection");
        m_lightColorLoc = m_program->uniformLocation("v_lightColor");
        m_alphaLoc = m_program->uniformLocation("mat_alpha");
        m_specularAmountLoc = m_program->uniformLocation("k_s");
        m_diffuseAmountLoc = m_program->uniformLocation("k_d");

        m_program->release();
    }

    return retVal;
}

std::string &Renderer::getVertexShader()
{
    return m_currentVertexShader;
}

std::string &Renderer::getFragmentShader()
{
    return m_currentFragmentShader;
}

void Renderer::initialize()
{
    // initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    createShaderProgram(
                loadTextFile("shaders/vertex.glsl"),
                loadTextFile("shaders/fragment.glsl"));

    // compose shader

    std::string composeVertexShaderSource = loadTextFile("shaders/vertex_copy.glsl");
    std::string composeFragmentShaderSource = loadTextFile("shaders/fragment_copy.glsl");

    if (!m_composeProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, composeVertexShaderSource.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
    }
    if (!m_composeProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, composeFragmentShaderSource.c_str()))
    {
        std::cerr << "could not load fragment shader" << std::endl;
    }
    m_composeProgram.bindAttributeLocation("v_position", 0);

    if (!m_composeProgram.link())
    {
        std::cerr << "could not link shader program" << std::endl;
    }

    m_composeProgram.bind();

    m_samplerLoc = m_composeProgram.uniformLocation("sampler");

    m_composeProgram.release();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // create quad
    m_quadVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_quadVao);

    float vertices[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1
    };

    // Setup our vertex buffer object.
    m_quadVbo.create();
    m_quadVbo.bind();
    m_quadVbo.allocate(vertices, 4*2*sizeof(float));

    // Store the vertex attribute bindings for the program.
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    m_quadVbo.release();
}

void Renderer::render(GLuint fbo, Scene *scene)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // render to texture!
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);
    // f->glEnable(GL_CULL_FACE);

    m_program->bind();

    m_program->setUniformValue(m_projectionLoc, scene->getProjection());

    for (auto it = scene->objectsBegin(); it != scene->objectsEnd(); it++)
    {
        auto *object = it->get();
        m_program->setUniformValue(m_modelViewLoc, scene->getCamera() * object->getWorld());
        m_program->setUniformValue(m_lightDirectionLoc, scene->getDirectionalLightDirection());
        m_program->setUniformValue(m_lightColorLoc, scene->getLightColor());
        m_program->setUniformValue(m_alphaLoc, object->getAlpha());
        m_program->setUniformValue(m_specularAmountLoc, object->getSpecularAmount());
        m_program->setUniformValue(m_diffuseAmountLoc, object->getDiffuseAmount());
        object->getModel()->draw();
    }

    m_program->release();

    // render to screen....
    f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);

    m_composeProgram.bind();

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_quadVao);

    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    m_composeProgram.setUniformValue(m_samplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0

    f->glDrawArrays(GL_QUADS, 0, 4);

    f->glBindTexture(GL_TEXTURE_2D, 0);

    m_composeProgram.release();

}

void Renderer::resize(int width, int height)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_width = width;
    m_height = height;

    f->glDeleteFramebuffers(1, &m_frameBuffer);
    f->glDeleteTextures(1, &m_renderTexture);
    f->glDeleteRenderbuffers(1, &m_depthBuffer);

    // hm
    // glViewport(0, 0, width, height);

    // Create texture
    f->glGenTextures(1, &m_renderTexture);
    f->glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    // Give an empty image to OpenGL ( the last "0" )
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // Poor filtering. Needed!
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    // Create depth buffer!
    f->glGenRenderbuffers(1, &m_depthBuffer);
    f->glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    f->glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    f->glGenFramebuffers(1, &m_frameBuffer);
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
    // Set "renderTexture" as our colour attachement #0
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTexture, 0);

    // Set the list of draw buffers.
    // GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    // glDrawBuffers(1, drawBuffers); // "1" is the size of DrawBuffers
}
