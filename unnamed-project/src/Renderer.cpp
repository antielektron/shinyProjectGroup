// for glDrawBuffers
#define GL_GLEXT_PROTOTYPES 1
#define _USE_MATH_DEFINES

#include <GL/glew.h>

#include <iostream>
#include <cmath>
#include <QtGui/qopenglframebufferobject.h>
#include <vector>
#include <chrono>

#include "Renderer.h"
#include "Scene/Scene.h"

#include "RendererDebugWidget.h"

#include "MathUtility.h"

#include "utility.h"


Renderer::Renderer() : RendererBase(),
        m_renderFrameBuffer(0),
        m_renderTexture(0),
        m_voMomentsTexture(0),
        m_renderDepthBuffer(0),
        // Options
        m_colorizeCascades(false),
        m_cascadedShadowMapsLambda(1),
        m_cascadeStrategy(SampleDistributionShadowMaps),
        m_shadowMapSampleCount(1),
        m_filterShadowMap(false),
        m_renderLightView(false),
        m_renderMomentView(false),
        m_captureRequested(false),
        m_captureEnabled(false),
        m_captureSlot(0),
        m_lightShearing(false),
        m_samples(10),
        m_plainObscurance(0),
        m_sky(1),
        m_glLinear(true),
        m_mmLinear(true),
        m_cursor(true)
{
    // nothing to do here
}

//------------------------------------------------------------------------------

Renderer::~Renderer()
{
    // Delete the manually created objects!
    glDeleteFramebuffers(1, &m_renderFrameBuffer);
    glDeleteTextures(1, &m_renderTexture);
    glDeleteTextures(1, &m_voMomentsTexture);
    glDeleteTextures(1, &m_renderDepthBuffer);
}

//------------------------------------------------------------------------------
void Renderer::initialize()
{
    // initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // set shaders:
    // Render
    setShaderSource(loadTextFile("shaders/render_vertex.glsl"),
                    KEYSTR_PROGRAM_RENDER,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/render_fragment.glsl"),
                    KEYSTR_PROGRAM_RENDER,
                    QOpenGLShader::Fragment);

    // Render Depth
    setShaderSource(loadTextFile("shaders/render/render_depth_only_vertex.glsl"),
                    KEYSTR_PROGRAM_RENDER_DEPTH,
                    QOpenGLShader::Vertex);
    // We need a fragment shader to write moments:
    setShaderSource(loadTextFile("shaders/render/render_depth_only_fragment.glsl"),
                    KEYSTR_PROGRAM_RENDER_DEPTH,
                    QOpenGLShader::Fragment);

    // Shadow Map
    setShaderSource(loadTextFile("shaders/shadowmap_vertex.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/shadowmap_geometry.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Geometry);
    // No fragment shader, it's empty

    // compose program:
    setShaderSource(loadTextFile("shaders/copy_vertex.glsl"),
                    KEYSTR_PROGRAM_COMPOSE,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/compose/compose_fragment_line_sampling_vo.glsl"),
                    KEYSTR_PROGRAM_COMPOSE,
                    QOpenGLShader::Fragment);
    // copy
    setShaderSource(loadTextFile("shaders/copy_vertex.glsl"),
                    KEYSTR_PROGRAM_COPY,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/copy_array_fragment.glsl"),
                    KEYSTR_PROGRAM_COPY,
                    QOpenGLShader::Fragment);

    // Reduce shader
    setShaderSource(loadTextFile("shaders/sdsm/reduce_depth_sampler.glsl"),
                    KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/sdsm/reduce_depth_compute.glsl"),
                    KEYSTR_PROGRAM_REDUCE_DEPTH,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/sdsm/reduce_frustum_sampler.glsl"),
                    KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/sdsm/reduce_frustum_compute.glsl"),
                    KEYSTR_PROGRAM_REDUCE_FRUSTUM,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/sdsm/create_cascade_fars.glsl"),
                    KEYSTR_PROGRAM_CREATE_CASCADE_FARS,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/sdsm/create_cascade_views.glsl"),
                    KEYSTR_PROGRAM_CREATE_CASCADE_VIEWS,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/csm/create_cascades.glsl"),
                    KEYSTR_PROGRAM_CREATE_CASCADES_CPU,
                    QOpenGLShader::Compute);


    // Gauss Filter shaders
    setShaderSource(loadTextFile("shaders/filter/vertical_gauss.glsl"),
                    KEYSTR_PROGRAM_VERTICAL_GAUSS,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/filter/horizontal_gauss.glsl"),
                    KEYSTR_PROGRAM_HORIZONTAL_GAUSS,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/create_moments.glsl"),
                    KEYSTR_PROGRAM_CREATE_MOMENTS,
                    QOpenGLShader::Compute);


    // Capture Frustum
    setShaderSource(loadTextFile("shaders/awesome_capture/awesome_capture_compute.glsl"),
                    KEYSTR_PROGRAM_CREATE_CAPTURE,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/awesome_capture/awesome_capture_vertex.glsl"),
                    KEYSTR_PROGRAM_RENDER_CAPTURE,
                    QOpenGLShader::Vertex);

    setShaderSource(loadTextFile("shaders/awesome_capture/awesome_capture_fragment.glsl"),
                    KEYSTR_PROGRAM_RENDER_CAPTURE,
                    QOpenGLShader::Fragment);

    // Light View
    setShaderSource(loadTextFile("shaders/lightview/lightview_vertex.glsl"),
                    KEYSTR_PROGRAM_RENDER_LIGHTVIEW,
                    QOpenGLShader::Vertex);

    setShaderSource(loadTextFile("shaders/lightview/lightview_fragment.glsl"),
                    KEYSTR_PROGRAM_RENDER_LIGHTVIEW,
                    QOpenGLShader::Fragment);

    setShaderSource(loadTextFile("shaders/lightview/lightview_samples.glsl"),
                    KEYSTR_PROGRAM_RENDER_VISIBLE_SAMPLES,
                    QOpenGLShader::Compute);


    // Generate attrib and uniform locations

    // Render
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_modelViewMatrixLoc, "modelViewMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_projectionMatrixLoc, "projectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_cascadeViewMatrixLoc, "cascadeViewMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_cascadeFarLoc, "cascadeFar");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_lightDirectionLoc, "lightDirection");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_lightColorLoc, "lightColor");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_specularColorLoc, "specularColor");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_diffuseColorLoc, "diffuseColor");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_ambientColorLoc, "ambientColor");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_shadowMapSamplerLoc, "shadowMapSampler");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].emplace_back(&m_colorizeCascadesLoc, "colorizeCascades");

    m_attribLocs[KEYSTR_PROGRAM_RENDER].emplace_back(0, "v_position");
    m_attribLocs[KEYSTR_PROGRAM_RENDER].emplace_back(1, "v_normal");


    // Render Depth
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_DEPTH].emplace_back(&m_depthOnlyModelViewMatrixLoc, "modelViewMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_DEPTH].emplace_back(&m_depthOnlyProjectionMatrixLoc, "projectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_DEPTH].emplace_back(&m_depthOnlyInverseProjectionMatrixLoc, "inverseProjectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_DEPTH].emplace_back(&m_depthOnlySceneDepth, "sceneDepth");

    m_attribLocs[KEYSTR_PROGRAM_RENDER_DEPTH].emplace_back(0, "v_position");
    m_attribLocs[KEYSTR_PROGRAM_RENDER_DEPTH].emplace_back(1, "v_normal");


    // Shadow
    m_uniformLocs[KEYSTR_PROGRAM_SHADOW].emplace_back(&m_shadowMapCascadeViewMatrixLoc, "cascadeViewMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_SHADOW].emplace_back(&m_shadowMapWorldMatrixLoc, "worldMatrix");

    m_uniformLocs[KEYSTR_PROGRAM_CREATE_MOMENTS].emplace_back(&m_createMomentsSampleCountLoc, "sampleCount");

    // Compose
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeProjectionMatrixLoc, "projectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeInverseProjectionMatrixLoc, "inverseProjectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeDepthBufferLoc, "depthBuffer");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeSamplerLoc, "sampler");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeMomentsSamplerLoc, "momentsSampler");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_ratioLoc, "ratio");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeLightDirectionLoc, "lightDirection");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeSamplesLoc, "samples");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composePlainObscuranceLoc, "isPlainObscurance");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeSkyLoc, "isSky");
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(&m_composeCursorLoc, "isCursor");


    m_attribLocs[KEYSTR_PROGRAM_COMPOSE].emplace_back(0, "v_position");


    // Copy (Debug)
    m_uniformLocs[KEYSTR_PROGRAM_COPY].emplace_back(&m_copyArraySamplerLoc, "sampler");
    m_uniformLocs[KEYSTR_PROGRAM_COPY].emplace_back(&m_copyArrayLayerLoc, "layer");

    m_attribLocs[KEYSTR_PROGRAM_COPY].emplace_back(0, "v_position");


    // Misc. Reduce
    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER].emplace_back(&m_reduceDepthInputSizeLoc, "inputSize");

    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].emplace_back(&m_reduceFrustumInputSizeLoc, "inputSize");
    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].emplace_back(&m_reduceFrustumCascadeFarLoc, "cascadeFar");
    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].emplace_back(&m_reduceFrustumScreenToLightMatrixLoc, "screenToLightMatrix");

    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CASCADE_FARS].emplace_back(&m_createCascadeFarsCameraProjectionLoc, "cameraProjectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CASCADE_FARS].emplace_back(&m_createCascadeFarsInverseCameraProjectionLoc, "inverseCameraProjectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CASCADE_FARS].emplace_back(&m_createCascadeFarsLambdaLoc, "lambda");

    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CASCADE_VIEWS].emplace_back(&m_createCascadeViewsLightViewMatrixLoc, "lightViewMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CASCADE_VIEWS].emplace_back(&m_createCascadeViewsInvTempProjMatrixLoc, "inverseTempProjectionMatrix");

    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CASCADES_CPU].emplace_back(&m_createCascadesCpuCascadeFarLoc, "inputCascadeFar");
    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CASCADES_CPU].emplace_back(&m_createCascadesCpuCascadeViewMatrixLoc, "inputCascadeViewMatrix");


    // Capture Frustum
    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CAPTURE].emplace_back(&m_createCaptureInverseCameraViewLoc, "inverseCameraView");
    m_uniformLocs[KEYSTR_PROGRAM_CREATE_CAPTURE].emplace_back(&m_createCaptureInverseCameraProjectionLoc, "inverseCameraProjection");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_CAPTURE].emplace_back(&m_renderCaptureCascadeIndexLoc, "cascadeIndex");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_CAPTURE].emplace_back(&m_renderCaptureCameraViewLoc, "cameraView");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_CAPTURE].emplace_back(&m_renderCaptureCameraProjectionLoc, "cameraProjection");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_CAPTURE].emplace_back(&m_renderCaptureColorLoc, "color");

    // Light View
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_LIGHTVIEW].emplace_back(&m_lightViewProjectionMatrixLoc, "projectionMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_LIGHTVIEW].emplace_back(&m_lightViewModelViewMatrixLoc, "modelViewMatrix");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_LIGHTVIEW].emplace_back(&m_lightViewDiffuseColorLoc, "diffuseColor");
    m_uniformLocs[KEYSTR_PROGRAM_RENDER_LIGHTVIEW].emplace_back(&m_lightViewAmbientColorLoc, "ambientColor");

    m_attribLocs[KEYSTR_PROGRAM_RENDER_LIGHTVIEW].emplace_back(0, "v_position");
    m_attribLocs[KEYSTR_PROGRAM_RENDER_LIGHTVIEW].emplace_back(1, "v_normal");

    m_uniformLocs[KEYSTR_PROGRAM_RENDER_VISIBLE_SAMPLES].emplace_back(&m_visibleSamplesScreenToLightLoc, "screenToLightTransformation");


    // create Programs:
    createProgram(KEYSTR_PROGRAM_RENDER);
    createProgram(KEYSTR_PROGRAM_RENDER_DEPTH);
    createProgram(KEYSTR_PROGRAM_SHADOW);
    createProgram(KEYSTR_PROGRAM_COMPOSE);
    createProgram(KEYSTR_PROGRAM_COPY);
    createProgram(KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER);
    createProgram(KEYSTR_PROGRAM_REDUCE_DEPTH);
    createProgram(KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER);
    createProgram(KEYSTR_PROGRAM_REDUCE_FRUSTUM);
    createProgram(KEYSTR_PROGRAM_CREATE_CASCADE_FARS);
    createProgram(KEYSTR_PROGRAM_CREATE_CASCADE_VIEWS);
    createProgram(KEYSTR_PROGRAM_CREATE_CASCADES_CPU);
    createProgram(KEYSTR_PROGRAM_HORIZONTAL_GAUSS);
    createProgram(KEYSTR_PROGRAM_VERTICAL_GAUSS);
    createProgram(KEYSTR_PROGRAM_CREATE_MOMENTS);
    createProgram(KEYSTR_PROGRAM_CREATE_CAPTURE);
    createProgram(KEYSTR_PROGRAM_RENDER_CAPTURE);
    createProgram(KEYSTR_PROGRAM_RENDER_LIGHTVIEW);
    createProgram(KEYSTR_PROGRAM_RENDER_VISIBLE_SAMPLES);

    // make stuff
    // --> default program (nothing to do) <------------------------------------

    // --> shadow map program <-------------------------------------------------
    // Create ShadowMap
    m_shadowMapSize = 1024;
    m_cascades = 4;

    // Create Texture
    glGenTextures(1, &m_shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA16, m_shadowMapSize, m_shadowMapSize, m_cascades, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // Create Texture 2 (back)
    glGenTextures(1, &m_shadowMapTexture2);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture2);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA16, m_shadowMapSize, m_shadowMapSize, m_cascades, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // Create DepthBuffer (multisample)
    glGenTextures(1, &m_shadowMapDepthBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, m_shadowMapDepthBuffer);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    // glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, m_shadowMapSize, m_shadowMapSize, m_cascades, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
    glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, m_shadowMapSampleCount, GL_DEPTH_COMPONENT, m_shadowMapSize, m_shadowMapSize, m_cascades, false);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 0);
    m_recreateShadowMap = false;

    // Create FrameBuffer
    glGenFramebuffers(1, &m_shadowMapFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMapDepthBuffer, 0);

    // Set the list of draw buffers.
    GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(0, attachments);

    // --> compose Program <----------------------------------------------------
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
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    m_quadVbo.release();


    // cascade buffers
    glGenBuffers(1, &m_cascadeFarBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_cascadeFarBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 /*m_cascades*/, NULL, GL_DYNAMIC_COPY);

    glGenBuffers(1, &m_cascadeViewBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_cascadeViewBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*16 * 4 /*m_cascades*/, NULL, GL_DYNAMIC_COPY);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


    // Capture Buffer
    glGenBuffers(4, m_capturedFrustumToWorldBuffer);
    for (int i = 0; i < 4; i++)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_capturedFrustumToWorldBuffer[i]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*16 * 5, NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}

void Renderer::createShadowMap()
{
    // Try to do minimal work here.
    glDeleteTextures(1, &m_shadowMapDepthBuffer);
    glGenTextures(1, &m_shadowMapDepthBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, m_shadowMapDepthBuffer);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, m_shadowMapSampleCount, GL_DEPTH_COMPONENT, m_shadowMapSize, m_shadowMapSize, m_cascades, false);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 0);

    // Bind new texture to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMapDepthBuffer, 0);

    m_recreateShadowMap = false;
}

void Renderer::rotateVectorToVector(const QVector3D &source, const QVector3D &destination, QMatrix4x4 &matrix)
{
    QVector3D rotationAxis = QVector3D::crossProduct(source, destination);
    float rotationAngle = std::acos(QVector3D::dotProduct(source, destination))*180.f/
                          static_cast<float>(M_PI);

    matrix.rotate(rotationAngle, rotationAxis);
}

void Renderer::createLightViewMatrix(const QVector3D &lightDir, const QMatrix4x4 &inverseCameraView, QMatrix4x4 &matrix)
{
    // Rotate light direction to z
    QMatrix4x4 rotateLightDir;
    rotateVectorToVector(lightDir.normalized(), QVector3D(0, 0, 1), rotateLightDir);

    // Camera z in light view (direction)
    auto cameraDirInLightView = rotateLightDir * inverseCameraView * QVector4D(0, 0, -1, 0);

    // Constrain rotation to rotation along z axis in xy plane, project to z=0 plane
    cameraDirInLightView.setZ(0);

    // Rotate cameraDir to (1, 0, 0)
    QMatrix4x4 rotateCameraDir;
    rotateVectorToVector(cameraDirInLightView.toVector3D().normalized(), QVector3D(1, 0, 0), rotateCameraDir);

    matrix = rotateCameraDir * rotateLightDir;
}

void Renderer::createFrustumProjectionMatrix(const QMatrix4x4 & frustumTransformation, QMatrix4x4 &matrix)
{
    QVector3D frustumCorners[] = {
            { -1, -1, -1 },
            {  1, -1, -1 },
            { -1,  1, -1 },
            {  1,  1, -1 },
            { -1, -1,  1 },
            {  1, -1,  1 },
            { -1,  1,  1 },
            {  1,  1,  1 }
    };

    MathUtility::transformVectors(frustumTransformation, frustumCorners);

    QVector3D minCorner = frustumCorners[0];
    QVector3D maxCorner = frustumCorners[1];

    for (const auto &corner : frustumCorners)
    {
        for (int i = 0; i < 3; i++)
        {
            minCorner[i] = std::min(minCorner[i], corner[i]);
            maxCorner[i] = std::max(maxCorner[i], corner[i]);
        }
    }

    // TODO check if this projection is what we need!
    matrix.ortho(minCorner.x(), maxCorner.x(), minCorner.y(), maxCorner.y(), -maxCorner.z(), -minCorner.z());
}

void Renderer::onRenderingInternal(GLuint fbo, Scene *scene)
{
    QOpenGLShaderProgram *shadowMapProgram = m_programs[KEYSTR_PROGRAM_SHADOW].get();
    QOpenGLShaderProgram *renderProgram = m_programs[KEYSTR_PROGRAM_RENDER].get();
    QOpenGLShaderProgram *renderDepthProgram = m_programs[KEYSTR_PROGRAM_RENDER_DEPTH].get();
    QOpenGLShaderProgram *composeProgram = m_programs[KEYSTR_PROGRAM_COMPOSE].get();
    QOpenGLShaderProgram *reduceDepthSamplerProgram = m_programs[KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER].get();
    QOpenGLShaderProgram *reduceDepthProgram = m_programs[KEYSTR_PROGRAM_REDUCE_DEPTH].get();
    QOpenGLShaderProgram *reduceFrustumSamplerProgram = m_programs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].get();
    QOpenGLShaderProgram *reduceFrustumProgram = m_programs[KEYSTR_PROGRAM_REDUCE_FRUSTUM].get();
    QOpenGLShaderProgram *createCascadeFars = m_programs[KEYSTR_PROGRAM_CREATE_CASCADE_FARS].get();
    QOpenGLShaderProgram *createCascadeViews = m_programs[KEYSTR_PROGRAM_CREATE_CASCADE_VIEWS].get();
    QOpenGLShaderProgram *createCascadesCpu = m_programs[KEYSTR_PROGRAM_CREATE_CASCADES_CPU].get();
    QOpenGLShaderProgram *copyProgram = m_programs[KEYSTR_PROGRAM_COPY].get();
    QOpenGLShaderProgram *verticalGaussProgram = m_programs[KEYSTR_PROGRAM_VERTICAL_GAUSS].get();
    QOpenGLShaderProgram *horizontalGaussProgram = m_programs[KEYSTR_PROGRAM_HORIZONTAL_GAUSS].get();
    QOpenGLShaderProgram *createMomentsProgram = m_programs[KEYSTR_PROGRAM_CREATE_MOMENTS].get();
    QOpenGLShaderProgram *createCaptureProgram = m_programs[KEYSTR_PROGRAM_CREATE_CAPTURE].get();
    QOpenGLShaderProgram *renderLightView = m_programs[KEYSTR_PROGRAM_RENDER_LIGHTVIEW].get();
    QOpenGLShaderProgram *renderVisibleSamples = m_programs[KEYSTR_PROGRAM_RENDER_VISIBLE_SAMPLES].get();

    // Check if we have to recreate the shadow map depth buffer, because sample count changed
    if (m_recreateShadowMap)
        createShadowMap();

    // Inverse of common transformations...
    const auto &cameraView = scene->getCameraView();
    const auto &cameraProjection = scene->getCameraProjection();
    auto inverseCameraView = cameraView.inverted();
    auto inverseCameraProjection = cameraProjection.inverted();
    auto inverseCameraTransformation = (cameraProjection * cameraView).inverted();

    // light direction from camera's perspective
    auto lightDirection = cameraView * QVector4D(scene->getDirectionalLightDirection(), 0);


    // 0. Clear render target
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);

    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_width, m_height);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // color attachment
    GLuint voAttachement[1] = { GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(1, voAttachement);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Render depth only
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    renderDepthProgram->bind();

    renderDepthProgram->setUniformValue(m_depthOnlyProjectionMatrixLoc, cameraProjection);
    renderDepthProgram->setUniformValue(m_depthOnlyInverseProjectionMatrixLoc, inverseCameraProjection);
    renderDepthProgram->setUniformValue(m_depthOnlySceneDepth, 100);

    // Render regular objects
    for (auto &object : scene->getObjects())
    {
        auto cameraModelView = cameraView * object->getWorld();

        renderDepthProgram->setUniformValue(m_depthOnlyModelViewMatrixLoc, cameraModelView);

        object->getModel()->draw();
    }

    // Render debug objects
    for (auto editorObject : scene->getEditorObjects())
    {
        if (!(static_cast<EditorObject *>(editorObject)->isVisible()))
        {
            continue;
        }

        auto cameraModelView = cameraView * editorObject->getWorld();

        renderDepthProgram->setUniformValue(m_depthOnlyModelViewMatrixLoc, cameraModelView);

        editorObject->getModel()->draw();
    }

    // glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    renderDepthProgram->release();


    // 2. Compute shadow map projection matrices

    switch (m_cascadeStrategy)
    {
        case SampleDistributionShadowMaps:
        {
            // 2.1a Reduce min/max depth
            // round up
            GLsizei initWidth = (m_width-1) / 2 / 16 + 1;
            GLsizei initHeight = (m_height-1) / 2 / 16 + 1;

            reduceDepthSamplerProgram->bind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);

            reduceDepthSamplerProgram->setUniformValue(m_reduceDepthInputSizeLoc, m_width, m_height);

            glBindImageTexture(1, m_depthReduceTextures[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16);

            // for every new pixel spawn a thread group!
            glDispatchCompute(initWidth, initHeight, 1);

            reduceDepthSamplerProgram->release();

            // size of first reduction result
            GLsizei size = initWidth*initHeight;

            reduceDepthProgram->bind();

            for (int i = 1; i < m_depthReduceTextures.size(); i++)
            {
                glBindImageTexture(0, m_depthReduceTextures[i-1], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG16);
                glBindImageTexture(1, m_depthReduceTextures[i], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16);

                // round up
                size = (size-1) / 1 / 256 + 1;

                // for every new pixel spawn a thread group!
                glDispatchCompute(size, 1, 1);
            }

            reduceDepthProgram->release();

            // Compute view matrix for light (light direction -> Z, camera Z -> X)
            QMatrix4x4 lightViewMatrix;
            createLightViewMatrix(scene->getDirectionalLightDirection(), inverseCameraView, lightViewMatrix);

            if (m_lightShearing)
            {
                // auto cameraXDir = lightViewMatrix * inverseCameraView * QVector4D(1, 0, 0, 0);
                auto cameraXDir = lightViewMatrix * QVector4D(QVector3D(scene->getDirectionalLightDirection().x(), 0, scene->getDirectionalLightDirection().z()).normalized(), 0);
                // sheer light Y to camera X
                QMatrix4x4 shearing;
                shearing.setColumn(1, cameraXDir);
                lightViewMatrix = shearing.inverted() * lightViewMatrix;
            }

            // Compute view frustum of camera in light view
            auto screenToLightTransformation = lightViewMatrix * inverseCameraTransformation;

            // Projection matrix that wraps whole camera frustum
            QMatrix4x4 tempLightProjection;
            createFrustumProjectionMatrix(screenToLightTransformation, tempLightProjection);


            // 2.2a Compute near and far plane for cascades! (on GPU!)
            createCascadeFars->bind();

            createCascadeFars->setUniformValue(m_createCascadeFarsCameraProjectionLoc, cameraProjection);
            createCascadeFars->setUniformValue(m_createCascadeFarsInverseCameraProjectionLoc, inverseCameraProjection);
            createCascadeFars->setUniformValue(m_createCascadeFarsLambdaLoc, m_cascadedShadowMapsLambda);

            glBindImageTexture(0, m_depthReduceTextures.back(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG16);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_cascadeFarBuffer);

            glDispatchCompute(1, 1, 1);

            createCascadeFars->release();


            // 2.3a Reduce bbox for each cascade
            // round up
            initWidth = (m_width-1) / 2 / 16 + 1;
            initHeight = (m_height-1) / 2 / 16 + 1;

            reduceFrustumSamplerProgram->bind();

            reduceFrustumSamplerProgram->setUniformValue(m_reduceFrustumInputSizeLoc, m_width, m_height);
            reduceFrustumSamplerProgram->setUniformValue(m_reduceFrustumScreenToLightMatrixLoc, tempLightProjection * screenToLightTransformation);
            // reduceFrustumSamplerProgram->setUniformValueArray(m_reduceFrustumCascadeFarLoc, cascadeFars.data(), m_cascades-1, 1);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_cascadeFarBuffer);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);

            glBindImageTexture(1, m_frustumReduceTextureArrays[0], 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

            // for every new pixel spawn a thread group!
            glDispatchCompute(initWidth, initHeight, 1);

            reduceFrustumSamplerProgram->release();

            // size of first reduction result
            size = initWidth*initHeight;

            reduceFrustumProgram->bind();

            for (int i = 1; i < m_frustumReduceTextureArrays.size(); i++)
            {
                glBindImageTexture(0, m_frustumReduceTextureArrays[i-1], 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA16);
                glBindImageTexture(1, m_frustumReduceTextureArrays[i], 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

                // round up
                size = (size-1) / 1 / 256 + 1;

                // for every new pixel spawn a thread group!
                glDispatchCompute(size, 1, 1);
                // NVIDIA gpu's are too aggressive here..
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            }

            reduceFrustumProgram->release();


            // 2.4a Compute cascade projection matrices on GPU!
            createCascadeViews->bind();

            auto inverseTempLightProjection = tempLightProjection.inverted();
            createCascadeViews->setUniformValue(m_createCascadeViewsInvTempProjMatrixLoc, inverseTempLightProjection);
            createCascadeViews->setUniformValue(m_createCascadeViewsLightViewMatrixLoc, lightViewMatrix * inverseCameraView);

            glBindImageTexture(0, m_frustumReduceTextureArrays.back(), 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA16);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_cascadeViewBuffer);

            glDispatchCompute(1, 1, 1);

            createCascadeViews->release();
        } break;


        case CascadedShadowMaps:
        {
            // 2.1b Compute Z-Partitions
            std::vector<float> cascadeFars;
            std::vector<QMatrix4x4> cascadeViews;

            // Compute view matrix for light (light direction -> Z, camera Z -> X)
            QMatrix4x4 lightViewMatrix;
            createLightViewMatrix(scene->getDirectionalLightDirection(), inverseCameraView, lightViewMatrix);

            if (m_lightShearing)
            {
                auto cameraXDir = lightViewMatrix * inverseCameraView * QVector4D(1, 0, 0, 0);
                // sheer light Y to camera X
                QMatrix4x4 shearing;
                shearing.setColumn(1, cameraXDir);
                lightViewMatrix = shearing.inverted() * lightViewMatrix;
            }

            // Compute view frustum of camera in light view
            auto screenToLightTransformation = lightViewMatrix * inverseCameraTransformation;

            // For interpolation of frustum corners
            QVector3D frustumNearCorners[] = {
                    { -1, -1, -1 },
                    {  1, -1, -1 },
                    { -1,  1, -1 },
                    {  1,  1, -1 }
            };
            QVector3D frustumFarCorners[] = {
                    { -1, -1,  1 },
                    {  1, -1,  1 },
                    { -1,  1,  1 },
                    {  1,  1,  1 }
            };

            // For finding near/far plane
            QVector3D frustumNearFarPoints[] = {
                    { 0, 0, -1},
                    { 0, 0,  1}
            };

            // Project frustum corners into light view space
            MathUtility::transformVectors(screenToLightTransformation, frustumNearCorners);
            MathUtility::transformVectors(screenToLightTransformation, frustumFarCorners);
            MathUtility::transformVectors(inverseCameraProjection, frustumNearFarPoints);

            // Looking into nevative Z direction!
            float nearZ = -frustumNearFarPoints[0].z();
            float farZ = -frustumNearFarPoints[1].z();

            for (int i = 0; i < m_cascades; i++)
            {
                // Redundant computations... I don't care!
                float nearIndex = i/4.f;
                float cNearUni = nearZ + (farZ - nearZ) * nearIndex;
                float cNearLog = nearZ * pow(farZ / nearZ, nearIndex);
                float cascadeNearZ = (1 - m_cascadedShadowMapsLambda) * cNearUni + m_cascadedShadowMapsLambda * cNearLog;

                float farIndex = (i+1.f)/4.f;
                float cFarUni = nearZ + (farZ - nearZ) * farIndex;
                float cFarLog = nearZ * pow(farZ / nearZ, farIndex);
                float cascadeFarZ = (1 - m_cascadedShadowMapsLambda) * cFarUni + m_cascadedShadowMapsLambda * cFarLog;

                {
                    // project into screen space!
                    QVector4D cascadeFarPoint(0, 0, -cascadeFarZ, 1);
                    cascadeFarPoint = cameraProjection * cascadeFarPoint;
                    // NOTE: THE RESULT IS FROM -1 TO 1, BUT OUR FAR VALUES FROM 0 TO 1!!!! SCREW YOU!
                    cascadeFars.push_back(cascadeFarPoint.z()/cascadeFarPoint.w()*0.5f + 0.5f);
                }

                {
                    QVector3D minCorner;
                    QVector3D maxCorner;

                    float coeffNear = (cascadeNearZ - nearZ) / (farZ - nearZ);
                    float coeffFar = (cascadeFarZ - nearZ) / (farZ - nearZ);

                    for (int j = 0; j < 4; j++)
                    {
                        auto &nearPoint = frustumNearCorners[j];
                        auto &farPoint = frustumFarCorners[j];

                        auto point = (1 - coeffNear) * nearPoint + coeffNear  * farPoint;

                        if (j == 0)
                        {
                            minCorner = maxCorner = point;
                            continue;
                        }

                        for (int k = 0; k < 3; k++)
                        {
                            if (minCorner[k] > point[k])
                                minCorner[k] = point[k];
                            if (maxCorner[k] < point[k])
                                maxCorner[k] = point[k];
                        }
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        auto &nearPoint = frustumNearCorners[j];
                        auto &farPoint = frustumFarCorners[j];

                        auto point = (1 - coeffFar) * nearPoint + coeffFar * farPoint;

                        for (int k = 0; k < 3; k++)
                        {
                            if (minCorner[k] > point[k])
                                minCorner[k] = point[k];
                            if (maxCorner[k] < point[k])
                                maxCorner[k] = point[k];
                        }
                    }

                    QMatrix4x4 cascadeProjection;
                    cascadeProjection.ortho(minCorner.x(), maxCorner.x(), minCorner.y(), maxCorner.y(), -maxCorner.z(), -minCorner.z());
                    cascadeViews.push_back(cascadeProjection * lightViewMatrix * inverseCameraView);
                }
            }

            // 2.2b Move computed values into GPU access only buffer
            createCascadesCpu->bind();

            createCascadesCpu->setUniformValueArray(m_createCascadesCpuCascadeFarLoc, cascadeFars.data(), 1, m_cascades);
            createCascadesCpu->setUniformValueArray(m_createCascadesCpuCascadeViewMatrixLoc, cascadeViews.data(), m_cascades);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_cascadeFarBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_cascadeViewBuffer);

            glDispatchCompute(1, 1, 1);

            createCascadesCpu->release();
        } break;
    }


    // DEBUG
    // Create Capture
    if (m_captureRequested || m_renderLightView)
    {
        createCaptureProgram->bind();

        createCaptureProgram->setUniformValue(m_createCaptureInverseCameraViewLoc, inverseCameraView);
        createCaptureProgram->setUniformValue(m_createCaptureInverseCameraProjectionLoc, inverseCameraProjection);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_cascadeViewBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_capturedFrustumToWorldBuffer[m_captureSlot]);

        glDispatchCompute(1, 1, 1);

        createCaptureProgram->release();

        m_captureRequested = false;
    }


    if (m_renderLightView)
    {
        // Use the captured information to display frustum stuff

        // Compute view matrix for light (light direction -> Z, camera Z -> X)
        QMatrix4x4 lightViewMatrix;
        createLightViewMatrix(scene->getDirectionalLightDirection(), inverseCameraView, lightViewMatrix);

        // Compute view frustum of camera in light view
        auto screenToLightTransformation = lightViewMatrix * inverseCameraTransformation;

        // Projection matrix that wraps whole camera frustum
        QMatrix4x4 tempLightProjection;
        createFrustumProjectionMatrix(screenToLightTransformation, tempLightProjection);
        QMatrix4x4 scaling;
        scaling.scale(0.99, 0.99, 0.99);
        tempLightProjection = scaling*tempLightProjection;

        // Render Light View
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glViewport(0, 0, m_width, m_height);

        // Clear!
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        // Draw closest pixel
        glDepthFunc(GL_LESS);

        renderLightView->bind();

        renderLightView->setUniformValue(m_lightViewProjectionMatrixLoc, tempLightProjection);

        // Render regular objects
        for (auto &object : scene->getObjects())
        {
            auto cameraModelView = lightViewMatrix * object->getWorld();

            renderLightView->setUniformValue(m_lightViewModelViewMatrixLoc, cameraModelView);
            renderLightView->setUniformValue(m_lightViewDiffuseColorLoc, object->getDiffuseColor());
            renderLightView->setUniformValue(m_lightViewAmbientColorLoc, object->getAmbientColor());

            object->getModel()->draw();
        }

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        renderLightView->release();

        renderVisibleSamples->bind();

        renderVisibleSamples->setUniformValue(m_visibleSamplesScreenToLightLoc, tempLightProjection * screenToLightTransformation);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);

        GLint windowTexture;
        glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &windowTexture);
        glBindImageTexture(1, windowTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

        glDispatchCompute((m_width - 1) / 16 + 1, (m_height - 1) / 16 + 1, 1);

        renderVisibleSamples->release();

        renderCapture(lightViewMatrix, tempLightProjection);
    }
    else
    {
        // 3. Render to ShadowMap
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);

        // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
        glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

        // Initialize with max depth.
        glClearColor(0.999990940f, 0.997558594f, 0.893437386f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_DEPTH_CLAMP);
        glEnable(GL_MULTISAMPLE);
        // glEnable(GL_CULL_FACE);

        shadowMapProgram->bind();


        // shadowMapProgram->setUniformValueArray(m_shadowMapCascadeViewMatrixLoc, cascadeViews.data(), m_cascades);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_cascadeViewBuffer);

        for (auto &object : scene->getObjects())
        {
            // work in camera view space
            shadowMapProgram->setUniformValue(m_shadowMapWorldMatrixLoc, cameraView * object->getWorld());

            object->getModel()->draw();
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_DEPTH_CLAMP);
        glDisable(GL_MULTISAMPLE);
        // glDisable(GL_CULL_FACE);

        shadowMapProgram->release();

        // AMD gpu's are too agressive!
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        // these barriers do work: GL_ALL_BARRIER_BITS GL_SHADER_IMAGE_ACCESS_BARRIER_BIT GL_TEXTURE_FETCH_BARRIER_BIT GL_SHADER_STORAGE_BARRIER_BIT


        // 3.1 Resolve MSAA depth to moment texture
        createMomentsProgram->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, m_shadowMapDepthBuffer);

        createMomentsProgram->setUniformValue(m_createMomentsSampleCountLoc, m_shadowMapSampleCount);

        glBindImageTexture(1, m_shadowMapTexture, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

        glDispatchCompute(m_shadowMapSize/16, m_shadowMapSize/16, 4);

        createMomentsProgram->release();

        /*
        glFinish();
        auto start = std::chrono::system_clock::now();
        */

        if (m_filterShadowMap)
        {
            // Filter Shadow Map
            for (int i = 0; i < 4; i++)
            {
                horizontalGaussProgram->bind();

                glBindImageTexture(0, m_shadowMapTexture, 0, GL_FALSE, i, GL_READ_ONLY, GL_RGBA16);
                glBindImageTexture(1, m_shadowMapTexture2, 0, GL_FALSE, i, GL_WRITE_ONLY, GL_RGBA16);

                glDispatchCompute(m_shadowMapSize/16, m_shadowMapSize/16, 1);

                horizontalGaussProgram->release();

                verticalGaussProgram->bind();

                glBindImageTexture(0, m_shadowMapTexture2, 0, GL_FALSE, i, GL_READ_ONLY, GL_RGBA16);
                glBindImageTexture(1, m_shadowMapTexture, 0, GL_FALSE, i, GL_WRITE_ONLY, GL_RGBA16);

                glDispatchCompute(m_shadowMapSize/16, m_shadowMapSize/16, 1);

                verticalGaussProgram->release();
            }
        }

        /*
        glFinish();
        auto end = std::chrono::system_clock::now();
        m_debugSum += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        m_debugCount++;
        std::cout << (double)m_debugSum/m_debugCount << std::endl;
        m_debugSum = 0;
        m_debugCount = 0;
        */

        if (m_renderMomentView)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            copyProgram->bind();

            m_quadVao.bind();

            copyProgram->setUniformValue(m_copyArraySamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0
            glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);


            glViewport(m_width*0/2, m_height*0/2, m_width/2, m_height/2);
            copyProgram->setUniformValue(m_copyArrayLayerLoc, 0);
            glDrawArrays(GL_QUADS, 0, 4);

            glViewport(m_width*1/2, m_height*0/2, m_width/2, m_height/2);
            copyProgram->setUniformValue(m_copyArrayLayerLoc, 1);
            glDrawArrays(GL_QUADS, 0, 4);

            glViewport(m_width*0/2, m_height*1/2, m_width/2, m_height/2);
            copyProgram->setUniformValue(m_copyArrayLayerLoc, 2);
            glDrawArrays(GL_QUADS, 0, 4);

            glViewport(m_width*1/2, m_height*1/2, m_width/2, m_height/2);
            copyProgram->setUniformValue(m_copyArrayLayerLoc, 3);
            glDrawArrays(GL_QUADS, 0, 4);

            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

            m_quadVao.release();

            copyProgram->release();
        }
        else
        {
            // 4. Render to Texture
            glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);

            // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
            glViewport(0, 0, m_width, m_height);

            // Enable color output
            GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
            glDrawBuffers(1, attachments);

            // NOTE: do not clear, as we already wrote to depth buffer!

            glEnable(GL_DEPTH_TEST);
            // glEnable(GL_CULL_FACE);
            // Draw closest pixel
            glDepthFunc(GL_LEQUAL);

            renderProgram->bind();

            renderProgram->setUniformValue(m_projectionMatrixLoc, cameraProjection);
            renderProgram->setUniformValue(m_lightDirectionLoc, QVector3D(lightDirection));
            renderProgram->setUniformValue(m_lightColorLoc, scene->getLightColor());
            renderProgram->setUniformValue(m_colorizeCascadesLoc, m_colorizeCascades);

            // defaultProgram->setUniformValueArray(m_cascadeViewMatrixLoc, cascadeViews.data(), m_cascades);
            // defaultProgram->setUniformValueArray(m_cascadeFarLoc, cascadeFars.data(), m_cascades, 1);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_cascadeViewBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_cascadeFarBuffer);

            // Bind shadow map
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);
            renderProgram->setUniformValue(m_shadowMapSamplerLoc, 0);

            // Render regular objects
            for (auto &object : scene->getObjects())
            {
                auto cameraModelView = cameraView * object->getWorld();


                renderProgram->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
                renderProgram->setUniformValue(m_specularColorLoc, object->getSpecularColor());
                renderProgram->setUniformValue(m_diffuseColorLoc, object->getDiffuseColor());
                renderProgram->setUniformValue(m_ambientColorLoc, object->getAmbientColor());

                object->getModel()->draw();
            }

            // Render debug objects
            for (auto editorObject : scene->getEditorObjects())
            {
                if (!(static_cast<EditorObject *>(editorObject)->isVisible()))
                {
                    continue;
                }

                auto cameraModelView = cameraView * editorObject->getWorld();

                renderProgram->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
                renderProgram->setUniformValue(m_specularColorLoc, editorObject->getSpecularColor());
                renderProgram->setUniformValue(m_diffuseColorLoc, editorObject->getDiffuseColor());
                renderProgram->setUniformValue(m_ambientColorLoc, editorObject->getAmbientColor());

                editorObject->getModel()->draw();
            }

            // Unbind shadow map texture
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

            // glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);

            renderProgram->release();


            // View Capture if enabled
            if (m_captureEnabled)
            {
                renderCapture(cameraView, cameraProjection);
            }

            /*
            // 5. Gauss moments of screenspace depth:
            verticalVO->bind();

            glBindImageTexture(0, m_voMomentsTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG16);
            glBindImageTexture(1, m_voGaussedMomentsBufferTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16);
            glDispatchCompute((m_width - 1) / 8 + 1, (m_height - 1) / 8 + 1, 1);

            verticalVO->release();


            horizontalVO->bind();

            glBindImageTexture(0, m_voGaussedMomentsBufferTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG16);
            glBindImageTexture(1, m_voMomentsTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16);
            glDispatchCompute((m_width - 1) / 8 + 1, (m_height - 1) / 8 + 1, 1);

            horizontalVO->release();
            */

            // generate mipmaps:
            glBindTexture(GL_TEXTURE_2D, m_voMomentsTexture);
            glGenerateMipmap(GL_TEXTURE_2D);
            if (m_mmLinear)
            {
                if (m_glLinear)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // needed, to make mipmaps available!!!
                }
                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); // needed, to make mipmaps available!!!
                }
            }
            else
            {
                if (m_glLinear)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // needed, to make mipmaps available!!!
                }
                else
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // needed, to make mipmaps available!!!
                }
            }
            glBindTexture(GL_TEXTURE_2D, 0);

            // 6. Render to Screen, apply VO
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glViewport(0, 0, m_width, m_height);

            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            composeProgram->bind();


            composeProgram->setUniformValue(m_composeSamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0

            composeProgram->setUniformValue(m_composeMomentsSamplerLoc, 1);
            composeProgram->setUniformValue(m_composeDepthBufferLoc,2);
            composeProgram->setUniformValue(m_composeProjectionMatrixLoc, scene->getCameraProjection());
            composeProgram->setUniformValue(m_composeInverseProjectionMatrixLoc, scene->getCameraProjection().inverted());
            composeProgram->setUniformValue(m_ratioLoc, m_ratio);
            composeProgram->setUniformValue(m_composeLightDirectionLoc, lightDirection);
            composeProgram->setUniformValue(m_composeSamplesLoc, m_samples);
            composeProgram->setUniformValue(m_composePlainObscuranceLoc, m_plainObscurance);
            composeProgram->setUniformValue(m_composeSkyLoc, m_sky);
            composeProgram->setUniformValue(m_composeCursorLoc, m_cursor);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_renderTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_voMomentsTexture);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);

            m_quadVao.bind();
            glDrawArrays(GL_QUADS, 0, 4);
            m_quadVao.release();

            /*
            glViewport(0, 0, m_width/4, m_height/4);
            glBindTexture(GL_TEXTURE_2D, m_normalTexture);
            glDrawArrays(GL_QUADS, 0, 4);
            */

            glBindTexture(GL_TEXTURE_2D, 0);

            glDisable(GL_BLEND);

            composeProgram->release();
        } // render moment view
    } // render light view
    // DONE



    // DEBUG
    /*
    QMatrix4x4 lightViewMatrix;
    createLightViewMatrix(scene->getDirectionalLightDirection(), inverseCameraView, lightViewMatrix);

    auto screenToLightTransformation = lightViewMatrix * inverseCameraTransformation;

    QMatrix4x4 tempLightProjection;
    createFrustumProjectionMatrix(screenToLightTransformation, tempLightProjection);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, m_width/2, m_height/2);

    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    renderProgram->bind();

    renderProgram->setUniformValue(m_projectionMatrixLoc, tempLightProjection);
    renderProgram->setUniformValue(m_lightDirectionLoc, QVector3D(0, 0, 1));
    renderProgram->setUniformValue(m_lightColorLoc, scene->getLightColor());

    // Render regular objects
    for (auto &object : scene->getObjects())
    {
        auto cameraModelView = lightViewMatrix * object->getWorld();

        renderProgram->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        renderProgram->setUniformValue(m_specularColorLoc, object->getSpecularColor());
        renderProgram->setUniformValue(m_diffuseColorLoc, object->getDiffuseColor());
        renderProgram->setUniformValue(m_ambientColorLoc, object->getAmbientColor());

        object->getModel()->draw();
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
     */
    /*
    QVector2D renderSliceCorners[] = {
            { -1, -1},
            {  1, -1},
            { -1,  1},
            {  1,  1}
    };

    std::vector<std::pair<float, QVector3D>> slices;
    slices.emplace_back(-1, QVector3D(1, 0, 0));
    slices.emplace_back( .99999, QVector3D(1, 0, 0));
    slices.emplace_back(minDepth*2 - 1, QVector3D(0, 1, 0));
    slices.emplace_back(maxDepth*-2 + 1, QVector3D(0, 1, 0));

    for (auto &entry : slices)
    {
        renderProgram->setUniformValue(m_modelViewMatrixLoc, lightViewMatrix);
        renderProgram->setUniformValue(m_specularColorLoc, QVector3D(0, 0, 0));
        renderProgram->setUniformValue(m_diffuseColorLoc, QVector3D(0, 0, 0));
        renderProgram->setUniformValue(m_ambientColorLoc, entry.second);

        for (auto &corner : renderSliceCorners)
        {
            auto result = lastInverseCameraTransformation * QVector4D(corner, entry.first, 1);
            result /= result.w();

            glPointSize(5);

            glBegin(GL_POINTS);

            glVertex3f(result.x(), result.y(), result.z());

            glEnd();

            // model->draw();
        }
    }

    // only last cascade
    renderProgram->setUniformValue(m_specularColorLoc, QVector3D(0, 0, 0));
    renderProgram->setUniformValue(m_diffuseColorLoc, QVector3D(0, 0, 0));
    renderProgram->setUniformValue(m_ambientColorLoc, QVector3D(1, 1, 0));

    for (int i = 0; i < 4; i++)
    {
        QMatrix4x4 world;
        world.setToIdentity();
        renderProgram->setUniformValue(m_modelViewMatrixLoc, world);

        auto &minCorner = minCornersCascade[i];
        auto &maxCorner = maxCornersCascade[i];

        glBegin(GL_LINE_LOOP);

        glVertex3f(minCorner.x(), minCorner.y(), 0);
        glVertex3f(maxCorner.x(), minCorner.y(), 0);
        glVertex3f(maxCorner.x(), maxCorner.y(), 0);
        glVertex3f(minCorner.x(), maxCorner.y(), 0);

        glEnd();
    }

    // Unbind shadow map texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    renderProgram->release();
    */

    /*
    GLint windowTexture;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &windowTexture);
    */

    /*
    // DEBUG: view shadow map depths
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    copyProgram->bind();

    m_quadVao.bind();

    copyProgram->setUniformValue(m_copyArraySamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);


    glViewport(m_width*0/4, m_height*3/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 0);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*1/4, m_height*3/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 1);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*2/4, m_height*3/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 2);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*3/4, m_height*3/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 3);
    glDrawArrays(GL_QUADS, 0, 4);

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapDepthBuffer);

    glViewport(m_width*0/4, m_height*2/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 0);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*1/4, m_height*2/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 1);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*2/4, m_height*2/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 2);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*3/4, m_height*2/4, m_width/4, m_height/4);
    copyProgram->setUniformValue(m_copyArrayLayerLoc, 3);
    glDrawArrays(GL_QUADS, 0, 4);


    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    m_quadVao.release();

    copyProgram->release();
    */
}

void Renderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    m_ratio = float(width)/float(height);

    glDeleteFramebuffers(1, &m_renderFrameBuffer);
    glDeleteTextures(1, &m_renderTexture);
    glDeleteTextures(1, &m_voMomentsTexture);
    glDeleteTextures(1, &m_renderDepthBuffer);
    glDeleteTextures(m_depthReduceTextures.size(), m_depthReduceTextures.data());
    glDeleteTextures(m_frustumReduceTextureArrays.size(), m_frustumReduceTextureArrays.data());

    // Create render texture
    glGenTextures(1, &m_renderTexture);
    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);



    // Create moments/variance texture for volumetric obscurance
    glGenTextures(1, &m_voMomentsTexture);
    glBindTexture(GL_TEXTURE_2D, m_voMomentsTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBindTexture(GL_TEXTURE_2D, 0);


    // Create depth buffer!
    glGenTextures(1, &m_renderDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    // Poor filtering. Needed!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBindTexture(GL_TEXTURE_2D, 0);


    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &m_renderFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_renderDepthBuffer, 0);

    // Set "renderTexture" as our colour attachement #0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_voMomentsTexture, 0);
    // Set the list of draw buffers.
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_DEPTH_ATTACHMENT};
    glDrawBuffers(2, attachments);

    // round up
    GLsizei initWidth = (m_width-1) / 2 / 16 + 1;
    GLsizei initHeight = (m_height-1) / 2 / 16 + 1;

    // size of first reduction result
    GLsizei size = initWidth*initHeight;

    // Create reduce textures!
    auto n = std::ceil(std::log(size) / std::log(1.*256.)) + 1;
    // n = 1; // reduce_frustum_compute.glsl not implemented yet.
    m_depthReduceTextures.resize(n);
    glGenTextures(m_depthReduceTextures.size(), m_depthReduceTextures.data());
    m_frustumReduceTextureArrays.resize(n);
    glGenTextures(m_frustumReduceTextureArrays.size(), m_frustumReduceTextureArrays.data());

    for (int i = 0; i < n; i++)
    {
        std::cout << initWidth << " " << initHeight << " " << size << std::endl;

        glBindTexture(GL_TEXTURE_1D_ARRAY, m_frustumReduceTextureArrays[i]);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, prevWidth, prevHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
        glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, GL_RGBA16, size, 6, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
        // No filtering required
        glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_1D_ARRAY, 0);

        glBindTexture(GL_TEXTURE_1D, m_depthReduceTextures[i]);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RG16, size, 0, GL_RG, GL_UNSIGNED_SHORT, 0);
        // No filtering required
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_1D, 0);

        // store last value..
        m_reduceLastTextureSize = size;

        // tack size for debug output
        initWidth = (initWidth-1) / 2 / 16 + 1;
        initHeight = (initHeight-1) / 2 / 16 + 1;

        // round up
        size = (size-1) / 1 / 256 + 1;
    }
}

void Renderer::renderCapture(const QMatrix4x4 &cameraView, const QMatrix4x4 &cameraProjection)
{
    QOpenGLShaderProgram *renderCaptureProgram = m_programs[KEYSTR_PROGRAM_RENDER_CAPTURE].get();

    auto drawCube = []() {
        glBegin(GL_QUADS);

        // X min
        glVertex3f(-1, -1, -1);
        glVertex3f(-1, -1,  1);
        glVertex3f(-1,  1,  1);
        glVertex3f(-1,  1, -1);

        // X max
        glVertex3f( 1, -1, -1);
        glVertex3f( 1, -1,  1);
        glVertex3f( 1,  1,  1);
        glVertex3f( 1,  1, -1);

        // Y min
        glVertex3f(-1, -1, -1);
        glVertex3f(-1, -1,  1);
        glVertex3f( 1, -1,  1);
        glVertex3f( 1, -1, -1);

        // Y max
        glVertex3f(-1,  1, -1);
        glVertex3f(-1,  1,  1);
        glVertex3f( 1,  1,  1);
        glVertex3f( 1,  1, -1);

        // Z min
        glVertex3f(-1, -1, -1);
        glVertex3f(-1,  1, -1);
        glVertex3f( 1,  1, -1);
        glVertex3f( 1, -1, -1);

        // Z max
        glVertex3f(-1, -1,  1);
        glVertex3f(-1,  1,  1);
        glVertex3f( 1,  1,  1);
        glVertex3f( 1, -1,  1);

        glEnd();
    };
    auto drawWireframe = []() {

        glBegin(GL_LINE_STRIP);

        glVertex3f(-1,  1, -1);
        glVertex3f(-1, -1, -1);
        glVertex3f( 1, -1, -1);
        glVertex3f( 1,  1, -1);

        glEnd();

        glBegin(GL_LINE_STRIP);

        glVertex3f(-1,  1,  1);
        glVertex3f(-1,  1, -1);
        glVertex3f( 1,  1, -1);
        glVertex3f( 1,  1,  1);

        glEnd();

        glBegin(GL_LINE_STRIP);

        glVertex3f(-1, -1,  1);
        glVertex3f(-1,  1,  1);
        glVertex3f( 1,  1,  1);
        glVertex3f( 1, -1,  1);

        glEnd();

        glBegin(GL_LINE_STRIP);

        glVertex3f(-1, -1, -1);
        glVertex3f(-1, -1,  1);
        glVertex3f( 1, -1,  1);
        glVertex3f( 1, -1, -1);

        glEnd();

    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QVector3D cascadeColors[] = {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1},
            {1, 1, 0},
            {1, 0, 1}
    };

    renderCaptureProgram->bind();

    renderCaptureProgram->setUniformValue(m_renderCaptureCameraProjectionLoc, cameraProjection);
    renderCaptureProgram->setUniformValue(m_renderCaptureCameraViewLoc, cameraView);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_capturedFrustumToWorldBuffer[m_captureSlot]);

    // faces
    for (int i = 0; i < 4; i++)
    {
        renderCaptureProgram->setUniformValue(m_renderCaptureCascadeIndexLoc, i);

        renderCaptureProgram->setUniformValue(m_renderCaptureColorLoc, QVector4D(cascadeColors[i], 0.5));
        drawCube();
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_DEPTH_CLAMP);

    // wireframes (also camera frustum)
    for (int i = 0; i < 5; i++)
    {
        renderCaptureProgram->setUniformValue(m_renderCaptureCascadeIndexLoc, i);

        renderCaptureProgram->setUniformValue(m_renderCaptureColorLoc, QVector4D(cascadeColors[i], 1));
        drawWireframe();
    }

    glDisable(GL_DEPTH_CLAMP);

    renderCaptureProgram->release();
}



QWidget *Renderer::createDebugWidget(QWidget *parent)
{
    return new RendererDebugWidget(this, parent);
}

void Renderer::setColorizeCascades(bool enabled)
{
    m_colorizeCascades = enabled;
}

bool Renderer::getColorizeCascades()
{
    return m_colorizeCascades;
}

void Renderer::setCascadedShadowMapsLambda(float lambda)
{
    m_cascadedShadowMapsLambda = lambda;
}

float Renderer::getCascadedShadowMapsLambda()
{
    return m_cascadedShadowMapsLambda;
}

void Renderer::setLightShearing(bool enabled)
{
    m_lightShearing = enabled;
}

bool Renderer::getLightShearing()
{
    return m_lightShearing;
}

void Renderer::setCascadeStrategy(CascadeStrategy strategy)
{
    m_cascadeStrategy = strategy;
}

Renderer::CascadeStrategy Renderer::getCascadeStrategy()
{
    return m_cascadeStrategy;
}

void Renderer::setShadowMapSampleCount(int sampleCount)
{
    m_shadowMapSampleCount = sampleCount;
    m_recreateShadowMap = true;
}

int Renderer::getShadowMapSampleCount()
{
    return m_shadowMapSampleCount;
}

void Renderer::setFilterShadowMap(bool enabled)
{
    m_filterShadowMap = enabled;
}

bool Renderer::getFilterShadowMap()
{
    return m_filterShadowMap;
}

void Renderer::awesomeCapture()
{
    m_captureRequested = true;
}

void Renderer::setCaptureSlot(int slot)
{
    m_captureSlot = slot;
}

int Renderer::getCaptureSlot()
{
    return m_captureSlot;
}

void Renderer::setCapture(bool enabled)
{
    m_captureEnabled = enabled;
}

bool Renderer::getCapture()
{
    return m_captureEnabled;
}

void Renderer::setRenderLightView(bool enabled)
{
    m_renderLightView = enabled;
}

void Renderer::setRenderMomentView(bool enabled)
{
    m_renderMomentView = enabled;
}

void Renderer::setSamples(int s)
{
    m_samples = s;
}

int Renderer::getSamples()
{
    return m_samples;
}

void Renderer::setPlainObscurance(bool b)
{
    m_plainObscurance = b ? 1 : 0;
}

bool Renderer::isPlainObscurance()
{
    return m_plainObscurance == 1;
}

void Renderer::setSky(bool b)
{
    m_sky = b ? 1 : 0;
}

bool Renderer::isSky()
{
    return m_sky == 1;
}

void Renderer::setMipMapStrategy(bool glLin, bool mmLin)
{
    m_glLinear = glLin;
    m_mmLinear = mmLin;
}

void Renderer::getMipMapStrategy(bool &glLin, bool &mmLin)
{
    glLin = m_glLinear;
    mmLin = m_mmLinear;
}

void Renderer::setCursor(bool b)
{
    m_cursor = b ? 1 : 0;
}

bool Renderer::isCursor()
{
    return m_cursor == 1;
}
