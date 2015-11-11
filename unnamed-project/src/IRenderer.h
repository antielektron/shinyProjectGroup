#ifndef UNNAMED_PROJECT_IRENDERER_H
#define UNNAMED_PROJECT_IRENDERER_H

class Scene;

class IRenderer
{
public:
    virtual ~IRenderer() {}

    // OpenGL might not be ready, while the constructor was called!
    virtual void initialize() = 0;
    virtual void render(GLuint fbo, Scene *scene) = 0;
    virtual void resize(int width, int height) = 0;
};

#endif // UNNAMED_PROJECT_IRENDERER_H
