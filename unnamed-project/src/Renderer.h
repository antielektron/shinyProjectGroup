#ifndef UNNAMED_PROJECT_RENDERER_H
#define UNNAMED_PROJECT_RENDERER_H

#include <QOpenGLShaderProgram>

class Scene;

class Renderer
{
public:
    void initialize();
    void render(Scene *scene);
    void resize(int width, int height);

private:
    QOpenGLShaderProgram m_program;
    int m_modelViewLoc;
    int m_projectionLoc;
};

#endif // UNNAMED_PROJECT_RENDERER_H
