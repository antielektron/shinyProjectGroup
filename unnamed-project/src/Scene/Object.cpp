#include "Scene/Object.h"

#include "Scene/Model.h"

Object::Object(Model *model) :
    m_model(model)
{
    specularAmount = 0.5;
    diffuseAmount = 0.5; //0.4;
    ambientAmount = 0.4;
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

void Object::setModel(Model *model)
{
    m_model = model;
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

ObjectType Object::getObjectType()
{
    return ObjectType::Object;
}
