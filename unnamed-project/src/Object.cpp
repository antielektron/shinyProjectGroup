#include "Object.h"

#include "Model.h"

Object::Object(Model *model) :
    m_model(model)
{
}

void Object::setWorld(const QMatrix4x4 &world)
{
    m_world = world;
}

QMatrix4x4 &Object::getWorld()
{
    return m_world;
}

Model *Object::getModel()
{
    return m_model;
}
