#include "Scene/ObjectBase.h"

void ObjectBase::setWorld(const QMatrix4x4 &world)
{
    m_world = world;
}

QMatrix4x4 &ObjectBase::getWorld()
{
    return m_world;
}
