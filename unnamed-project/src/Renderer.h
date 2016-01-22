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

class Renderer : public RendererBase
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void initialize() override;
    virtual void onRenderingInternal(GLuint fbo, Scene *scene) override;
    virtual void resize(int width, int height) override;



private:
    void rotateVectorToVector(const QVector3D &source,
                              const QVector3D &destination,
                              QMatrix4x4 &matrix);

    GLsizei m_width;
    GLsizei m_height;

    GLsizei m_shadowMapSize;
    GLsizei m_cascades;

    int m_modelViewMatrixLoc;
    int m_projectionMatrixLoc;
    int m_projectionMatrixLocCompose;
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

    // Copy Shader for texture arrays
    int m_copyArraySamplerLoc;
    int m_copyArrayLayerLoc;

    QOpenGLVertexArrayObject m_quadVao;
    QOpenGLBuffer m_quadVbo;

    GLuint m_renderFrameBuffer;
    GLuint m_renderTexture;
    GLuint m_normalTexture;
    GLuint m_renderDepthBuffer;


};

#endif // UNNAMED_PROJECT_RENDERER_H
