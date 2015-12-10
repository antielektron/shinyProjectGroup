#include "Scene/Object.h"

#include "Scene/Model.h"

Object::Object(Model *model) :
    m_model(model)
{
    specularAmount = 1.;
    diffuseAmount = 0.; //0.4;
    ambientAmount = 0.1;
}

void Object::setDiffuseAmount(float diff)
{
    this->diffuseAmount = diff;
}

void Object::setSpecularAmount(float specular)
{
    this->specularAmount = specular;
}

void Object::setAmbientAmount(float ambient)
{
    this->ambientAmount = ambient;
}

Model *Object::getModel()
{
    return m_model;
}

float Object::getDiffuseAmount()
{
    return diffuseAmount;
}

float Object::getSpecularAmount()
{
    return specularAmount;
}

float Object::getAmbientAmount()
{
    return ambientAmount;
}

