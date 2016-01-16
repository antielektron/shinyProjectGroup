#include "Scene/Object.h"

#include "Scene/Model.h"

Object::Object(Model *model) :
    m_model(model),
    m_color(1., 1., 1.)
{
    m_specularAmount = 0.5;
    m_diffuseAmount = 0.5; //0.4;
    m_ambientAmount = 0.4;
}

void Object::setColor(const QVector3D &color)
{
    m_color = color;
}

QVector3D &Object::getColor()
{
    return m_color;
}

void Object::setDiffuseAmount(float diff)
{
    this->m_diffuseAmount = diff;
}

void Object::setSpecularAmount(float specular)
{
    this->m_specularAmount = specular;
}

void Object::setAmbientAmount(float ambient)
{
    this->m_ambientAmount = ambient;
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
    return m_diffuseAmount;
}

float Object::getSpecularAmount()
{
    return m_specularAmount;
}


float Object::getAmbientAmount()
{
    return m_ambientAmount;
}

ObjectType Object::getObjectType()
{
    return ObjectType::Object;
}
