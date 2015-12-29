#include "BulletGame.h"

#include "BulletHelper.h"

#include "Scene/Scene.h"
#include "KeyManager.h"

#include <iostream>

BulletGame::BulletGame(const QString &scenefile) :
        m_scenefile(scenefile),
        m_wasEscDown(false)
{
}

void BulletGame::initialize()
{
    // call parent
    IGame::initialize();

    // Create the bullet world!
    m_collisionConfiguration.reset(new btDefaultCollisionConfiguration());

    m_dispatcher.reset(new btCollisionDispatcher(m_collisionConfiguration.get()));

    m_broadphase.reset(new btDbvtBroadphase());

    m_solver.reset(new btSequentialImpulseConstraintSolver());

    m_bulletWorld.reset(new btDiscreteDynamicsWorld(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get()));

//     m_bulletWorld->setGravity(btVector3(0., 0., 0.));
    m_bulletWorld->setGravity(btVector3(0, -200, 0));

    loadScene(m_scenefile);

    for (auto *object : m_scene->getObjects())
    {
        auto minCorner = object->getModel()->getMinExtent();
        auto maxCorner = object->getModel()->getMaxExtent();

        btBoxShape *box = new btBoxShape(btVector3( std::max(-minCorner.x(), maxCorner.x()) + 0.02,
                                                    std::max(-minCorner.y(), maxCorner.y()) + 0.02,
                                                    std::max(-minCorner.z(), maxCorner.z()) + 0.02 ));

        // TODO: remove scaling from transformation!
        btTransform transformation;
        transformation.setFromOpenGLMatrix(object->getWorld().constData());

        // TODO save body - object pair
        btRigidBody *body = new btRigidBody(0., nullptr, box, btVector3(0., 0., 0.)); // No inertia
		body->setWorldTransform(transformation);

        m_bodies.push_back(body);

        m_bulletWorld->addRigidBody(body);
    }

    // add player to bullet world
    float mass = 1.;
    btVector3 inertia;
    btTransform transform;
    transform.setIdentity();

    btSphereShape *sphere = new btSphereShape(1.);
    sphere->calculateLocalInertia(mass, inertia);

    btDefaultMotionState *motionState = new btDefaultMotionState(transform);
    m_playerBody.reset(new btRigidBody(mass, motionState, sphere, inertia));
    m_bulletWorld->addRigidBody(m_playerBody.get());
}

void BulletGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void BulletGame::tick(float dt)
{
    handleInput(dt);



    m_bulletWorld->stepSimulation(dt);


    for (auto *body : m_bodies)
    {
        auto player = body->getWorldTransform() * btVector3(0., 0., 0.);
        std::cout << player.x() << " " << player.y() << " " << player.z() << std::endl;
    }
    auto player = m_playerBody->getWorldTransform() * btVector3(0., 0., 0.);
    std::cout << player.x() << " " << player.y() << " " << player.z() << std::endl;

    std::cout << std::endl;

    // TODO physics & camera

}

void BulletGame::handleInput(float deltaTime)
{
    // NOTE: velocity in camera space.
    QVector3D velocity(0, 0, 0);

    m_keyManager->tick();

    float speed = 10.0f;

    if (m_keyManager->shouldCatchMouse())
    {
        m_rotX += m_keyManager->getRelativeY() * .1;
        m_rotY += m_keyManager->getRelativeX() * .1;
    }

    if (m_keyManager->isKeyPressed(Qt::Key_Right))
    {
        m_rotY += 0.5;
    }
    if (m_keyManager->isKeyPressed(Qt::Key_Left))
    {
        m_rotY -= 0.5;
    }
    if (m_keyManager->isKeyPressed(Qt::Key_Up))
    {
        m_rotX += 0.5;
    }
    if (m_keyManager->isKeyPressed(Qt::Key_Down))
    {
        m_rotX -= 0.5;
    }

    if (m_keyManager->isKeyPressed(Qt::Key_S))
    {
        velocity += QVector3D(0, 0, speed);
    }
    if (m_keyManager->isKeyPressed(Qt::Key_W))
    {
        velocity += QVector3D(0, 0, -speed);
    }
    if (m_keyManager->isKeyPressed(Qt::Key_D))
    {
        velocity += QVector3D(speed, 0, 0);
    }
    if (m_keyManager->isKeyPressed(Qt::Key_A))
    {
        velocity += QVector3D(-speed, 0, 0);
    }
    if (m_keyManager->isKeyPressed(Qt::Key_Space) || m_keyManager->isKeyPressed(Qt::Key_Q))
    {
        velocity += QVector3D(0, speed, 0);
    }
    if (m_keyManager->isKeyPressed(Qt::Key_Shift) || m_keyManager->isKeyPressed(Qt::Key_Z) || m_keyManager->isKeyPressed(Qt::Key_Y) || m_keyManager->isKeyPressed(Qt::Key_C))
    {
        velocity += QVector3D(0, -speed, 0);
    }

    // Reset camera
    if (m_keyManager->isKeyPressed(Qt::Key_R))
    {
        m_position = QVector3D(0, 0, 0);
        m_rotX = 0;
        m_rotY = 0;
    }

    if (m_rotX > 90.f)
        m_rotX = 90.f;
    if (m_rotX < -90.f)
        m_rotX = -90.f;

    // Player up will be "world" up.
    // Player forward/sideward will be in xz plane.
    QMatrix4x4 halfCameraTransformation;
    halfCameraTransformation.rotate(m_rotY, 0., 1., 0.);
    auto worldVelocity = QVector3D(halfCameraTransformation.transposed() * QVector4D(velocity, 0.));
    // auto worldVelocity = QVector3D(m_scene->getCamera().transposed() * QVector4D(velocity, 0.));
    // m_position += worldVelocity * deltaTime;

    m_playerBody->set

    m_playerBody->setLinearVelocity(toBulletVector3(worldVelocity));
    m_playerBody->activate(true);

    // TODO modify camera!
    m_position = toQVector3D(m_playerBody->getWorldTransform() * btVector3(0., 0., 0.));
    updateCamera();

    // Start/Stop catching mouse
    if (m_keyManager->isKeyPressed(Qt::Key_Escape) && !m_wasEscDown)
    {
        m_keyManager->setCatchMouse(!m_keyManager->shouldCatchMouse());
    }
    m_wasEscDown = m_keyManager->isKeyPressed(Qt::Key_Escape);
}

void BulletGame::updateCamera()
{
    QMatrix4x4 &camera = m_scene->getCamera();

    QMatrix4x4 xRotation;
    xRotation.rotate(m_rotX, 1, 0, 0);

    QMatrix4x4 yRotation;
    yRotation.rotate(m_rotY, 0, 1, 0);

    QMatrix4x4 translation;
    translation.translate(-1 * m_position);

    QMatrix4x4 rotation = xRotation * yRotation;

    camera = rotation * translation;
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
