#include "PrimitiveGame.h"

void PrimitiveGame::initialize()
{
    m_scene = std::unique_ptr<Scene>(new Scene());

    // static camera
    auto &camera = m_scene->getCamera();
    camera.setToIdentity();
    camera.translate(0, 0, -10);

    m_scene->addModel("test", std::unique_ptr<Model>(new Model("models/test.obj")));
    m_dummy = m_scene->createObject("test");
}

void PrimitiveGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void PrimitiveGame::tick()
{
    QMatrix4x4 world;
    world.setToIdentity(); // lame!
    m_dummy->setWorld(world);
}

Scene *PrimitiveGame::getScene()
{
    return m_scene.get();
}
