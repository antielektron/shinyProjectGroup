#include "Object.h"

#include "Model.h"

Object::Object(Model *model) :
    m_model(model)
{
    alpha = 0.0;
    specularAmount = 1;
    diffuseAmount = 0.1;
}

void Object::setWorld(const QMatrix4x4 &world)
{
    m_world = world;
}

void Object::setAlpha(float a)
{
    this->alpha = a;
}

void Object::setDiffuseAmount(float diff)
{
    this->diffuseAmount = diff;
}

void Object::setSpecularAmount(float spec)
{
    this->specularAmount = spec;
}

QMatrix4x4 &Object::getWorld()
{
    return m_world;
}

Model *Object::getModel()
{
    return m_model;
}

float Object::getAlpha()
{
    return alpha;
}

float Object::getDiffuseAmount()
{
    return diffuseAmount;
}

float Object::getSpecularAmount()
{
    return specularAmount;
}

