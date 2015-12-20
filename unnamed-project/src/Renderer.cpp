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

    /*
    // normals after projection
    QVector4D normals[4] = {
            {-1., 0., 0., -1.},
            { 1., 0., 0., -1.},
            { 0., 1., 0., -1.},
            { 0.,-1., 0., -1.},
    };
    // n*x <= 0 iff x in view frustum

    QVector4D normalsWorld[4];
    for (int i = 0; i < 4; i++)
        normalsWorld[i] = viewProjection.transposed() * normals[i];

    float nearZ = 1000., farZ = 0.;

    // TODO compute view frustum (in world coordinates) nonononono (pseudo infinite far plane is not such a good idea..)
    for (auto &object : scene->getObjects())
    {
        auto minCorner = object->getModel()->getMinCorner();
        auto maxCorner = object->getModel()->getMaxCorner();

        // TODO consider hyperplanes
        QMatrix4x4 transformation = viewProjection * object->getWorld();
        QVector4D corners[8];
        QVector4D cornersWorld[8];
        for (int i = 0; i < 8; i++)
        {
            auto inputCorner = QVector4D(i & 1 ? minCorner[0] : maxCorner[0],
                                         i & 2 ? minCorner[1] : maxCorner[1],
                                         i & 4 ? minCorner[2] : maxCorner[2], 1.);
            corners[i] = transformation * inputCorner;
            cornersWorld[i] = object->getWorld() * inputCorner;
        }

        bool visible = true;
        // see out-codes
        for (int i = 0; i < 4; i++)
        {
            bool violated = true;
            for (int j = 0; j < 8; j++)
            {
                float f1 = QVector4D::dotProduct(normals[i], corners[j]);
                float f2 = QVector4D::dotProduct(normalsWorld[i], cornersWorld[j]);

                if (f1 <= 0.) // not violated
                {
                    violated = false;
                    break;
                }
            }

            if (violated)
            {
                visible = false;
                break;
            }
        }

        if (visible)
        {
            // TODO extend view frustum Z
            for (int i = 0; i < 8; i++)
            {
                if (corners[i].z() < nearZ)
                    nearZ = corners[i].z();
                if (corners[i].z() > farZ);
                    farZ = corners[i].z();
            }
        }
    }
    */

    // Render to ShadowMap

    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

    f->glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);
    // Initialize with max depth.
    f->glClearColor(1, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glEnable(GL_DEPTH_TEST);

    // TODO rotate light direction to -z

    QVector3D sourceDir = scene->getDirectionalLightDirection().normalized();
    QVector3D targetDir = QVector3D(0., 0., 1.);

    QVector3D lightRotationAxis = QVector3D::crossProduct(sourceDir, targetDir);
    float lightRotationAngle = std::acos(QVector3D::dotProduct(sourceDir, targetDir))*180./M_PI;

    QMatrix4x4 lightViewRotation;
    lightViewRotation.rotate(lightRotationAngle, lightRotationAxis);

    // Compute viewFrustum of camera in light view
    auto inverseViewProjection = (scene->getProjection() * scene->getCamera()).inverted();
    auto screenToLightTransformation = lightViewRotation * inverseViewProjection;

    QVector3D frustumCorners[8] = {
            {-1., -1., -1.},
            { 1., -1., -1.},
            {-1.,  1., -1.},
            { 1.,  1., -1.},
            {-1., -1.,  1.},
            { 1., -1.,  1.},
            {-1.,  1.,  1.},
            { 1.,  1.,  1.}
    };

    // TODO compute MINIMAL bounding box of frustumCorners
    QVector3D minCorner;
    QVector3D maxCorner;

    for (int i = 0; i < 8; i++)
    {
        QVector4D res = screenToLightTransformation * QVector4D(frustumCorners[i], 1.);
        frustumCorners[i] = QVector3D(res / res.w());

        if (i == 0)
        {
            minCorner = maxCorner = frustumCorners[i];
        }
        else
        {
            for (int j = 0; j < 3; j++)
            {
                if (minCorner[j] > frustumCorners[i][j])
                    minCorner[j] = frustumCorners[i][j];
                if (maxCorner[j] < frustumCorners[i][j])
                    maxCorner[j] = frustumCorners[i][j];
            }
        }
    }

    // compute light viewprojection
    QMatrix4x4 lightViewProjection;
    lightViewProjection.ortho(minCorner.x(), maxCorner.x(), minCorner.y(), maxCorner.y(), minCorner.z(), maxCorner.z());

    auto lightView = lightViewProjection * lightViewRotation;

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

    for (auto editorObject : scene->getEditorObjects())
    {
        if (!(static_cast<EditorObject *>(editorObject)->isVisible()))
        {
            continue;
        }
        auto cameraModelView = scene->getCamera() * editorObject->getWorld();
        auto lightModelView  = lightView * editorObject->getWorld();

        m_program->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        m_program->setUniformValue(m_lightViewMatrixLoc, lightModelView);
        m_program->setUniformValue(m_specularColorLoc, editorObject->getSpecularAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_diffuseColorLoc, editorObject->getDiffuseAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_ambientColorLoc, editorObject->getAmbientAmount() * QVector3D(1., 1., 1.));

        editorObject->getModel()->draw();
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
