#include "Scene/Object.h"

#include "Scene/Model.h"

Object::Object(Model *model) :
    m_model(model),
    m_userPointer(nullptr)
{
    m_specularColor = QVector3D(0.5, 0.5, 0.5);
    m_diffuseColor = QVector3D(0.5, 0.5, 0.5); //0.4;
    m_ambientColor = QVector3D(0.4, 0.4, 0.4);
}

void Object::setDiffuseColor(const QVector3D &diffuse)
{
    this->m_diffuseColor = diffuse;
}

void Object::setSpecularColor(const QVector3D &specular)
{
    this->m_specularColor = specular;
}

void Object::setAmbientColor(const QVector3D &ambient)
{
    this->m_ambientColor = ambient;
}

Model *Object::getModel()
{
    return m_model;
}

void Object::setModel(Model *model)
{
    m_model = model;
}

QVector3D &Object::getDiffuseColor()
{
    return m_diffuseColor;
}

QVector3D &Object::getSpecularColor()
{
    return m_specularColor;
}


QVector3D &Object::getAmbientColor()
{
    return m_ambientColor;
}

ObjectType Object::getObjectType()
{
    return ObjectType::Object;
}

void Object::setUserPointer(void *userPointer)
{
    m_userPointer = userPointer;
}

void *Object::getUserPointer()
{
    return m_userPointer;
}

void Object::setInteractionEvent(const QString &interactionEvent)
{
    m_interactionEvent = interactionEvent;
}

const QString &Object::getInteractionEvent()
{
    return m_interactionEvent;
}
