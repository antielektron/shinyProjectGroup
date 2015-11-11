#include "PrimitiveGame.h"

#include <iostream>

void PrimitiveGame::initialize()
{
    m_scene = std::unique_ptr<Scene>(new Scene());

    // static camera
    auto &camera = m_scene->getCamera();
    camera.setToIdentity();
    camera.translate(0, 0, -10);
    camera.lookAt(QVector3D(0, 5, -10), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    //sun is located "in the right direction behind the user at the sky"
    m_scene->setDirectionalLightDirection(QVector3D(-1,-1,-1));
    m_scene->setLightColor(QVector3D(1,1,1));

    m_scene->addModel("test", std::unique_ptr<Model>(new Model("models/test.obj")));
    m_scene->addModel("audi", std::unique_ptr<Model>(new Model("models/Audi_R8.obj")));
    m_dummy = m_scene->createObject("audi");

    m_dummy->getWorld().setToIdentity();
    r = 0.;
}

void PrimitiveGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void PrimitiveGame::tick()
{
    auto &world = m_dummy->getWorld();
    world.setToIdentity();

    r+= 1.f;
    world.rotate(r, 0, 1, 0);
}

Scene *PrimitiveGame::getScene()
{
    return m_scene.get();
}
