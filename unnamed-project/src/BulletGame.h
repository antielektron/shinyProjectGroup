#ifndef UNNAMED_PROJECT_BULLET_GAME_H
#define UNNAMED_PROJECT_BULLET_GAME_H

#ifdef HAVE_BULLET

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <QString>
#include <QMatrix4x4>

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

private:
    // TODO maybe public..
    void loadScene(const QString &filename);

    void handleInput(float deltaTime);
    void updateCamera();

    QString m_scenefile;
    std::unique_ptr<Scene> m_scene;

    std::vector<btRigidBody *> m_bodies;
    std::unique_ptr<btRigidBody> m_playerBody;
    QVector3D m_position;
    float m_rotX;
    float m_rotY;

    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btBroadphaseInterface> m_broadphase; // TODO what's this
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_bulletWorld;
};

#endif // HAVE_BULLET

#endif // UNNAMED_PROJECT_BULLET_GAME_H
