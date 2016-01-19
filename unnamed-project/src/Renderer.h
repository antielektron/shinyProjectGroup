#ifndef UNNAMED_PROJECT_RENDERER_H
#define UNNAMED_PROJECT_RENDERER_H

#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <memory>
#include <map>
#include <unordered_map>
#include <string>

#include "IRenderer.h"
#include "Scene/Scene.h"

#define KEYSTR_PROGRAM_DEFAULT "Default"
#define KEYSTR_PROGRAM_COMPOSE "Compose"
#define KEYSTR_PROGRAM_SHADOW  "Shadow"
#define KEYSTR_PROGRAM_COPY    "Copy"

namespace std
{
    template<>
    struct less<std::pair<std::string, QOpenGLShader::ShaderTypeBit>>
    {
        typedef std::pair<std::string, QOpenGLShader::ShaderTypeBit> type;

        bool operator () (const type &lhs, const type &rhs)
        {
            if (lhs.first == rhs.first)
            {
                return lhs.second < rhs.second;
            }
            else
            {
                return lhs.first < rhs.first;
            }
        }
    };
}

class Renderer : public IRenderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void initialize() override;
    virtual void render(GLuint fbo, Scene *scene) override;
    virtual void resize(int width, int height) override;

    /**
     * @brief setShaderSource   add or replace a shader
     * @param shaderSrc         shader's code
     * @param progName          name of the corresponding program
     * @param type              shader's type
     */
    virtual void setShaderSource(const std::string &shaderSrc,
                   const std::string &progName,
                   QOpenGLShader::ShaderTypeBit type) override;

    /**
     * @brief createProgram creates an openGlProgram from given shaders
     * @param program name of the Program
     * @return
     */
    virtual ShaderErrorType createProgram(const std::string &program) override;

    /**
     * @brief getPrograms   append a list of available programs to 'progs'
     * @param progs
     */
    virtual void getPrograms(std::vector<std::string> &progs) override;

    /**
     * @brief getShadersForProgram  append available shaders for 'progName' to 'shaders'
     * @param shaders
     */
    virtual void getShadersForProgram(const std::string &progName,
                              ShaderSourcesType &shaders) override;

    virtual void resumeRendering() override;

    virtual void pauseRendering() override;

    virtual void requestSingleFrameRendering() override;


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

    // Copy Shader for texture arrays
    int m_copyArraySamplerLoc;
    int m_copyArrayLayerLoc;

    QOpenGLVertexArrayObject m_quadVao;
    QOpenGLBuffer m_quadVbo;

    GLuint m_renderFrameBuffer;
    GLuint m_renderTexture;
    GLuint m_normalTexture;
    GLuint m_renderDepthBuffer;

    // map for shader programs:
    std::map<std::string, std::unique_ptr<QOpenGLShaderProgram>> m_programs;
    std::map<ShaderSourcesKeyType, std::string> m_sources;

    // uniforms and attrib locations:
    std::map<std::string, std::vector< std::pair<int *, const char *>>>
        m_uniformLocs;

    std::map<std::string, std::vector< std::pair<int, const char *>>>
        m_attribLocs;

    // boolean for checking if we're in rendering or pause mode:
    bool m_renderingPaused;
    bool m_singleFrameRenderingRequested;
};

#endif // UNNAMED_PROJECT_RENDERER_H
