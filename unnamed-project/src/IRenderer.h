#ifndef UNNAMED_PROJECT_IRENDERER_H
#define UNNAMED_PROJECT_IRENDERER_H

#include "utility.h"
#include <string>

class Scene;

class IRenderer
{
public:
    virtual ~IRenderer() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() = 0;
    virtual void render(GLuint fbo, Scene *scene) = 0;
    virtual void resize(int width, int height) = 0;
    virtual OpenglErrorType createShaderProgram(std::string vs, std::string fs) = 0;
    virtual std::string &getVertexShader() = 0;
    virtual std::string &getFragmentShader() = 0;

};

#endif // UNNAMED_PROJECT_IRENDERER_H
