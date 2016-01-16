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


enum class ShaderType
{
    Fragment,
    Vertex,
    Geometry
};

const std::map<ShaderType, std::string> shaderTypeToString =
{
    { ShaderType::KEY_SHADER_FRAGMENT, TOSTR(KEY_SHADER_FRAGMENT) },
    { ShaderType::KEY_SHADER_VERTEX, TOSTR(KEY_SHADER_VERTEX) },
    { ShaderType::KEY_SHADER_GEOMETRY, TOSTR(KEY_SHADER_GEOMETRY) }
};

const std::map<std::string, ShaderType> stringToShaderType =
{
    { TOSTR(KEY_SHADER_FRAGMENT), ShaderType::KEY_SHADER_FRAGMENT },
    { TOSTR(KEY_SHADER_VERTEX), ShaderType::KEY_SHADER_VERTEX },
    { TOSTR(KEY_SHADER_GEOMETRY), ShaderType::KEY_SHADER_GEOMETRY }
};

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

    typedef std::pair<std::string, ShaderType> ShaderSourcesKeyType;
    typedef std::vector<std::pair<ShaderType, std::string>> ShaderSourcesType;

    void createProgram(const std::string &program, const ShaderSourcesType &sources);

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
    std::unordered_map<ShaderSourcesKeyType, std::string> m_sources;

    // uniforms and attrib locations:
    std::map<std::string, std::pair<int *, std::string>> m_uniformKeys;
};

#endif // UNNAMED_PROJECT_RENDERER_H
