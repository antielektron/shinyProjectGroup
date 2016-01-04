#ifndef UNNAMED_PROJECT_IRENDERER_H
#define UNNAMED_PROJECT_IRENDERER_H

#include <GL/glew.h>
#include <string>

#include "utility.h"

class Scene;

class IRenderer
{
public:
    virtual ~IRenderer() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() = 0;
    virtual void render(GLuint fbo, Scene *scene) = 0;
    virtual void resize(int width, int height) = 0;
    virtual ShaderErrorType createShaderProgram(const std::string &vs, const std::string &fs) = 0;
    virtual std::string &getVertexShader() = 0;
    virtual std::string &getFragmentShader() = 0;

};

#endif // UNNAMED_PROJECT_IRENDERER_H
