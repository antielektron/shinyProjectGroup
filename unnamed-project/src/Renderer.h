#ifndef UNNAMED_PROJECT_RENDERER_H
#define UNNAMED_PROJECT_RENDERER_H

#include <QOpenGLShaderProgram>

#include "IRenderer.h"
#include "Scene.h"

class Renderer : public IRenderer
{
public:
    virtual void initialize() override;
    virtual void render(Scene *scene) override;
    virtual void resize(int width, int height) override;

private:
    QOpenGLShaderProgram m_program;
    int m_modelViewLoc;
    int m_projectionLoc;
    int m_lightDirectionLoc;
    int m_lightColorLoc;
    int m_alphaLoc;
    int m_specularAmountLoc;
    int m_diffuseAmountLoc;
};

#endif // UNNAMED_PROJECT_RENDERER_H
