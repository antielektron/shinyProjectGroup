#include "BulletGame.h"

#include "BulletHelper.h"

#include "Scene/Scene.h"
#include "KeyManager.h"

#include <iostream>

/*
 * References:
 *
 * Character with spring
 * http://stackoverflow.com/questions/25605659/avoid-ground-collision-with-bullet
 */

#ifdef HAVE_BULLET

BulletGame::BulletGame(const QString &scenefile) :
        m_scenefile(scenefile),
        m_rotX(0.f),
        m_rotY(0.f),
        m_time(0.f)
{
}

BulletGame::~BulletGame()
{
    // nothing to do here...
}

void BulletGame::initialize()
{
    // call parent
    IGame::initialize();

    loadScene(m_scenefile);

    m_scene->getGlobalState()->triggerEvent("init");
}

void BulletGame::resize(int width, int height)
{
    auto &proj = m_scene->getCameraProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.2f, 500.0f);
}

void BulletGame::tick(float dt)
{
    handleInput(dt);

    // update time in globalState:
    // TODO provide player position + time in "external attribute"

    // TODO count time
    m_time += dt;
    m_scene->getGlobalState()->setTime(m_time);

    // run Animataions and run handle game logic events:
    m_scene->getGlobalState()->triggerEvent("tick");
    m_scene->performAnimations(m_time, this);

    // TODO check if we clicked anything!

    m_bulletWorld->stepSimulation(dt);

    m_bulletWorld->debugDrawWorld();

}

void BulletGame::handleInput(float deltaTime)
{
    // NOTE: velocity in camera space.
    QVector3D velocity(0, 0, 0);

    m_keyManager->tick();

    float speed = 10.0f;

    if (m_keyManager->isKeyPressed(Qt::Key_F))
    {
        performInteraction();
    }

    if (m_keyManager->shouldCatchMouse())
    {
        m_rotX += m_keyManager->getRelativeY() * .1;
        m_rotY += m_keyManager->getRelativeX() * .1;
    }

    if (m_keyManager->isKeyDown(Qt::Key_Right))
    {
        m_rotY += 0.5;
    }
    if (m_keyManager->isKeyDown(Qt::Key_Left))
    {
        m_rotY -= 0.5;
    }
    if (m_keyManager->isKeyDown(Qt::Key_Up))
    {
        m_rotX += 0.5;
    }
    if (m_keyManager->isKeyDown(Qt::Key_Down))
    {
        m_rotX -= 0.5;
    }

    if (m_keyManager->isKeyDown(Qt::Key_S))
    {
        velocity += QVector3D(0, 0, speed);
    }
    if (m_keyManager->isKeyDown(Qt::Key_W))
    {
        velocity += QVector3D(0, 0, -speed);
    }
    if (m_keyManager->isKeyDown(Qt::Key_D))
    {
        velocity += QVector3D(speed, 0, 0);
    }
    if (m_keyManager->isKeyDown(Qt::Key_A))
    {
        velocity += QVector3D(-speed, 0, 0);
    }

    if (m_keyManager->isKeyPressed(Qt::Key_Space))
    {
        performJump();
    }

    // Reset camera
    if (m_keyManager->isKeyDown(Qt::Key_R))
    {
        m_playerBody->getWorldTransform().setOrigin(toBulletVector3(m_scene->getPlayerPosition()));
        m_playerBody->setLinearVelocity(btVector3(0, 0, 0));
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
    // auto worldVelocity = QVector3D(m_scene->getCameraView().transposed() * QVector4D(velocity, 0.));
    // m_position += worldVelocity * deltaTime;

    m_playerBody->applyCentralForce(toBulletVector3(worldVelocity));

    // TODO modify camera!
    // NOTE: camera not at center of player!
    m_position = toQVector3D(m_playerBody->getWorldTransform() * btVector3(0., 1.8/2.-0.2, 0.));
    updateCamera();

    // Start/Stop catching mouse
    if (m_keyManager->isKeyPressed(Qt::Key_Escape))
    {
        m_keyManager->setCatchMouse(!m_keyManager->shouldCatchMouse());
    }
}

void BulletGame::updateCamera()
{
    QMatrix4x4 &camera = m_scene->getCameraView();

    QMatrix4x4 xRotation;
    xRotation.rotate(m_rotX, 1, 0, 0);

    QMatrix4x4 yRotation;
    yRotation.rotate(m_rotY, 0, 1, 0);

    QMatrix4x4 translation;
    translation.translate(-1 * m_position);

    QMatrix4x4 rotation = xRotation * yRotation;

    camera = rotation * translation;
}

void BulletGame::updateBulletGeometry(ObjectBase *obj)
{
    if (obj->getObjectType() == ObjectType::ObjectGroup)
    {
        for (auto *child : static_cast<ObjectGroup *>(obj)->getObjects())
        {
            updateBulletGeometry(child);
        }
    }
    else
    {
        auto object = static_cast<Object *>(obj);
        auto body = static_cast<btRigidBody *>(object->getUserPointer());

        auto oldTransform = body->getWorldTransform();

        btTransform transform;
        transform.setFromOpenGLMatrix(object->getWorld().constData());
        body->setWorldTransform(transform);

        auto deltaTransform = oldTransform.inverse() * transform;

        // TODO dynamic framerate
        body->setLinearVelocity(deltaTransform.getOrigin() * 60);

        auto rotationQuaternion = deltaTransform.getRotation();
        if (rotationQuaternion.getAngle() != 0)
        {
            // Manual axis computation.. Bullet thinks that the rotation is to small.
            btScalar s_squared = 1.f - rotationQuaternion.w()*rotationQuaternion.w();
            btScalar s = 1.f/btSqrt(s_squared);
            auto axis = btVector3(rotationQuaternion.x() * s, rotationQuaternion.y() * s, rotationQuaternion.z() * s);

            body->setAngularVelocity(axis*rotationQuaternion.getAngle() * 60);
        }
        else
        {
            body->setAngularVelocity(btVector3(0, 0, 0));
        }
    }
}

void BulletGame::performInteraction()
{
    QMatrix4x4 xRotation;
    xRotation.rotate(m_rotX, 1, 0, 0);
    QMatrix4x4 yRotation;
    yRotation.rotate(m_rotY, 0, 1, 0);
    QMatrix4x4 rotation = xRotation * yRotation;

    // TODO adjust range!!

    btVector3 start = toBulletVector3(m_position);
    btVector3 end = toBulletVector3(m_position + QVector3D(rotation.transposed() * QVector4D(0, 0, -100, 0.)));

    btCollisionWorld::ClosestRayResultCallback callback(start, end);

    // Perform raycast
    m_bulletWorld->rayTest(start, end, callback);

    if(callback.hasHit())
    {
        auto object = (Object *)callback.m_collisionObject->getUserPointer();
        if (object != nullptr)
        {
            auto event = object->getInteractionEvent();
            if (!event.isEmpty())
            {
                // Trigger the event associated to the object
                m_scene->getGlobalState()->triggerEvent(event);
            }
        }
    }
}

void BulletGame::performJump()
{
    // player bottom is at -0.9
    // player eye  is at 0.9-0.2 = 0.7
    // bottom = -0.9-0.7 = -1.6
    btVector3 start = toBulletVector3(m_position);
    btVector3 end = toBulletVector3(m_position + QVector3D(0, -1.6f-0.1f, 0));

    btCollisionWorld::ClosestRayResultCallback callback(start, end);

    // Perform raycast
    m_bulletWorld->rayTest(start, end, callback);

    if(callback.hasHit())
    {
        m_playerBody->applyCentralImpulse(btVector3(0, 10, 0));
    }
}

Scene *BulletGame::getScene()
{
    return m_scene.get();
}

void BulletGame::notify(ObjectBase *obj)
{
    updateBulletGeometry(obj);
}

void BulletGame::loadScene(const QString &filename)
{
    m_scene.reset(new Scene(filename));
    // TODO find out player position!

    // Create the bullet world!
    m_collisionConfiguration.reset(new btDefaultCollisionConfiguration());

    m_dispatcher.reset(new btCollisionDispatcher(m_collisionConfiguration.get()));

    m_broadphase.reset(new btDbvtBroadphase());

    m_solver.reset(new btSequentialImpulseConstraintSolver());

    m_bulletWorld.reset(new btDiscreteDynamicsWorld(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get()));

    m_bulletWorld->setGravity(btVector3(0, -10, 0));

    typedef std::map<Model *, btBvhTriangleMeshShape *> ShapesMapType;
    ShapesMapType availableShapes;
    // Add objects
    for (auto *object : m_scene->getObjects())
    {
        auto it = availableShapes.find(object->getModel());
        if (it == availableShapes.end() || it->first != object->getModel())
        {
            auto *mesh = new btTriangleMesh();

            auto vertices = object->getModel()->getVertices();
            auto indices = object->getModel()->getIndices();

            auto iThVector = [&](unsigned int i)->btVector3
            {
                return toBulletVector3(vertices[indices[i]]);
            };

            for (int i = 0; i < indices.size(); i += 3)
            {
                mesh->addTriangle(iThVector(i), iThVector(i+1), iThVector(i+2));
            }

            auto *shape = new btBvhTriangleMeshShape(mesh, true);
            // Insert with hint!
            it = availableShapes.insert(it, ShapesMapType::value_type(object->getModel(), shape));
        }

        // TODO: remove scaling from transformation!
        btTransform transformation;
        transformation.setFromOpenGLMatrix(object->getWorld().constData());


        // TODO save body - object pair
        btRigidBody *body = new btRigidBody(0, nullptr, it->second, btVector3(0, 0, 0)); // No inertia
        body->setWorldTransform(transformation);
        body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
        m_bulletWorld->addRigidBody(body);

        // link body and object together!
        body->setUserPointer(object);
        object->setUserPointer(body);

        /*
        btCollisionObject *collisionObject = new btCollisionObject();
        collisionObject->setCollisionShape(it->second);
        collisionObject->setWorldTransform(transformation);
        m_bulletWorld->addCollisionObject(collisionObject);

        collisionObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);

        collisionObject->setUserPointer(object);
        object->setUserPointer(collisionObject);
        */
    }

    // Add player to bullet world
    float mass = 1;
    btVector3 inertia;
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(toBulletVector3(m_scene->getPlayerPosition()));

    float radius = 0.4;
    float height = 1.8;
    auto *playerShape = new btCapsuleShape(radius, height-2*radius);
    playerShape->calculateLocalInertia(mass, inertia);

    btDefaultMotionState *motionState = new btDefaultMotionState(transform);
    m_playerBody.reset(new btRigidBody(mass, motionState, playerShape, inertia));
    m_playerBody->setActivationState(DISABLE_DEACTIVATION);

    m_playerBody->setAngularFactor(0);

    m_bulletWorld->addRigidBody(m_playerBody.get());

    // Use constraint to lock the rotation of the player body.
    btTransform tr;
    tr.setIdentity();
    /*
    auto *springConstraint = new btGeneric6DofSpringConstraint(*m_playerBody, tr, false);

    btScalar springRange(7.f);
    springConstraint->setLinearUpperLimit(springRange * btVector3(1., 1., 1.));
    springConstraint->setLinearLowerLimit(-springRange * btVector3(1., 1., 1.));

    springConstraint->setAngularLowerLimit(btVector3(0.f, 0.f, 0.f));
    springConstraint->setAngularUpperLimit(btVector3(0.f, 0.f, 0.f));

    // Enable spring
    for (int i = 0; i < 3; i++)
    {
        springConstraint->enableSpring(i, true);
        springConstraint->setStiffness(i, 4); // period 1 sec for !kG body
        springConstraint->setDamping(i, 1.00f); // add some damping
    }
    m_bulletWorld->addConstraint(springConstraint, false);
    */
}

#endif // HAVE_BULLET
