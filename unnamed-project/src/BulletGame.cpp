#include "BulletGame.h"

#include "Scene/Scene.h"

BulletGame::BulletGame(const QString &scenefile) :
        m_scenefile(scenefile)
{
}

void BulletGame::initialize()
{
    // call parent
    IGame::initialize();

    loadScene(m_scenefile);
}

void BulletGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void BulletGame::tick(float dt)
{
    // TODO physics & camera
}

Scene *BulletGame::getScene()
{
    return m_scene.get();
}

void BulletGame::loadScene(const QString &filename)
{
    m_scene.reset(new Scene(filename));
    // TODO find out player position!
}
