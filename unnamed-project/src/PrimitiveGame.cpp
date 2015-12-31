#include "PrimitiveGame.h"

#include <iostream>

PrimitiveGame::PrimitiveGame() :
        m_wasEscDown(false)
{
}

void PrimitiveGame::initialize()
{
    // call parent
    IGame::initialize();

    m_scene = std::unique_ptr<Scene>(new Scene());

    // static camera
    m_position = QVector3D(0, 0, 5);
    updatePosMatrix(QVector3D(0,0,0));


    m_scene->addModel(std::unique_ptr<Model>(new Model("models/octonorm.obj")));
    m_scene->addModel(std::unique_ptr<Model>(new Model("models/sphere.obj")));
    m_scene->addModel(std::unique_ptr<Model>(new Model("models/suzanne.obj")));

    auto octo = m_scene->createObject("octonorm");
    auto sphere = m_scene->createObject("sphere");
    auto suzanne = m_scene->createObject("suzanne");
    sphere->setPosition(QVector3D(3, 0, 0));
    suzanne->setPosition(QVector3D(6, 0, 0));


    m_scene->getSceneRoot()->updateWorld();

    //set directional light:
    m_scene->setDirectionalLightDirection(QVector3D(0.3, 1.0, 0.3));
    m_scene->setLightColor(QVector3D(1.0, 1.0, 1.0));
}

void PrimitiveGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void PrimitiveGame::tick(float dt)
{
    QVector3D deltaPos(0, 0, 0);

    if (!m_keyManager)
        return;

    m_keyManager->tick();

    float speed = 7.0f * dt;

    if (m_keyManager->shouldCatchMouse())
    {
        rotX += m_keyManager->getRelativeY() * .1;
        rotY += m_keyManager->getRelativeX() * .1;
    }

    if (m_keyManager->isKeyDown(Qt::Key_Right))
    {
        rotY += 0.5;
    }
    if (m_keyManager->isKeyDown(Qt::Key_Left))
    {
        rotY -= 0.5;
    }
    if (m_keyManager->isKeyDown(Qt::Key_Up))
    {
        rotX += 0.5;
    }
    if (m_keyManager->isKeyDown(Qt::Key_Down))
    {
        rotX -= 0.5;
    }
    if (m_keyManager->isKeyDown(Qt::Key_S))
    {
        deltaPos += QVector3D(0, 0, speed);
    }
    if (m_keyManager->isKeyDown(Qt::Key_W))
    {
        deltaPos += QVector3D(0, 0, -speed);
    }
    if (m_keyManager->isKeyDown(Qt::Key_D))
    {
        deltaPos += QVector3D(speed, 0, 0);
    }
    if (m_keyManager->isKeyDown(Qt::Key_A))
    {
        deltaPos += QVector3D(-speed, 0, 0);
    }
    if (m_keyManager->isKeyDown(Qt::Key_Space))
    {
        deltaPos += QVector3D(0, speed, 0);
    }
    
    if (m_keyManager->isKeyDown(Qt::Key_Shift))
    {
        deltaPos += QVector3D(0, -speed, 0);
    }

    // Reset camera
    if (m_keyManager->isKeyDown(Qt::Key_R))
    {
        m_position = QVector3D(0, 0, 0);
        rotX = 0;
        rotY = 0;
    }

    if (rotX > 90.f)
        rotX = 90.f;
    if (rotX < -90.f)
        rotX = -90.f;

    updatePosMatrix(deltaPos);

    // Start/Stop catching mouse
    if (m_keyManager->isKeyDown(Qt::Key_Escape) && !m_wasEscDown)
    {
        m_keyManager->setCatchMouse(!m_keyManager->shouldCatchMouse());
    }
    m_wasEscDown = m_keyManager->isKeyDown(Qt::Key_Escape);
}

Scene *PrimitiveGame::getScene()
{
    return m_scene.get();
}

void PrimitiveGame::updatePosMatrix(QVector3D deltaPos) 
{
    QMatrix4x4 &camera = m_scene->getCamera();
    QMatrix4x4 translation;
    QMatrix4x4 xRotation;
    QMatrix4x4 yRotation;

    xRotation.rotate(rotX, 1, 0, 0);
    yRotation.rotate(rotY, 0, 1, 0);
    translation.translate(-1 * m_position);

    QMatrix4x4 rotation = xRotation * yRotation;

    m_position += rotation.transposed() * deltaPos;

    camera = rotation * translation;
    
}
