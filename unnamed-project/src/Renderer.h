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

#define STRINGIZE(s) #s
#define TOSTR(s) STRINGIZE(s)

#define KEY_SHADER_FRAGMENT Fragment
#define KEY_SHADER_VERTEX Vertex
#define KEY_SHADER_GEOMETRY Geometry

#define KEYSTR_PROGRAM_DEFAULT "Default"
#define KEYSTR_PROGRAM_COMPOSE "Compose"
#define KEYSTR_PROGRAM_SHADOW  "Shadow"
#define KEYSTR_PROGRAM_COPY    "Copy"

const std::map<QOpenGLShader::ShaderTypeBit, std::string> shaderTypeToString =
{
    { QOpenGLShader::KEY_SHADER_FRAGMENT, TOSTR(KEY_SHADER_FRAGMENT) },
    { QOpenGLShader::KEY_SHADER_VERTEX, TOSTR(KEY_SHADER_VERTEX) },
    { QOpenGLShader::KEY_SHADER_GEOMETRY, TOSTR(KEY_SHADER_GEOMETRY) }
};

const std::map<std::string, QOpenGLShader::ShaderTypeBit> stringToShaderType =
{
    { TOSTR(KEY_SHADER_FRAGMENT), QOpenGLShader::KEY_SHADER_FRAGMENT },
    { TOSTR(KEY_SHADER_VERTEX), QOpenGLShader::KEY_SHADER_VERTEX },
    { TOSTR(KEY_SHADER_GEOMETRY), QOpenGLShader::KEY_SHADER_GEOMETRY }
};

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


private:
    void rotateVectorToVector(const QVector3D &source, const QVector3D &destination, QMatrix4x4 &matrix);

    GLsizei m_width;
    GLsizei m_height;

    GLsizei m_shadowMapSize;
    GLsizei m_cascades;

    void createComposeProgram();
    void createShadowMapProgram();

    std::unique_ptr<QOpenGLShaderProgram> m_program;
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
    QOpenGLShaderProgram m_shadowMapProgram;
    int m_shadowMapWorldMatrixLoc;
    int m_shadowMapCascadeViewMatrixLoc;

    GLuint m_shadowMapFrameBuffer;
    GLuint m_shadowMapTexture;
    GLuint m_shadowMapDepthBuffer;

    // Compose Shader
    QOpenGLShaderProgram m_composeProgram;
    int m_composeSamplerLoc;

    // Copy Shader for texture arrays
    QOpenGLShaderProgram m_copyArrayProgram;
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
    std::map<std::string, std::vector< std::pair<int *, const char *>>> m_uniformLocs;
    std::map<std::string, std::vector< std::pair<int, const char *>>> m_attribLocs;
};

#endif // UNNAMED_PROJECT_RENDERER_H
