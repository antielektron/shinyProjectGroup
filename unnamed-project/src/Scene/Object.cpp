#include "Scene/Object.h"

#include "Scene/Model.h"

Object::Object(Model *model) :
    m_model(model)
{
    alpha = 1.0;
    specularAmount = 1;
    diffuseAmount = 0.2;
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

