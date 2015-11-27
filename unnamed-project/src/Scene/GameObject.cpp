#include "Scene/GameObject.h"

#include "Scene/ObjectBase.h"

GameObject::GameObject(ObjectBase *object) : m_object{object}
{
}

void GameObject::setPosition(const QVector3D &position)
{
    m_position = position;
}

void GameObject::setRotation(const QVector3D &rotation)
{
    m_rotation = rotation;
}

void GameObject::setRotation(float yaw, float pitch, float roll)
{
    // Y = up (yaw)
    // Z = forward (roll)
    // X = left (pitch)
    m_rotation.setX(pitch);
    m_rotation.setY(yaw);
    m_rotation.setZ(roll);
}

ObjectBase *GameObject::getObject()
{
    return m_object;
}

const QVector3D &GameObject::getPosition() const
{
    return m_position;
}

const QVector3D &GameObject::getRotation() const
{
    return m_rotation;
}

void GameObject::applyTransformation()
{
    QMatrix4x4 world = m_object->getWorld();
    world.setToIdentity();

    world.rotate(m_rotation.z(), 0, 0, 1); // roll
    world.rotate(m_rotation.x(), 1, 0, 0); // pitch
    world.rotate(m_rotation.y(), 0, 1, 0); // yaw

    world.translate(m_rotation);
}
