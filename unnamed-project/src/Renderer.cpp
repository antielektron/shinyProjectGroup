// for glDrawBuffers
#define GL_GLEXT_PROTOTYPES 1
#define _USE_MATH_DEFINES

#include <GL/glew.h>

#include <iostream>
#include <cmath>
#include <QtGui/qopenglframebufferobject.h>
#include <vector>

#include "Renderer.h"
#include "Scene/Scene.h"

#include "MathUtility.h"

#include "utility.h"


Renderer::Renderer() :
    m_renderFrameBuffer(0),
    m_renderTexture(0),
    m_normalTexture(0),
    m_renderDepthBuffer(0)
{
}

//------------------------------------------------------------------------------

Renderer::~Renderer()
{
    // Delete the manually created objects!
    glDeleteFramebuffers(1, &m_renderFrameBuffer);
    glDeleteTextures(1, &m_renderTexture);
    glDeleteTextures(1, &m_normalTexture);
    glDeleteRenderbuffers(1, &m_renderDepthBuffer);
}

//------------------------------------------------------------------------------
void Renderer::setShaderSource(const std::string &shaderSrc,
                               const std::string &progName,
                               QOpenGLShader::ShaderTypeBit type)
{
    m_sources[std::make_pair(progName, type)] = shaderSrc;
}

//------------------------------------------------------------------------------
ShaderErrorType Renderer::createProgram(const std::string &program)
{
    // check whether there are sources for given program
    auto vertexShaderIt = m_sources.find(
                              std::make_pair(program, QOpenGLShader::Vertex));
    auto fragmentShaderIt = m_sources.find(
                                std::make_pair(program, QOpenGLShader::Fragment));
    auto geometryShaderIt = m_sources.find(
                                std::make_pair(program, QOpenGLShader::Geometry));

    if (vertexShaderIt == m_sources.end() || fragmentShaderIt == m_sources.end())
    {
        return ShaderErrorType::MissingSourcesError;
    }

    // create new Program
    std::unique_ptr<QOpenGLShaderProgram> prog(new QOpenGLShaderProgram());

    // append cached vertex and fragment shader code
    if (!prog->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                       vertexShaderIt->second.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
        return ShaderErrorType::VertexShaderError;
    }
    if (!prog->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       fragmentShaderIt->second.c_str()))
    {
        std::cerr << "could not load fragment shader" << std::endl;
        return ShaderErrorType::FragmentShaderError;
    }

    // append cached geometry shader, if there is one
    if (geometryShaderIt != m_sources.end())
    {
        if (!prog->addShaderFromSourceCode(QOpenGLShader::Geometry,
                                           geometryShaderIt->second.c_str()))
        {
            std::cerr << "could not load geometry shader" << std::endl;
            return ShaderErrorType::GeometryShaderError;
        }
    }

    // bind cached attribute locations
    for( const auto &v : m_attribLocs[program])
    {
        prog->bindAttributeLocation(v.second, v.first);
    }

    // link program
    if (!prog->link())
    {
        std::cerr << "could not link shader program" << std::endl;
        return ShaderErrorType::LinkingError;
    }

    // no errors so far, finally move program to our programs map
    QOpenGLShaderProgram *pProg = prog.get();
    m_programs[program] = std::move(prog);

    // get uniform locations:
    pProg->bind();
    for (const auto &v : m_uniformLocs[program])
    {
        *(v.first) = pProg->uniformLocation(v.second);
    }
    pProg->release();
    return ShaderErrorType::NoError;
}

//------------------------------------------------------------------------------
void Renderer::getPrograms(std::vector<std::string> &progs)
{
    // just in case...
    progs.clear();
    for (const auto &pair : m_programs)
    {
        progs.push_back(pair.first);
    }
}

//------------------------------------------------------------------------------
void Renderer::getShadersForProgram(const std::string &progName,
                                    ShaderSourcesType &shaders)
{
    for (const auto &pair : m_sources)
    {
        if (pair.first.first == progName)
        {
            shaders.push_back(std::make_pair(pair.first.second, pair.second));
            // ^ don't get confused!
        }
    }
}

//------------------------------------------------------------------------------
void Renderer::initialize()
{
    // initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // set shaders:
    // default program:
    setShaderSource(loadTextFile("shaders/vertex.glsl"),
                    KEYSTR_PROGRAM_DEFAULT,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/fragment.glsl"),
                    KEYSTR_PROGRAM_DEFAULT,
                    QOpenGLShader::Fragment);
    // shadow map program
    setShaderSource(loadTextFile("shaders/vertex_shadowmap.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/geometry_shadowmap.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Geometry);
    setShaderSource(loadTextFile("shaders/fragment_shadowmap.glsl"),
                    KEYSTR_PROGRAM_SHADOW,
                    QOpenGLShader::Fragment);
    // compose program:
    setShaderSource(loadTextFile("shaders/vertex_copy.glsl"),
                    KEYSTR_PROGRAM_COMPOSE,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/fragment_copy.glsl"),
                    KEYSTR_PROGRAM_COMPOSE,
                    QOpenGLShader::Fragment);
    // copy
    setShaderSource(loadTextFile("shaders/vertex_copy.glsl"),
                    KEYSTR_PROGRAM_COPY,
                    QOpenGLShader::Vertex);
    setShaderSource(loadTextFile("shaders/fragment_copy_array.glsl"),
                    KEYSTR_PROGRAM_COPY,
                    QOpenGLShader::Fragment);

    // generate attrib and uniform locations
    // default:
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                std::make_pair(&m_modelViewMatrixLoc, "modelViewMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                std::make_pair(&m_projectionMatrixLoc, "projectionMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_cascadeViewMatrixLoc, "cascadeViewMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_cascadeFarLoc, "cascadeFar"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_lightDirectionLoc, "lightDirection"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_lightColorLoc, "lightColor"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_specularColorLoc, "specularColor"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_diffuseColorLoc, "diffuseColor"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_ambientColorLoc, "ambientColor"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_shadowMapSamplerLoc, "shadowMapSampler"));

    m_attribLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                std::make_pair(0, "v_position"));
    m_attribLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                std::make_pair(1, "v_normal"));

    // shadow map:
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_shadowMapCascadeViewMatrixLoc, "cascadeViewMatrix"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                        std::make_pair(&m_shadowMapWorldMatrixLoc, "worldMatrix"));

    // compose
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_composeSamplerLoc, "sampler"));
    m_attribLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                std::make_pair(0, "v_position"));

    // copy
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_copyArraySamplerLoc, "sampler"));
    m_uniformLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                    std::make_pair(&m_copyArrayLayerLoc, "layer"));
    m_attribLocs[KEYSTR_PROGRAM_DEFAULT].push_back(
                std::make_pair(0, "v_position"));

    // create Programs:
    createProgram(KEYSTR_PROGRAM_DEFAULT);
    createProgram(KEYSTR_PROGRAM_SHADOW);
    createProgram(KEYSTR_PROGRAM_COMPOSE);
    createProgram(KEYSTR_PROGRAM_COPY);

    // make stuff
    // --> default program (nothing to do) <------------------------------------

    // --> shadow map program <-------------------------------------------------
    // Create ShadowMap
    m_shadowMapSize = 2048;
    m_cascades = 4;

    // TODO don't render to texture but reuse depth buffer!!!

    // Create Texture
    glGenTextures(1, &m_shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED, m_shadowMapSize, m_shadowMapSize, m_cascades, 0, GL_RED, GL_UNSIGNED_SHORT, 0);
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

void Renderer::rotateVectorToVector(const QVector3D &source, const QVector3D &destination, QMatrix4x4 &matrix)
{
    QVector3D rotationAxis = QVector3D::crossProduct(source, destination);
    float rotationAngle = std::acos(QVector3D::dotProduct(source, destination))*180.f/M_PI;

    matrix.rotate(rotationAngle, rotationAxis);
}

void Renderer::render(GLuint fbo, Scene *scene)
{
    /*

    CULLING snipet

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


    // Input: lightDirection, cameraProjection, cameraView, frustum
    // Output: lightProjection

    // Rotate light direction to z
    QMatrix4x4 lightViewRotation;
    rotateVectorToVector(scene->getDirectionalLightDirection().normalized(), QVector3D(0, 0, 1), lightViewRotation);

    // Compute viewFrustum of camera in light view
    auto inverseCameraTransformation = (scene->getCameraProjection() * scene->getCameraView()).inverted();
    auto screenToLightTransformation = lightViewRotation * inverseCameraTransformation;

    // Corners of slices
    std::vector<std::vector<QVector3D>> sliceCorners(m_cascades+1);

    // Compute these values, they will be given to the gpu!
    std::vector<QMatrix4x4> cascadeViews;
    std::vector<float> cascadeFars;

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

    QVector3D nearFarCorners[] = {
            { 0, 0, -1 },
            { 0, 0,  1 }
    };

    // Transform corners into light view space
    mathUtility::transformVectors(screenToLightTransformation, minCorners);
    mathUtility::transformVectors(screenToLightTransformation, maxCorners);

    // Transform corners into light view space
    mathUtility::transformVectors(scene->getCameraProjection().inverted(), nearFarCorners);
    // NOTE: z values are inverted! multiply by -1
    float nearPlane = -nearFarCorners[0].z();
    float farPlane = -nearFarCorners[1].z();

    // Coefficient for combining uniform and logarithmic results
    float lambdaUniLog = 0.5f;

    // Interpolate corners in light view space, not screen space
    for (int i = 0; i <= m_cascades; i++)
    {
        float cUni = nearPlane + (farPlane - nearPlane) * i / m_cascades;
        float cLog = nearPlane * std::pow(farPlane / nearPlane, (float) i / m_cascades); // COMPLEX!

        // combine cLog and cUni
        float currentZ = lambdaUniLog * cUni + (1 - lambdaUniLog) * cLog;

        if (i != 0)
            cascadeFars.push_back(currentZ);

        // Coefficient for affine combination of frustum corners
        float coeff = (currentZ - nearPlane) / (farPlane - nearPlane);

        for (int j = 0; j < 4; j++)
        {
            sliceCorners[i].push_back((1 - coeff) * minCorners[j] + coeff * maxCorners[j]);
        }
    }

    // For each cascade combine two slices
    for (int i = 0; i < m_cascades; i++)
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

        mathUtility::getConvexHull(corners2D, hull2D);
        mathUtility::getMinimalBoundingBox(hull2D,
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

    m_shadowMapProgram.bind();

    m_shadowMapProgram.setUniformValueArray(m_shadowMapCascadeViewMatrixLoc, cascadeViews.data(), m_cascades);

    for (auto &object : scene->getObjects())
    {
        // work in camera view space
        m_shadowMapProgram.setUniformValue(m_shadowMapWorldMatrixLoc, scene->getCameraView() * object->getWorld());

        object->getModel()->draw();
    }

    glDisable(GL_DEPTH_TEST);

    m_shadowMapProgram.release();



    // Render to Texture

    glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);

    // VIEWPORTS FOR SHADOW MAP AND WINDOW IS DIFFERENT!!!
    glViewport(0, 0, m_width, m_height);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_program->bind();

    m_program->setUniformValue(m_projectionMatrixLoc, scene->getCameraProjection());
    m_program->setUniformValueArray(m_cascadeViewMatrixLoc, cascadeViews.data(), m_cascades);
    m_program->setUniformValueArray(m_cascadeFarLoc, cascadeFars.data(), m_cascades, 1);
    m_program->setUniformValue(m_lightDirectionLoc, QVector3D(lightDirection));
    m_program->setUniformValue(m_lightColorLoc, scene->getLightColor());

    // Bind shadow map
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapTexture);
    m_program->setUniformValue(m_shadowMapSamplerLoc, 0);

    // Render regular objects
    for (auto &object : scene->getObjects())
    {
        auto cameraModelView = scene->getCameraView() * object->getWorld();
        // auto lightModelView = lightView * object->getWorld(); // TODO

        m_program->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        // m_program->setUniformValue(m_lightViewMatrixLoc, lightModelView);
        m_program->setUniformValue(m_specularColorLoc, object->getSpecularAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_diffuseColorLoc, object->getDiffuseAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_ambientColorLoc, object->getAmbientAmount() * QVector3D(1., 1., 1.));

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
        // auto lightModelView = lightView * editorObject->getWorld(); // TODO

        m_program->setUniformValue(m_modelViewMatrixLoc, cameraModelView);
        // m_program->setUniformValue(m_lightViewMatrixLoc, lightModelView);
        m_program->setUniformValue(m_specularColorLoc, editorObject->getSpecularAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_diffuseColorLoc, editorObject->getDiffuseAmount() * QVector3D(1., 1., 1.));
        m_program->setUniformValue(m_ambientColorLoc, editorObject->getAmbientAmount() * QVector3D(1., 1., 1.));

        editorObject->getModel()->draw();
    }

    // Unbind shadow map texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    m_program->release();



    // Render to Screen

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_composeProgram.bind();

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_quadVao);

    glActiveTexture(GL_TEXTURE0);
    m_composeProgram.setUniformValue(m_composeSamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0

    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glDrawArrays(GL_QUADS, 0, 4);


    /*
    glViewport(0, 0, m_width/4, m_height/4);
    glBindTexture(GL_TEXTURE_2D, m_normalTexture);
    glDrawArrays(GL_QUADS, 0, 4);
    */

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);

    m_composeProgram.release();


    /*
    m_copyArrayProgram.bind();

    m_copyArrayProgram.setUniformValue(m_copyArraySamplerLoc, 0); //set to 0 because the texture is bound to GL_TEXTURE0
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadowMapDepthBuffer);

    glViewport(m_width*0/4, m_height*3/4, m_width/4, m_height/4);
    m_copyArrayProgram.setUniformValue(m_copyArrayLayerLoc, 0);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*1/4, m_height*3/4, m_width/4, m_height/4);
    m_copyArrayProgram.setUniformValue(m_copyArrayLayerLoc, 1);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*2/4, m_height*3/4, m_width/4, m_height/4);
    m_copyArrayProgram.setUniformValue(m_copyArrayLayerLoc, 2);
    glDrawArrays(GL_QUADS, 0, 4);

    glViewport(m_width*3/4, m_height*3/4, m_width/4, m_height/4);
    m_copyArrayProgram.setUniformValue(m_copyArrayLayerLoc, 3);
    glDrawArrays(GL_QUADS, 0, 4);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    m_copyArrayProgram.release();
    */
}

void Renderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    glDeleteFramebuffers(1, &m_renderFrameBuffer);
    glDeleteTextures(1, &m_renderTexture);
    glDeleteTextures(1, &m_normalTexture);
    glDeleteRenderbuffers(1, &m_renderDepthBuffer);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // Poor filtering. Needed!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    // Create depth buffer!
    glGenRenderbuffers(1, &m_renderDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &m_renderFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderFrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderDepthBuffer);
    // Set "renderTexture" as our colour attachement #0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalTexture, 0);

    // Set the list of draw buffers.
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
}
