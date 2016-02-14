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

#include "MathUtility.h"

#include "utility.h"


Renderer::Renderer() : RendererBase(),
    m_renderFrameBuffer(0),
    m_renderTexture(0),
    m_voMomentsTexture(0),
    m_renderDepthBuffer(0)
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
    // default program:
    setShaderSource(loadTextFile("shaders/render_vertex.glsl"),
                    KEYSTR_PROGRAM_RENDER,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/render_fragment.glsl"),
                    KEYSTR_PROGRAM_RENDER,
                    QOpenGLShader::Fragment);
    // shadow map program
    setShaderSource(loadTextFile("shaders/shadowmap_vertex.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/shadowmap_geometry.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Geometry);
    setShaderSource(loadTextFile("shaders/shadowmap_fragment.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Fragment);
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
    setShaderSource(loadTextFile("shaders/reduce/reduce_depth_sampler.glsl"),
                    KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/reduce/reduce_depth_compute.glsl"),
                    KEYSTR_PROGRAM_REDUCE_DEPTH,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/reduce/reduce_frustum_sampler.glsl"),
                    KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/reduce/reduce_frustum_compute.glsl"),
                    KEYSTR_PROGRAM_REDUCE_FRUSTUM,
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

    // vo variance/moment filter shaders
    setShaderSource(loadTextFile("shaders/filter/horizontal_vo_area.glsl"),
                    KEYSTR_PROGRAM_HORIZONTAL_VO_AREA,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/filter/vertical_vo_area.glsl"),
                    KEYSTR_PROGRAM_VERTICAL_VO_AREA,
                    QOpenGLShader::Compute);

    // generate attrib and uniform locations
    // default:
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                std::make_pair(&m_modelViewMatrixLoc, "modelViewMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                std::make_pair(&m_projectionMatrixLoc, "projectionMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_cascadeViewMatrixLoc, "cascadeViewMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_cascadeFarLoc, "cascadeFar"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_lightDirectionLoc, "lightDirection"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_lightColorLoc, "lightColor"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_specularColorLoc, "specularColor"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_diffuseColorLoc, "diffuseColor"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_ambientColorLoc, "ambientColor"));
    m_uniformLocs[KEYSTR_PROGRAM_RENDER].push_back(
                    std::make_pair(&m_shadowMapSamplerLoc, "shadowMapSampler"));

    m_attribLocs[KEYSTR_PROGRAM_RENDER].push_back(
                std::make_pair(0, "v_position"));
    m_attribLocs[KEYSTR_PROGRAM_RENDER].push_back(
                std::make_pair(1, "v_normal"));

    // shadow map:
    m_uniformLocs[KEYSTR_PROGRAM_SHADOW].push_back(
                    std::make_pair(&m_shadowMapCascadeViewMatrixLoc, "cascadeViewMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_SHADOW].push_back(
                        std::make_pair(&m_shadowMapWorldMatrixLoc, "worldMatrix"));

    // compose
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].push_back(
                std::make_pair(&m_composeProjectionMatrixLoc, "projectionMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].push_back(
                std::make_pair(&m_composeInverseProjectionMatrixLoc, "inverseProjectionMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].push_back(
                std::make_pair(&m_composeDepthBufferLoc, "depthBuffer"));
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].push_back(
                    std::make_pair(&m_composeSamplerLoc, "sampler"));
    m_uniformLocs[KEYSTR_PROGRAM_COMPOSE].push_back(
                    std::make_pair(&m_composeMomentsSamplerLoc, "momentsSampler"));
    m_attribLocs[KEYSTR_PROGRAM_COMPOSE].push_back(
                std::make_pair(0, "v_position"));

    // copy
    m_uniformLocs[KEYSTR_PROGRAM_COPY].push_back(std::make_pair(&m_copyArraySamplerLoc, "sampler"));
    m_uniformLocs[KEYSTR_PROGRAM_COPY].push_back(std::make_pair(&m_copyArrayLayerLoc, "layer"));
    m_attribLocs[KEYSTR_PROGRAM_COPY].push_back(std::make_pair(0, "v_position"));


    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER].emplace_back(&m_reduceDepthInputSizeLoc, "inputSize");

    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].emplace_back(&m_reduceFrustumInputSizeLoc, "inputSize");
    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].emplace_back(&m_reduceFrustumCascadeFarLoc, "cascadeFar");
    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].emplace_back(&m_reduceFrustumScreenToLightMatrixLoc, "screenToLightMatrix");

    // create Programs:
    createProgram(KEYSTR_PROGRAM_RENDER);
    createProgram(KEYSTR_PROGRAM_SHADOW);
    createProgram(KEYSTR_PROGRAM_COMPOSE);
    createProgram(KEYSTR_PROGRAM_COPY);
    createProgram(KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER);
    createProgram(KEYSTR_PROGRAM_REDUCE_DEPTH);
    createProgram(KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER);
    createProgram(KEYSTR_PROGRAM_REDUCE_FRUSTUM);
    createProgram(KEYSTR_PROGRAM_HORIZONTAL_GAUSS);
    createProgram(KEYSTR_PROGRAM_VERTICAL_GAUSS);
    createProgram(KEYSTR_PROGRAM_CREATE_MOMENTS);

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
    glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 4, GL_DEPTH_COMPONENT, m_shadowMapSize, m_shadowMapSize, m_cascades, false);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, 0);

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
    QOpenGLShaderProgram *defaultProgram = m_programs[KEYSTR_PROGRAM_RENDER].get();
    QOpenGLShaderProgram *composeProgram = m_programs[KEYSTR_PROGRAM_COMPOSE].get();
    QOpenGLShaderProgram *reduceDepthSamplerProgram = m_programs[KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER].get();
    QOpenGLShaderProgram *reduceDepthProgram = m_programs[KEYSTR_PROGRAM_REDUCE_DEPTH].get();
    QOpenGLShaderProgram *reduceFrustumSamplerProgram = m_programs[KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER].get();
    QOpenGLShaderProgram *reduceFrustumProgram = m_programs[KEYSTR_PROGRAM_REDUCE_FRUSTUM].get();
    QOpenGLShaderProgram *copyProgram = m_programs[KEYSTR_PROGRAM_COPY].get();
    QOpenGLShaderProgram *verticalGaussProgram = m_programs[KEYSTR_PROGRAM_VERTICAL_GAUSS].get();
    QOpenGLShaderProgram *horizontalGaussProgram = m_programs[KEYSTR_PROGRAM_HORIZONTAL_GAUSS].get();
    QOpenGLShaderProgram *createMomentsProgram = m_programs[KEYSTR_PROGRAM_CREATE_MOMENTS].get();
    QOpenGLShaderProgram *verticalVO = m_programs[KEYSTR_PROGRAM_VERTICAL_VO_AREA].get();
    QOpenGLShaderProgram *horizontalVO = m_programs[KEYSTR_PROGRAM_HORIZONTAL_VO_AREA].get();

    // Input: lightDirection, cameraProjection, cameraView, frustum
    // Output: lightProjection

    // Compute these values, they will be given to the gpu!
    std::vector<QMatrix4x4> cascadeViews;
    std::vector<float> cascadeFars;

    // Inverse of common transformations...
    auto lastInverseCameraView = m_lastCameraView.inverted();
    auto lastInverseCameraProjection = scene->getCameraProjection().inverted();
    auto lastInverseCameraTransformation = (scene->getCameraProjection() * m_lastCameraView).inverted();

    QMatrix4x4 lightViewMatrix;
    createLightViewMatrix(scene->getDirectionalLightDirection(), lastInverseCameraView, lightViewMatrix);

    // Compute actual near and far plane!

    // Check last texture for values..
    std::vector<std::pair<float, float>> reducedDepthPixels;
    reducedDepthPixels.resize(m_reduceLastTextureSize);

    glBindTexture(GL_TEXTURE_1D, m_depthReduceTextures.back());
    glGetTexImage(GL_TEXTURE_1D, 0, GL_RG, GL_FLOAT, reducedDepthPixels.data());
    glBindTexture(GL_TEXTURE_1D, 0);

    float minDepth = 1;
    float maxDepth = 1;

    for (auto &p : reducedDepthPixels)
    {
        if (minDepth > p.first)
            minDepth = p.first;
        if (maxDepth > p.second) // 1 -> min, 0 -> max
            maxDepth = p.second;
    }

    QVector3D lastNearFarCorners[] = {
            { 0, 0, minDepth*2 - 1 },
            { 0, 0, maxDepth*-2 + 1 },
    };

    // Transform corners into light view space
    MathUtility::transformVectors(lastInverseCameraProjection, lastNearFarCorners);

    // NOTE: z values are inverted! multiply by -1
    float lastNearPlane = -lastNearFarCorners[0].z();
    float lastFarPlane = -lastNearFarCorners[1].z();

    // Coefficient for combining uniform and logarithmic results
    float lambdaUniLog = 0.3f;

    for (size_t i = 1; i <= static_cast<size_t>(m_cascades); i++)
    {
        // interpolate between actual near/far planes
        float cUni = lastNearPlane + (lastFarPlane - lastNearPlane) * i / m_cascades;
        float cLog = lastNearPlane * std::pow(lastFarPlane / lastNearPlane, static_cast<float>(i) / m_cascades); // COMPLEX!

        // combine cLog and cUni
        float currentZ = lambdaUniLog * cUni + (1 - lambdaUniLog) * cLog;

        if (i != 0)
            cascadeFars.push_back(currentZ);
    }

    // transform farz into current camera view! (take camera movement into account)
    auto cameraView = scene->getCameraView();
    auto cameraMovement = cameraView * lastInverseCameraView * QVector4D(0, 0, 0, 1);
    // transform cascade far planes into screen space
    std::vector<float> cascadeFarScreen;
    for (auto &farz : cascadeFars)
    {
        // negative farz!
        auto result = scene->getCameraProjection() * QVector4D(0, 0, -farz, 1);
        float z = result.z()/result.w();
        cascadeFarScreen.push_back(z * 0.5f + 0.5f);

        farz -= cameraMovement.z();
    }

    // Compute view frustum of camera in light view
    auto lastScreenToLightTransformation = lightViewMatrix * lastInverseCameraTransformation;

    // Compute temporary projection matrix wrapping the whole view frustum!
    // TODO check if this projection is what we need! (looks okay, when used for rendering)
    QMatrix4x4 tempLightProjection;
    createFrustumProjectionMatrix(lastScreenToLightTransformation, tempLightProjection);

    // in shader:
    // inLight = screenToLight * (xy , depth, 1)
    // inLight /= inLight.w
    // branch for depth -> cascades
    // store in appropriate texture
    // store: vec3 minCorner[4], vec3 maxCorner[4]
    // store min[12], max[12]

    // round up
    GLsizei initWidth = (m_width-1) / 2 / 16 + 1;
    GLsizei initHeight = (m_height-1) / 2 / 16 + 1;

    reduceFrustumSamplerProgram->bind();

    reduceFrustumSamplerProgram->setUniformValue(m_reduceFrustumInputSizeLoc, m_width, m_height);
    reduceFrustumSamplerProgram->setUniformValueArray(m_reduceFrustumCascadeFarLoc, cascadeFarScreen.data(), m_cascades-1, 1);
    reduceFrustumSamplerProgram->setUniformValue(m_reduceFrustumScreenToLightMatrixLoc, tempLightProjection * lastScreenToLightTransformation);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);

    glBindImageTexture(1, m_frustumReduceTextureArrays[0], 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

    // for every new pixel spawn a thread group!
    glDispatchCompute(initWidth, initHeight, 1);

    reduceFrustumSamplerProgram->release();

    // size of first reduction result
    GLsizei size = initWidth*initHeight;

    reduceFrustumProgram->bind();

    for (int i = 1; i < m_frustumReduceTextureArrays.size(); i++)
    {
        glBindImageTexture(0, m_frustumReduceTextureArrays[i-1], 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA16);
        glBindImageTexture(1, m_frustumReduceTextureArrays[i], 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

        // round up
        size = (size-1) / 1 / 256 + 1;

        // for every new pixel spawn a thread group!
        glDispatchCompute(size, 1, 1);
    }

    reduceFrustumProgram->release();


    // NOTE: read back values are in texturespace [0, 1]
    QVector4D reducedCorners[6] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};

    std::vector<std::array<float, 4>> reducedFrustumPixels;
    reducedFrustumPixels.resize(m_reduceLastTextureSize*6);

    glBindTexture(GL_TEXTURE_1D_ARRAY, m_frustumReduceTextureArrays.back());
    glGetTexImage(GL_TEXTURE_1D_ARRAY, 0, GL_RGBA, GL_FLOAT, reducedFrustumPixels.data());
    glBindTexture(GL_TEXTURE_1D_ARRAY, 0);

    for (int i = 0; i < 6; i++)
    {
        for (int j = m_reduceLastTextureSize*i; j < m_reduceLastTextureSize*(i+1); j++)
        {
            auto &p = reducedFrustumPixels[j];
            for (int k = 0; k < 4; k++)
            {
                reducedCorners[i][k] = std::min(reducedCorners[i][k], p[k]);
            }
        }
    }

    // sort by cascades
    QVector3D minCornersCascade[4];
    QVector3D maxCornersCascade[4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // transform from texture space to projected space
            minCornersCascade[i][j] = reducedCorners[j][i] * 2 - 1; // min
            maxCornersCascade[i][j] = reducedCorners[3+j][i] * -2 + 1; // max
        }
    }

    // transform min/max corners back into light space
    auto inverseTempLightProjection = tempLightProjection.inverted();
    MathUtility::transformVectors(inverseTempLightProjection, minCornersCascade);
    MathUtility::transformVectors(inverseTempLightProjection, maxCornersCascade);

    // adjust min x value of first cascade to camera movement..
    minCornersCascade[0][0] += cameraMovement.z();

    // for all cascades calculate projection matrix
    for (int i = 0; i < m_cascades; i++)
    {
        QMatrix4x4 cascadeProjection;
        // min/max did happen in projected space => -minZ~>minZ, -maxZ~>maxZ
        cascadeProjection.ortho(minCornersCascade[i].x(), maxCornersCascade[i].x(), minCornersCascade[i].y(), maxCornersCascade[i].y(), -minCornersCascade[i].z(), -maxCornersCascade[i].z());

        // choose shadow maps exactly twice as bit as they should be!
        QMatrix4x4 scaling;
       // scaling.scale(0.5, 0.5);

        cascadeViews.push_back(scaling * cascadeProjection * lightViewMatrix * scene->getCameraView().inverted());
    }

    // light direction from camera's perspective
    auto lightDirection = scene->getCameraView() * QVector4D(scene->getDirectionalLightDirection(), 0);


    // Render to ShadowMap

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);

    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

    // Initialize with max depth.
    glClearColor(0.999990940f, 0.997558594f, 0.893437386f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    shadowMapProgram->bind();


    shadowMapProgram->setUniformValueArray(m_shadowMapCascadeViewMatrixLoc, cascadeViews.data(), m_cascades);

    for (auto &object : scene->getObjects())
    {
        // work in camera view space
        shadowMapProgram->setUniformValue(m_shadowMapWorldMatrixLoc, scene->getCameraView() * object->getWorld());

        object->getModel()->draw();
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);

    shadowMapProgram->release();

    // AMD gpu's are too agressive!
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    // these barriers do work: GL_ALL_BARRIER_BITS GL_SHADER_IMAGE_ACCESS_BARRIER_BIT GL_TEXTURE_FETCH_BARRIER_BIT GL_SHADER_STORAGE_BARRIER_BIT

    // Resolve MSAA depth to moment texture
    createMomentsProgram->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, m_shadowMapDepthBuffer);

    glBindImageTexture(1, m_shadowMapTexture, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

    glDispatchCompute(m_shadowMapSize/8, m_shadowMapSize/8, 4);

    createMomentsProgram->release();

    /*
    glFinish();
    auto start = std::chrono::system_clock::now();

    for (int j = 0; j < 100; j++)
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

    glFinish();
    auto end = std::chrono::system_clock::now();
    m_debugSum += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    m_debugCount++;
    std::cout << (double)m_debugSum/m_debugCount << std::endl;
    m_debugSum = 0;
    m_debugCount = 0;
    */

    // Render to Texture

    glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);

    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_width, m_height);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    defaultProgram->bind();

    defaultProgram->setUniformValue(m_projectionMatrixLoc, scene->getCameraProjection());
    defaultProgram->setUniformValueArray(m_cascadeViewMatrixLoc, cascadeViews.data(), m_cascades);
    defaultProgram->setUniformValueArray(m_cascadeFarLoc, cascadeFars.data(), m_cascades, 1);
    defaultProgram->setUniformValue(m_lightDirectionLoc, QVector3D(lightDirection));
    defaultProgram->setUniformValue(m_lightColorLoc, scene->getLightColor());

    // Bind shadow map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);
    defaultProgram->setUniformValue(m_shadowMapSamplerLoc, 0);

    // Render regular objects
    for (auto &object : scene->getObjects())
    {
        auto cameraModelView = scene->getCameraView() * object->getWorld();


        defaultProgram->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        defaultProgram->setUniformValue(m_specularColorLoc, object->getSpecularColor());
        defaultProgram->setUniformValue(m_diffuseColorLoc, object->getDiffuseColor());
        defaultProgram->setUniformValue(m_ambientColorLoc, object->getAmbientColor());

        object->getModel()->draw();
    }

    // Render debug objects
    for (auto editorObject : scene->getEditorObjects())
    {
        if (!(static_cast<EditorObject *>(editorObject)->isVisible()))
        {
            continue;
        }

        auto cameraModelView = scene->getCameraView() * editorObject->getWorld();

        defaultProgram->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        defaultProgram->setUniformValue(m_specularColorLoc, editorObject->getSpecularColor());
        defaultProgram->setUniformValue(m_diffuseColorLoc, editorObject->getDiffuseColor());
        defaultProgram->setUniformValue(m_ambientColorLoc, editorObject->getAmbientColor());

        editorObject->getModel()->draw();
    }

    // Unbind shadow map texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    defaultProgram->release();

    // gauss moments of screenspace depth:

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


    // Render to Screen

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(m_width/2, 0, m_width/2, m_height/2);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    composeProgram->bind();


    composeProgram->setUniformValue(m_composeSamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0

    composeProgram->setUniformValue(m_composeMomentsSamplerLoc, 1);
    composeProgram->setUniformValue(m_composeDepthBufferLoc,2);
    composeProgram->setUniformValue(m_composeProjectionMatrixLoc, scene->getCameraProjection());
    composeProgram->setUniformValue(m_composeInverseProjectionMatrixLoc, inverseCameraProjection);

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


    // Invoke reduce ...
    // round up
    initWidth = (m_width-1) / 2 / 16 + 1;
    initHeight = (m_height-1) / 2 / 16 + 1;

    reduceDepthSamplerProgram->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);

    reduceDepthSamplerProgram->setUniformValue(m_reduceDepthInputSizeLoc, m_width, m_height);

    glBindImageTexture(1, m_depthReduceTextures[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16);

    // for every new pixel spawn a thread group!
    glDispatchCompute(initWidth, initHeight, 1);

    reduceDepthSamplerProgram->release();

    // size of first reduction result
    size = initWidth*initHeight;

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

    /*
    m_lastCameraView = scene->getCameraView();





    // DEBUG
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, m_width/2, m_height/2);

    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    defaultProgram->bind();

    defaultProgram->setUniformValue(m_projectionMatrixLoc, tempLightProjection);
    defaultProgram->setUniformValue(m_lightDirectionLoc, QVector3D(0, 0, 1));
    defaultProgram->setUniformValue(m_lightColorLoc, scene->getLightColor());

    // Render regular objects
    for (auto &object : scene->getObjects())
    {
        auto cameraModelView = lightViewMatrix * object->getWorld();

        defaultProgram->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        defaultProgram->setUniformValue(m_specularColorLoc, object->getSpecularColor());
        defaultProgram->setUniformValue(m_diffuseColorLoc, object->getDiffuseColor());
        defaultProgram->setUniformValue(m_ambientColorLoc, object->getAmbientColor());

        object->getModel()->draw();
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

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
        defaultProgram->setUniformValue(m_modelViewMatrixLoc, lightViewMatrix);
        defaultProgram->setUniformValue(m_specularColorLoc, QVector3D(0, 0, 0));
        defaultProgram->setUniformValue(m_diffuseColorLoc, QVector3D(0, 0, 0));
        defaultProgram->setUniformValue(m_ambientColorLoc, entry.second);

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
    defaultProgram->setUniformValue(m_specularColorLoc, QVector3D(0, 0, 0));
    defaultProgram->setUniformValue(m_diffuseColorLoc, QVector3D(0, 0, 0));
    defaultProgram->setUniformValue(m_ambientColorLoc, QVector3D(1, 1, 0));

    for (int i = 0; i < 4; i++)
    {
        QMatrix4x4 world;
        world.setToIdentity();
        defaultProgram->setUniformValue(m_modelViewMatrixLoc, world);

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

    defaultProgram->release();


    GLint windowTexture;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &windowTexture);
    */

    /*
    verticalGaussProgram->bind();

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, windowTexture);
    glBindImageTexture(0, windowTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
    glBindImageTexture(1, m_renderTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glDispatchCompute((m_width - 1) / 8 + 1, (m_height - 1) / 8 + 1, 1);

    verticalGaussProgram->release();

    horizontalGaussProgram->bind();

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glBindImageTexture(0, m_renderTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
    glBindImageTexture(1, windowTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glDispatchCompute((m_width - 1) / 8 + 1, (m_height - 1) / 8 + 1, 1);

    horizontalGaussProgram->release();
    */

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
}

void Renderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    glDeleteFramebuffers(1, &m_renderFrameBuffer);
    glDeleteTextures(1, &m_renderTexture);
    glDeleteTextures(1, &m_voMomentsTexture);
    glDeleteTextures(1, &m_renderDepthBuffer);
    glDeleteTextures(1, &m_tempTexture);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, m_width, m_height, 0, GL_RG, GL_FLOAT, 0);
    // Poor filtering. Needed!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create gauss filtered Texture for screenspace depth moments:
    glGenTextures(1, &m_voGaussedMomentsBufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_voGaussedMomentsBufferTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, m_width, m_height, 0, GL_RG, GL_FLOAT, 0);
    // Poor filtering. Needed!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create TEMP
    glGenTextures(1, &m_tempTexture);
    glBindTexture(GL_TEXTURE_2D, m_tempTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_SHORT, 0);
    // Poor filtering. Needed!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    // Create depth buffer!
    glGenTextures(1, &m_renderDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
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

