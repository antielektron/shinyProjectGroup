#ifndef UNNAMED_PROJECT_RENDERER_BASE_H
#define UNNAMED_PROJECT_RENDERER_BASE_H

#include <GL/glew.h>
#include <string>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <memory>

#include "utility.h"

#define STRINGIZE(s) #s
#define TOSTR(s) STRINGIZE(s)

#define KEY_SHADER_FRAGMENT Fragment
#define KEY_SHADER_VERTEX Vertex
#define KEY_SHADER_GEOMETRY Geometry
#define KEY_SHADER_COMPUTE Compute

const std::map<QOpenGLShader::ShaderTypeBit, std::string> shaderTypeToString =
{
    { QOpenGLShader::KEY_SHADER_FRAGMENT, TOSTR(KEY_SHADER_FRAGMENT) },
    { QOpenGLShader::KEY_SHADER_VERTEX, TOSTR(KEY_SHADER_VERTEX) },
    { QOpenGLShader::KEY_SHADER_GEOMETRY, TOSTR(KEY_SHADER_GEOMETRY) },
    { QOpenGLShader::KEY_SHADER_COMPUTE, TOSTR(KEY_SHADER_COMPUTE) }
};

const std::map<std::string, QOpenGLShader::ShaderTypeBit> stringToShaderType =
{
    { TOSTR(KEY_SHADER_FRAGMENT), QOpenGLShader::KEY_SHADER_FRAGMENT },
    { TOSTR(KEY_SHADER_VERTEX), QOpenGLShader::KEY_SHADER_VERTEX },
    { TOSTR(KEY_SHADER_GEOMETRY), QOpenGLShader::KEY_SHADER_GEOMETRY },
    { TOSTR(KEY_SHADER_COMPUTE), QOpenGLShader::KEY_SHADER_COMPUTE }
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


class Scene;

class RendererBase
{
public:

    RendererBase();
    virtual ~RendererBase() = default; // better than inline implementation!
                                    // (vtable will not be emmited)

    typedef std::pair<std::string, QOpenGLShader::ShaderTypeBit> ShaderSourcesKeyType;
    typedef std::vector<std::pair<QOpenGLShader::ShaderTypeBit, std::string>> ShaderSourcesType;

    // loading an saving configurations:
    void saveConfiguration(const std::string &filename);
    void loadConfiguration(const std::string &filename);

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() = 0;
    void render(GLuint fbo, Scene *scene);
    virtual void resize(int width, int height) = 0;
    /**
     * @brief createProgram creates an openGlProgram from given shaders
     * @param program name of the Program
     * @return
     */
    virtual ShaderErrorType createProgram(const std::string &program);

    /**
     * @brief setShaderSource   add or replace a shader
     * @param shaderSrc         shader's code
     * @param progName          name of the corresponding program
     * @param type              shader's type
     */
    virtual void setShaderSource(const std::string &shaderSrc,
                   const std::string &progName,
                   QOpenGLShader::ShaderTypeBit type);

    /**
     * @brief getPrograms   append a list of available programs to 'progs'
     * @param progs
     */
    virtual void getPrograms(std::vector<std::string> &progs);

    /**
     * @brief getShadersForProgram  append available shaders for 'progName' to 'shaders'
     * @param shaders
     */
    virtual void getShadersForProgram(const std::string &progName,
                              ShaderSourcesType &shaders);

    virtual void resumeRendering();

    virtual void pauseRendering();

    virtual void requestSingleFrameRendering();

protected:
    virtual void onRenderingInternal(GLuint fbo, Scene *scene) = 0;

    // map for shader programs:
    std::map<std::string, std::unique_ptr<QOpenGLShaderProgram>> m_programs;
    std::map<ShaderSourcesKeyType, std::string> m_sources;

    // uniforms and attrib locations:
    std::map<std::string, std::vector< std::pair<int *, const char *>>> m_uniformLocs;

    std::map<std::string, std::vector< std::pair<int, const char *>>> m_attribLocs;

    // boolean for checking if we're in rendering or pause mode:
    bool m_renderingPaused;
    bool m_singleFrameRenderingRequested;

};

#endif // UNNAMED_PROJECT_RENDERER_BASE_H
