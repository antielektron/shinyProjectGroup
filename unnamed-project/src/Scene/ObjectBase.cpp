#include "Scene/ObjectBase.h"

ObjectBase::ObjectBase() : m_scaling(1., 1., 1.)
{}

const QMatrix4x4 &ObjectBase::getWorld()
{
    return m_world;
}

void ObjectBase::updateWorld()
{
    // TODO correct order of things!!!!!!!!
    m_world.setToIdentity();

    m_world.scale(m_scaling);

    m_world.rotate(m_rotation.y(), 0., 1., 0.);
    m_world.rotate(m_rotation.x(), 1., 0., 0.);
    m_world.rotate(m_rotation.z(), 0., 0., 1.);

    m_world.translate(m_position);
}

void ObjectBase::setPosition(const QVector3D &position)
{
    m_position = position;
}

QVector3D &ObjectBase::getPosition()
{
    return m_position;
}

void ObjectBase::setRotation(const QVector3D &rotation)
{
    m_rotation = rotation;
}

QVector3D &ObjectBase::getRotation()
{
    return m_rotation;
}

void ObjectBase::setScaling(const QVector3D &scaling)
{
    m_scaling = scaling;
}

QVector3D &ObjectBase::getScaling()
{
    return m_scaling;
}
