#ifndef UNNAMED_PROJECT_BULLET_GAME_H
#define UNNAMED_PROJECT_BULLET_GAME_H

#include <QString>

#include "IGame.h"

class Scene;

class BulletGame : public IGame
{
public:
    BulletGame(const QString &scenefile);

    virtual void initialize() override;
    virtual void resize(int width, int height) override;
    virtual void tick(float dt) override;

    virtual Scene *getScene() override;

    void loadScene(const QString &filename);

private:
    QString m_scenefile;
    std::unique_ptr<Scene> m_scene;
};

#endif // UNNAMED_PROJECT_BULLET_GAME_H
