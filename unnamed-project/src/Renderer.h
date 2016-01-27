#ifndef UNNAMED_PROJECT_RENDERER_H
#define UNNAMED_PROJECT_RENDERER_H

#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <memory>
#include <map>
#include <unordered_map>
#include <string>

#include "RendererBase.h"
#include "Scene/Scene.h"

#define KEYSTR_PROGRAM_RENDER "Render"
#define KEYSTR_PROGRAM_COMPOSE "Compose"
#define KEYSTR_PROGRAM_SHADOW  "Shadow"
#define KEYSTR_PROGRAM_COPY    "Copy"
#define KEYSTR_PROGRAM_REDUCE   "Reduce"
#define KEYSTR_PROGRAM_REDUCE_SAMPLER "Reduce from DepthBuffer"
#define KEYSTR_PROGRAM_HORIZONTAL_GAUSS "Horizontal Gauss"
#define KEYSTR_PROGRAM_VERTICAL_GAUSS "Vertical Gauss"

class Renderer : public RendererBase
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void initialize() override;
    virtual void resize(int width, int height) override;



protected:
    virtual void onRenderingInternal(GLuint fbo, Scene *scene) override;
    void rotateVectorToVector(const QVector3D &source,
                              const QVector3D &destination,
                              QMatrix4x4 &matrix);
    void createLightViewMatrix(const QVector3D &lightDir, const QMatrix4x4 &inverseCameraView, QMatrix4x4 &matrix);


    GLsizei m_width;
    GLsizei m_height;

    GLsizei m_shadowMapSize;
    GLsizei m_cascades;

    // Render Shader
    int m_modelViewMatrixLoc;
    int m_projectionMatrixLoc;
    int m_cascadeViewMatrixLoc;
    int m_cascadeFarLoc;
    int m_lightDirectionLoc;
    int m_lightColorLoc;
    int m_specularColorLoc;
    int m_diffuseColorLoc;
    int m_ambientColorLoc;
    int m_shadowMapSamplerLoc;

    // ShadowMap Shader
    int m_shadowMapWorldMatrixLoc;
    int m_shadowMapCascadeViewMatrixLoc;

    GLuint m_shadowMapFrameBuffer;
    GLuint m_shadowMapTexture;
    GLuint m_shadowMapDepthBuffer;

    // Compose Shader
    int m_composeSamplerLoc;
    int m_composeOvSamplerLoc;
    int m_composeProjectionMatrixLoc;

    // Copy Shader for texture arrays
    int m_copyArraySamplerLoc;
    int m_copyArrayLayerLoc;

    // Reduce
    // int m_reduceInverseProjectionMatrixLoc;
    int m_reduceInputSizeLoc;
    std::vector<GLuint> m_depthReduceTextures;
    GLsizei m_reduceLastTextureSize;

    // vertical gauss
    int m_verticalGaussSourceLoc;
    int m_verticalGaussFilteredLoc;

    // horizontal gauss
    int m_horizontalGaussSourceLoc;
    int m_horizontalGaussFilteredLoc;


    QOpenGLVertexArrayObject m_quadVao;
    QOpenGLBuffer m_quadVbo;

    GLuint m_renderFrameBuffer;
    GLuint m_renderTexture;
    GLuint m_normalTexture;
    GLuint m_renderDepthBuffer;

    GLuint m_tempTexture;

    size_t m_debugSum;
    size_t m_debugCount;

};

#endif // UNNAMED_PROJECT_RENDERER_H
