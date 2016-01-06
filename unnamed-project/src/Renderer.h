#ifndef UNNAMED_PROJECT_RENDERER_H
#define UNNAMED_PROJECT_RENDERER_H

#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <memory>

#include "IRenderer.h"
#include "Scene/Scene.h"

class Renderer : public IRenderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void initialize() override;
    virtual void render(GLuint fbo, Scene *scene) override;
    virtual void resize(int width, int height) override;
    virtual ShaderErrorType createShaderProgram(const std::string &vs, const std::string &fs) override;
    virtual std::string &getVertexShader() override;
    virtual std::string &getFragmentShader() override;

private:
    GLsizei m_width;
    GLsizei m_height;

    GLsizei m_shadowMapSize;

    void createComposeProgram();
    void createShadowMapProgram();

    std::string m_currentVertexShader;
    std::string m_currentFragmentShader;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    int m_modelViewMatrixLoc;
    int m_projectionMatrixLoc;
    int m_lightViewMatrixLoc;
    int m_lightDirectionLoc;
    int m_lightColorLoc;
    int m_specularColorLoc;
    int m_diffuseColorLoc;
    int m_ambientColorLoc;
    int m_shadowMapSamplerLoc;

    // ShadowMap Shader
    QOpenGLShaderProgram m_shadowMapProgram;
    int m_shadowMapLightViewMatrixLoc;

    GLuint m_shadowMapFrameBuffer;
    GLuint m_shadowMapTexture;
    GLuint m_shadowMapDepthBuffer;

    // Compose Shader
    QOpenGLShaderProgram m_composeProgram;
    int m_composeSamplerLoc;

    QOpenGLVertexArrayObject m_quadVao;
    QOpenGLBuffer m_quadVbo;

    GLuint m_renderFrameBuffer;
    GLuint m_renderTexture;
    GLuint m_normalTexture;
    GLuint m_renderDepthBuffer;
};

#endif // UNNAMED_PROJECT_RENDERER_H
