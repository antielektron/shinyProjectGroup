#include "PrimitiveGame.h"

#include <iostream>

PrimitiveGame::PrimitiveGame(const QString &scenefile) :
        m_wasEscDown(false),
        m_scenefile(scenefile)
{
}

void PrimitiveGame::initialize()
{
    // call parent
    IGame::initialize();

    // Load a test level by default or empty scene
    m_scene.reset(new Scene(m_scenefile));

    m_scene->getSceneRoot()->updateWorld();

    m_position = m_scene->getPlayerPosition();
}

void PrimitiveGame::resize(int width, int height)
{
    auto &proj = m_scene->getCameraProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.2f, 500.0f);
}

void PrimitiveGame::tick(float dt)
{
    QVector3D deltaPos(0, 0, 0);

    if (!m_keyManager)
        return;

    m_keyManager->tick();

    float speed = 10.0f * dt;

    if (m_keyManager->isKeyDown(Qt::Key_Control))
        speed *= 5;

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
    QMatrix4x4 &camera = m_scene->getCameraView();
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
