#ifndef UNNAMED_PROJECT_PRIMITIVE_GAME_H
#define UNNAMED_PROJECT_PRIMITIVE_GAME_H

#include "IGame.h"
#include "Scene.h"

class PrimitiveGame : public IGame
{
public:

    virtual void initialize() override;

    virtual void resize(int width, int height) override;

    virtual void tick() override;

    virtual Scene *getScene() override;

private:

    std::unique_ptr<Scene> m_scene;

    Object *m_dummy;
    float r;
};

#endif // UNNAMED_PROJECT_PRIMITIVE_GAME_H
