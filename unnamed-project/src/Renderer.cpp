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
    m_normalTexture(0),
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
    glDeleteTextures(1, &m_normalTexture);
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
    setShaderSource(loadTextFile("shaders/copy_fragment.glsl"),
                    KEYSTR_PROGRAM_COMPOSE,
                    QOpenGLShader::Fragment);
    // copy
    //setShaderSource(loadTextFile("shaders/copy_vertex.glsl"),
    //                KEYSTR_PROGRAM_COPY,
    //                QOpenGLShader::Vertex);
    //setShaderSource(loadTextFile("shaders/fragment_array_copy.glsl"),
    //                KEYSTR_PROGRAM_COPY,
    //                QOpenGLShader::Fragment);

    // Reduce shader
    setShaderSource(loadTextFile("shaders/reduce/reduce_sampler.glsl"),
                    KEYSTR_PROGRAM_REDUCE_SAMPLER,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/reduce/reduce_compute.glsl"),
                    KEYSTR_PROGRAM_REDUCE,
                    QOpenGLShader::Compute);

    // Gauss Filter shaders
    setShaderSource(loadTextFile("shaders/filter/vertical_gauss.glsl"),
                    KEYSTR_PROGRAM_VERTICAL_GAUSS,
                    QOpenGLShader::Compute);

    setShaderSource(loadTextFile("shaders/filter/horizontal_gauss.glsl"),
                    KEYSTR_PROGRAM_HORIZONTAL_GAUSS,
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
                    std::make_pair(&m_composeOvSamplerLoc, "ovSampler"));
    m_attribLocs[KEYSTR_PROGRAM_COMPOSE].push_back(
                std::make_pair(0, "v_position"));

    // copy
    //m_uniformLocs[KEYSTR_PROGRAM_COPY].push_back(
    //                std::make_pair(&m_copyArraySamplerLoc, "sampler"));
    //m_uniformLocs[KEYSTR_PROGRAM_COPY].push_back(
    //                std::make_pair(&m_copyArrayLayerLoc, "layer"));
    //m_attribLocs[KEYSTR_PROGRAM_COPY].push_back(
    //            std::make_pair(0, "v_position"));


    m_uniformLocs[KEYSTR_PROGRAM_REDUCE_SAMPLER].emplace_back(&m_reduceInputSizeLoc, "inputSize");

    m_uniformLocs[KEYSTR_PROGRAM_HORIZONTAL_GAUSS].emplace_back(&m_verticalGaussSourceLoc, "sourceImage");
    m_uniformLocs[KEYSTR_PROGRAM_HORIZONTAL_GAUSS].emplace_back(&m_verticalGaussFilteredLoc, "filteredImage");

    m_uniformLocs[KEYSTR_PROGRAM_VERTICAL_GAUSS].emplace_back(&m_verticalGaussSourceLoc, "sourceImage");
    m_uniformLocs[KEYSTR_PROGRAM_VERTICAL_GAUSS].emplace_back(&m_verticalGaussFilteredLoc, "filteredImage");

    // create Programs:
    createProgram(KEYSTR_PROGRAM_RENDER);
    createProgram(KEYSTR_PROGRAM_SHADOW);
    createProgram(KEYSTR_PROGRAM_COMPOSE);
    // createProgram(KEYSTR_PROGRAM_COPY);
    createProgram(KEYSTR_PROGRAM_REDUCE_SAMPLER);
    createProgram(KEYSTR_PROGRAM_REDUCE);
    createProgram(KEYSTR_PROGRAM_HORIZONTAL_GAUSS);
    createProgram(KEYSTR_PROGRAM_VERTICAL_GAUSS);

    // make stuff
    // --> default program (nothing to do) <------------------------------------

    // --> shadow map program <-------------------------------------------------
    // Create ShadowMap
    m_shadowMapSize = 1024;
    m_cascades = 4;

    // TODO don't render to texture but reuse depth buffer!!!

    // Create Texture
    glGenTextures(1, &m_shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R16, m_shadowMapSize, m_shadowMapSize, m_cascades, 0, GL_RED, GL_UNSIGNED_SHORT, 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // Create DepthBuffer
    glGenTextures(1, &m_shadowMapDepthBuffer);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapDepthBuffer);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, m_shadowMapSize, m_shadowMapSize, m_cascades, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // Create FrameBuffer
    glGenFramebuffers(1, &m_shadowMapFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMapDepthBuffer, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_shadowMapTexture, 0);

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

void Renderer::rotateVectorToVector(const QVector3D &source,
                                    const QVector3D &destination,
                                    QMatrix4x4 &matrix)
{
    QVector3D rotationAxis = QVector3D::crossProduct(source, destination);
    float rotationAngle = std::acos(QVector3D::dotProduct(source, destination))*180.f/
                          static_cast<float>(M_PI);

    matrix.rotate(rotationAngle, rotationAxis);
}

void Renderer::onRenderingInternal(GLuint fbo, Scene *scene)
{

    QOpenGLShaderProgram *shadowMapProgram = m_programs[KEYSTR_PROGRAM_SHADOW].get();
    QOpenGLShaderProgram *defaultProgram = m_programs[KEYSTR_PROGRAM_RENDER].get();
    QOpenGLShaderProgram *composeProgram = m_programs[KEYSTR_PROGRAM_COMPOSE].get();
    QOpenGLShaderProgram *reduceStartProgram = m_programs[KEYSTR_PROGRAM_REDUCE_SAMPLER].get();
    QOpenGLShaderProgram *reduceProgram = m_programs[KEYSTR_PROGRAM_REDUCE].get();
    // QOpenGLShaderProgram *copyProgram = m_programs[KEYSTR_PROGRAM_COPY].get();
    QOpenGLShaderProgram *verticalGaussProgram = m_programs[KEYSTR_PROGRAM_VERTICAL_GAUSS].get();
    QOpenGLShaderProgram *horizontalGaussProgram = m_programs[KEYSTR_PROGRAM_HORIZONTAL_GAUSS].get();

    // Input: lightDirection, cameraProjection, cameraView, frustum
    // Output: lightProjection

    // Rotate light direction to z
    QMatrix4x4 lightViewRotation;
    rotateVectorToVector(scene->getDirectionalLightDirection().normalized(),
                         QVector3D(0, 0, 1),
                         lightViewRotation);

    // Compute viewFrustum of camera in light view
    auto inverseCameraProjection = scene->getCameraProjection().inverted();
    auto inverseCameraTransformation = (scene->getCameraProjection() *
                                        scene->getCameraView()).inverted();
    auto screenToLightTransformation = lightViewRotation *
                                       inverseCameraTransformation;

    // Corners of slices
    std::vector<std::vector<QVector3D>> sliceCorners(static_cast<size_t>(m_cascades)+1);

    // Compute these values, they will be given to the gpu!
    std::vector<QMatrix4x4> cascadeViews;
    std::vector<float> cascadeFars;


    // Compute actual near and far plane!

    // Check last texture for values..
    std::vector<std::pair<float, float>> reducedDepthPixels;
    reducedDepthPixels.resize(m_reduceLastTextureSize);

    glBindTexture(GL_TEXTURE_2D, m_depthReduceTextures.back());
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, reducedDepthPixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    float minDepth = 1;
    float maxDepth = 0;

    for (auto &p : reducedDepthPixels)
    {
        if (minDepth > p.first)
            minDepth = p.first;
        if (maxDepth < p.second)
            maxDepth = p.second;
    }


    QVector3D minCorners[] = {
            { -1, -1, -1 },
            {  1, -1, -1 },
            { -1,  1, -1 },
            {  1,  1, -1 }
    };
    QVector3D maxCorners[] = {
            { -1, -1,  1 },
            {  1, -1,  1 },
            { -1,  1,  1 },
            {  1,  1,  1 }
    };

    QVector3D frustumNearFarCorners[] = {
            { 0, 0, -1 },
            { 0, 0,  1 }
    };

    QVector3D actualNearFarCorners[] = {
            { 0, 0, minDepth*2 - 1 },
            { 0, 0, maxDepth*2 - 1 },
    };

    // Transform corners into light view space
    MathUtility::transformVectors(screenToLightTransformation, minCorners);
    MathUtility::transformVectors(screenToLightTransformation, maxCorners);

    // Transform corners into light view space
    MathUtility::transformVectors(inverseCameraProjection, frustumNearFarCorners);
    MathUtility::transformVectors(inverseCameraProjection, actualNearFarCorners);

    // NOTE: z values are inverted! multiply by -1
    float projectionNearPlane = -frustumNearFarCorners[0].z();
    float projectionFarPlane = -frustumNearFarCorners[1].z();

    float actualNearPlane = -actualNearFarCorners[0].z();
    float actualFarPlane = -actualNearFarCorners[1].z();

    // Coefficient for combining uniform and logarithmic results
    float lambdaUniLog = 0.3f;

    // Interpolate corners in light view space, not screen space
    for (size_t i = 0; i <= static_cast<size_t>(m_cascades); i++)
    {
        // interpolate between actual near/far planes
        float cUni = actualNearPlane + (actualFarPlane - actualNearPlane) * i / m_cascades;
        float cLog = actualNearPlane * std::pow(actualFarPlane / actualNearPlane, static_cast<float>(i) / m_cascades); // COMPLEX!

        // combine cLog and cUni
        float currentZ = lambdaUniLog * cUni + (1 - lambdaUniLog) * cLog;

        if (i != 0)
            cascadeFars.push_back(currentZ);

        // Coefficient for affine combination of frustum corners
        // NOTE: use projection near/far plane
        float coeff = (currentZ - projectionNearPlane) / (projectionFarPlane - projectionNearPlane);

        for (size_t j = 0; j < 4; j++)
        {
            sliceCorners[i].push_back((1 - coeff) * minCorners[j] + coeff * maxCorners[j]);
        }
    }

    // For each cascade combine two slices
    for (size_t i = 0; i < static_cast<size_t>(m_cascades); i++)
    {
        // compute MINIMAL bounding box of frustumCorners
        float minZ = sliceCorners[i].front().z();
        float maxZ = sliceCorners[i].front().z();
        std::vector<QVector2D> corners2D;

        for (const auto &corner : sliceCorners[i])
        {
            corners2D.push_back(corner.toVector2D());
            if (minZ > corner.z())
                minZ = corner.z();
            if (maxZ < corner.z())
                maxZ = corner.z();

        }
        for (const auto &corner : sliceCorners[i+1])
        {
            corners2D.push_back(corner.toVector2D());
            if (minZ > corner.z())
                minZ = corner.z();
            if (maxZ < corner.z())
                maxZ = corner.z();
        }

        // Compute on x-y-plane the minimal bounding rectangle
        std::vector<QVector2D> hull2D;
        QVector2D maxCorner2D;
        QVector2D minCorner2D;
        float frustumRotationAngle;

        MathUtility::getConvexHull(corners2D, hull2D);
        MathUtility::getMinimalBoundingBox(hull2D,
                                           minCorner2D,
                                           maxCorner2D,
                                           frustumRotationAngle);

        // rotate such that minimal bounding box is aabb
        QMatrix4x4 lightViewRotationZ;
        lightViewRotationZ.rotate(frustumRotationAngle * 180.f / static_cast<float>(M_PI), QVector3D(0, 0, -1));

        // compute light view projection
        QMatrix4x4 lightViewProjection;
        lightViewProjection.ortho(minCorner2D.x(), maxCorner2D.x(), minCorner2D.y(), maxCorner2D.y(), -maxZ, -minZ); // TODO find out why near/far plane are so?

        // NOTE: will work in view space not world space while rendering!
        cascadeViews.push_back(lightViewProjection * lightViewRotationZ * lightViewRotation * scene->getCameraView().inverted());
    }


    // light direction from camera's perspective
    auto lightDirection = scene->getCameraView() * QVector4D(scene->getDirectionalLightDirection(), 0.);


    // Render to ShadowMap

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFrameBuffer);

    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

    // Initialize with max depth.
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    shadowMapProgram->bind();


    shadowMapProgram->setUniformValueArray(m_shadowMapCascadeViewMatrixLoc, cascadeViews.data(), m_cascades);

    for (auto &object : scene->getObjects())
    {
        // work in camera view space
        shadowMapProgram->setUniformValue(m_shadowMapWorldMatrixLoc, scene->getCameraView() * object->getWorld());

        object->getModel()->draw();
    }

    glDisable(GL_DEPTH_TEST);

    shadowMapProgram->release();



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



    // Render to Screen

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    composeProgram->bind();


    composeProgram->setUniformValue(m_composeSamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0

    composeProgram->setUniformValue(m_composeOvSamplerLoc, 1);
    composeProgram->setUniformValue(m_composeDepthBufferLoc,2);
    composeProgram->setUniformValue(m_composeProjectionMatrixLoc, scene->getCameraProjection());
    composeProgram->setUniformValue(m_composeInverseProjectionMatrixLoc, inverseCameraProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalTexture);
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


    /*
    */
    // Invoke reduce ...
    GLsizei prevWidth = m_width;
    GLsizei prevHeight = m_height;

    reduceStartProgram->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderDepthBuffer);

    reduceStartProgram->setUniformValue(m_reduceInputSizeLoc, m_width, m_height);

    glBindImageTexture(1, m_depthReduceTextures[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16);

    // round up
    prevWidth = (prevWidth-1) / 2 / 8 + 1;
    prevHeight = (prevHeight-1) / 2 / 8 + 1;

    // for every new pixel spawn a thread group!
    glDispatchCompute(prevWidth, prevHeight, 1);

    reduceStartProgram->release();

    reduceProgram->bind();

    for (int i = 1; i < m_depthReduceTextures.size(); i++)
    {
        glBindImageTexture(0, m_depthReduceTextures[i-1], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG16);
        glBindImageTexture(1, m_depthReduceTextures[i], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16);

        // round up
        prevWidth = (prevWidth-1) / 2 / 8 + 1;
        prevHeight = (prevHeight-1) / 2 / 8 + 1;

        // for every new pixel spawn a thread group!
        glDispatchCompute(prevWidth, prevHeight, 1);
    }

    reduceProgram->release();

    /*
    */
    /*
    GLint windowTexture;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &windowTexture);
    */

    /*
    verticalGaussProgram->bind();

    glBindImageTexture(0, windowTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
    glBindImageTexture(1, m_renderTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glDispatchCompute((m_width - 1) / 8 + 1, (m_height - 1) / 8 + 1, 1);

    verticalGaussProgram->release();


    horizontalGaussProgram->bind();

    glBindImageTexture(0, m_renderTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
    glBindImageTexture(1, windowTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glDispatchCompute((m_width - 1) / 8 + 1, (m_height - 1) / 8 + 1, 1);

    horizontalGaussProgram->release();
    */

    /*
    copyProgram->bind();

    copyProgram->setUniformValue(m_copyArraySamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapDepthBuffer);

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

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    copyProgram->release();
    */
}

void Renderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    glDeleteFramebuffers(1, &m_renderFrameBuffer);
    glDeleteTextures(1, &m_renderTexture);
    glDeleteTextures(1, &m_normalTexture);
    glDeleteTextures(1, &m_renderDepthBuffer);
    glDeleteTextures(1, &m_tempTexture);
    glDeleteTextures(m_depthReduceTextures.size(), m_depthReduceTextures.data());

    // Create render texture
    glGenTextures(1, &m_renderTexture);
    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // Poor filtering. Needed!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    // Create normal texture
    glGenTextures(1, &m_normalTexture);
    glBindTexture(GL_TEXTURE_2D, m_normalTexture);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
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
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalTexture, 0);
    // Set the list of draw buffers.
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_DEPTH_ATTACHMENT};
    glDrawBuffers(2, attachments);


    // Create reduce textures!
    auto n = std::ceil(std::log(std::min(m_width, m_height)) / std::log(2.*8.));
    m_depthReduceTextures.resize(n);
    glGenTextures(m_depthReduceTextures.size(), m_depthReduceTextures.data());

    GLsizei prevWidth = m_width;
    GLsizei prevHeight = m_height;

    for (int i = 0; i < n; i++)
    {
        // round up
        prevWidth = (prevWidth-1) / 2 / 8 + 1;
        prevHeight = (prevHeight-1) / 2 / 8 + 1;

        glBindTexture(GL_TEXTURE_2D, m_depthReduceTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, prevWidth, prevHeight, 0, GL_RG, GL_UNSIGNED_SHORT, 0);
        // No filtering required
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    m_reduceLastTextureSize = prevWidth * prevHeight;
}

