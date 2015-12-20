// for glDrawBuffers
#define GL_GLEXT_PROTOTYPES 1

#include <iostream>
#include <QtGui/qopenglframebufferobject.h>

#include "Renderer.h"
#include "Scene/Scene.h"

#include "utility.h"

Renderer::Renderer() :
    m_renderFrameBuffer(0),
    m_renderTexture(0),
    m_renderDepthBuffer(0)
{
}

Renderer::~Renderer()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Delete the manually created objects!
    f->glDeleteFramebuffers(1, &m_renderFrameBuffer);
    f->glDeleteTextures(1, &m_renderTexture);
    f->glDeleteRenderbuffers(1, &m_renderDepthBuffer);
}

ShaderErrorType Renderer::createShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
    std::unique_ptr<QOpenGLShaderProgram> prog(new QOpenGLShaderProgram());

    if (!prog->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
        return ShaderErrorType::VertexShaderError;
    }
    if (!prog->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str()))
    {
        std::cerr << "could not load fragment shader" << std::endl;
        return ShaderErrorType::FragmentShaderError;
    }
    prog->bindAttributeLocation("v_position", 0);
    prog->bindAttributeLocation("v_normal", 1);

    if (!prog->link())
    {
        std::cerr << "could not link shader program" << std::endl;
        return ShaderErrorType::LinkingError;
    }

    m_program = std::move(prog);
    m_currentVertexShader = vertexShaderSource;
    m_currentFragmentShader = fragmentShaderSource;

    m_program->bind();
    m_modelViewMatrixLoc = m_program->uniformLocation("modelViewMatrix");
    m_projectionMatrixLoc = m_program->uniformLocation("projectionMatrix");
    m_lightViewMatrixLoc = m_program->uniformLocation("lightViewMatrix");

    m_lightDirectionLoc = m_program->uniformLocation("lightDirection");
    m_lightColorLoc = m_program->uniformLocation("lightColor");

    m_specularColorLoc = m_program->uniformLocation("specularColor");
    m_diffuseColorLoc = m_program->uniformLocation("diffuseColor");
    m_ambientColorLoc = m_program->uniformLocation("ambientColor");

    m_shadowMapSamplerLoc = m_program->uniformLocation("shadowMapSampler");

    m_program->release();

    return ShaderErrorType::NoError;
}

std::string &Renderer::getVertexShader()
{
    return m_currentVertexShader;
}

std::string &Renderer::getFragmentShader()
{
    return m_currentFragmentShader;
}

void Renderer::createComposeProgram()
{
    if (!m_composeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex_copy.glsl"))
    {
        throw std::runtime_error("could not load vertex shader");
    }
    if (!m_composeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment_copy.glsl"))
    {
        throw std::runtime_error("could not load fragment shader");
    }
    m_composeProgram.bindAttributeLocation("v_position", 0);

    if (!m_composeProgram.link())
    {
        throw std::runtime_error("could not link shader program");
    }

    m_composeProgram.bind();

    m_composeSamplerLoc = m_composeProgram.uniformLocation("sampler");

    m_composeProgram.release();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Create quad
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

void Renderer::createShadowMapProgram()
{
    if (!m_shadowMapProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex_shadowmap.glsl"))
    {
        throw std::runtime_error("could not load vertex shader");
    }
    if (!m_shadowMapProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment_shadowmap.glsl"))
    {
        throw std::runtime_error("could not load fragment shader");
    }
    m_shadowMapProgram.bindAttributeLocation("v_position", 0);

    if (!m_shadowMapProgram.link())
    {
        throw std::runtime_error("could not link shader program");
    }

    m_shadowMapProgram.bind();

    m_shadowMapLightViewMatrixLoc = m_shadowMapProgram.uniformLocation("lightViewMatrix");

    m_shadowMapProgram.release();

    // Create ShadowMap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_shadowMapSize = 2048;

    // Create Texture
    f->glGenTextures(1, &m_shadowMapTexture);
    f->glBindTexture(GL_TEXTURE_2D, m_shadowMapTexture);
    // Give an empty image to OpenGL ( the last "0" )
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_shadowMapSize, m_shadowMapSize, 0, GL_RED, GL_UNSIGNED_SHORT, 0);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    // Create DepthBuffer
    f->glGenRenderbuffers(1, &m_shadowMapDepthBuffer);
    f->glBindRenderbuffer(GL_RENDERBUFFER, m_shadowMapDepthBuffer);
    f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_shadowMapSize, m_shadowMapSize);
    f->glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Create FrameBuffer
    f->glGenFramebuffers(1, &m_shadowMapFrameBuffer);
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_shadowMapDepthBuffer);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_shadowMapTexture, 0);
}

void Renderer::initialize()
{
    // initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    createShaderProgram(
                loadTextFile("shaders/vertex.glsl"),
                loadTextFile("shaders/fragment.glsl"));

    createShadowMapProgram();
    createComposeProgram();
}

void Renderer::render(GLuint fbo, Scene *scene)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Render to ShadowMap

    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

    f->glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);
    // Initialize with max depth.
    f->glClearColor(1, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);

    // TODO compute light viewprojection!
    // TODO find any vector orthogonal to light direction
    QMatrix4x4 lightView;
    lightView.ortho(-10, 10, -10, 10, -5, 5);
    lightView.lookAt(scene->getDirectionalLightDirection().normalized(), QVector3D(0, 0, 0), QVector3D(1, 1, 0));

    m_shadowMapProgram.bind();

    for (auto &object : scene->getObjects())
    {
        // TODO
        auto lightModelView = lightView * object->getWorld();

        m_shadowMapProgram.setUniformValue(m_shadowMapLightViewMatrixLoc, lightModelView);

        object->getModel()->draw();
    }

    m_shadowMapProgram.release();


    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_width, m_height);


    // Render to Texture
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);
    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);
    // f->glEnable(GL_CULL_FACE);

    m_program->bind();

    auto lightDir = scene->getCamera() * QVector4D(scene->getDirectionalLightDirection(), 0.);
    lightDir.setW(1.);

    m_program->setUniformValue(m_projectionMatrixLoc, scene->getProjection());
    m_program->setUniformValue(m_lightDirectionLoc, QVector3D(lightDir));
    m_program->setUniformValue(m_lightColorLoc, scene->getLightColor());

    // Bind shadow map
    f->glBindTexture(GL_TEXTURE_2D, m_shadowMapTexture);
    m_program->setUniformValue(m_shadowMapSamplerLoc, 0);

    for (auto &object : scene->getObjects())
    {
        auto cameraModelView = scene->getCamera() * object->getWorld();
        auto lightModelView = lightView * object->getWorld();

        m_program->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        m_program->setUniformValue(m_lightViewMatrixLoc, lightModelView);
        m_program->setUniformValue(m_specularColorLoc, object->getSpecularAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_diffuseColorLoc, object->getDiffuseAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_ambientColorLoc, object->getAmbientAmount() * QVector3D(1., 1., 1.));

        object->getModel()->draw();
    }

    // Unbind shadow map texture
    f->glBindTexture(GL_TEXTURE_2D, 0);

    m_program->release();


    // Render to Screen
    f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_composeProgram.bind();

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_quadVao);

    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    // f->glBindTexture(GL_TEXTURE_2D, m_shadowMapTexture);
    m_composeProgram.setUniformValue(m_composeSamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0

    f->glDrawArrays(GL_QUADS, 0, 4);

    f->glBindTexture(GL_TEXTURE_2D, 0);

    m_composeProgram.release();

}

void Renderer::resize(int width, int height)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_width = width;
    m_height = height;

    f->glDeleteFramebuffers(1, &m_renderFrameBuffer);
    f->glDeleteTextures(1, &m_renderTexture);
    f->glDeleteRenderbuffers(1, &m_renderDepthBuffer);

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
    f->glGenRenderbuffers(1, &m_renderDepthBuffer);
    f->glBindRenderbuffer(GL_RENDERBUFFER, m_renderDepthBuffer);
    f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    f->glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    f->glGenFramebuffers(1, &m_renderFrameBuffer);
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderDepthBuffer);
    // Set "renderTexture" as our colour attachement #0
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTexture, 0);

    // Set the list of draw buffers.
    // GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    // glDrawBuffers(1, drawBuffers); // "1" is the size of DrawBuffers
}
