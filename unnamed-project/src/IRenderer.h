#ifndef UNNAMED_PROJECT_IRENDERER_H
#define UNNAMED_PROJECT_IRENDERER_H

#include <GL/glew.h>
#include <string>
#include <QOpenGLShader>

#include "utility.h"

#define STRINGIZE(s) #s
#define TOSTR(s) STRINGIZE(s)

#define KEY_SHADER_FRAGMENT Fragment
#define KEY_SHADER_VERTEX Vertex
#define KEY_SHADER_GEOMETRY Geometry

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

class Scene;

class IRenderer
{
public:
    virtual ~IRenderer() = default; // better than inline implementation!
                                    // (vtable will not be emmited)

    typedef std::pair<std::string, QOpenGLShader::ShaderTypeBit> ShaderSourcesKeyType;
    typedef std::vector<std::pair<QOpenGLShader::ShaderTypeBit, std::string>> ShaderSourcesType;

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() = 0;
    virtual void render(GLuint fbo, Scene *scene) = 0;
    virtual void resize(int width, int height) = 0;
    /**
     * @brief createProgram creates an openGlProgram from given shaders
     * @param program name of the Program
     * @return
     */
    virtual ShaderErrorType createProgram(const std::string &program) = 0;

    /**
     * @brief setShaderSource   add or replace a shader
     * @param shaderSrc         shader's code
     * @param progName          name of the corresponding program
     * @param type              shader's type
     */
    virtual void setShaderSource(const std::string &shaderSrc,
                   const std::string &progName,
                   QOpenGLShader::ShaderTypeBit type) = 0;

    /**
     * @brief getPrograms   append a list of available programs to 'progs'
     * @param progs
     */
    virtual void getPrograms(std::vector<std::string> &progs) = 0;

    /**
     * @brief getShadersForProgram  append available shaders for 'progName' to 'shaders'
     * @param shaders
     */
    virtual void getShadersForProgram(const std::string &progName,
                              ShaderSourcesType &shaders) = 0;

    virtual void resumeRendering() = 0;

    virtual void pauseRendering() = 0;

};

#endif // UNNAMED_PROJECT_IRENDERER_H
