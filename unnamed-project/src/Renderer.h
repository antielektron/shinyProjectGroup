#ifndef UNNAMED_PROJECT_RENDERER_H
#define UNNAMED_PROJECT_RENDERER_H

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
    int m_width;
    int m_height;

    std::string m_currentVertexShader;
    std::string m_currentFragmentShader;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    int m_modelViewLoc;
    int m_projectionLoc;
    int m_lightDirectionLoc;
    int m_lightColorLoc;
    int m_specularColorLoc;
    int m_diffuseColorLoc;
    int m_ambientColorLoc;

    QOpenGLShaderProgram m_composeProgram;
    int m_samplerLoc;


    QOpenGLVertexArrayObject m_quadVao;
    QOpenGLBuffer m_quadVbo;

    GLuint m_frameBuffer;
    GLuint m_renderTexture;
    GLuint m_depthBuffer;
};

#endif // UNNAMED_PROJECT_RENDERER_H
