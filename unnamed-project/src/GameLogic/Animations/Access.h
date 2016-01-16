#ifndef UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ACCESS_H
#define UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ACCESS_H

// TODO find correct place for this header

#include "Scene/Object.h"
#include "Scene/Scene.h"

struct AccessObjectPosition
{
    typedef QVector3D ValueType;
    typedef ObjectBase ObjectType;

    AccessObjectPosition(ObjectType *object) : m_object(object) {}

    const ValueType &get()
    {
        return m_object->getPosition();
    }

    void set(const ValueType &value)
    {
        m_object->setPosition(value);
    }

    ObjectType *m_object;
};

struct AccessObjectRotation
{
    typedef QVector3D ValueType;
    typedef ObjectBase ObjectType;

    AccessObjectRotation(ObjectBase *object) : m_object(object) {}

    const ValueType &get()
    {
        return m_object->getRotation();
    }

    void set(const ValueType &value)
    {
        m_object->setRotation(value);
    }

    ObjectType *m_object;
};

struct AccessObjectAmbientColor
{
    typedef QVector3D ValueType;
    typedef Object ObjectType;

    AccessObjectAmbientColor(Object *object) : m_object(object) {}

    const ValueType &get()
    {
        return m_object->getAmbientColor();
    }

    void set(const ValueType &value)
    {
        m_object->setAmbientColor(value);
    }

    ObjectType *m_object;
};

struct AccessObjectDiffuseColor
{
    typedef QVector3D ValueType;
    typedef Object ObjectType;

    AccessObjectDiffuseColor(Object *object) : m_object(object) {}

    const ValueType &get()
    {
        return m_object->getDiffuseColor();
    }

    void set(const ValueType &value)
    {
        m_object->setDiffuseColor(value);
    }

    ObjectType *m_object;
};

struct AccessObjectSpecularColor
{
    typedef QVector3D ValueType;
    typedef Object ObjectType;

    AccessObjectSpecularColor(Object *object) : m_object(object) {}

    const ValueType &get()
    {
        return m_object->getSpecularColor();
    }

    void set(const ValueType &value)
    {
        m_object->setSpecularColor(value);
    }

    ObjectType *m_object;
};

struct AccessLightColor
{
    typedef QVector3D ValueType;

    AccessLightColor(Scene *scene) : m_scene(scene) {}

    const ValueType &get()
    {
        return m_scene->getLightColor();
    }

    void set(const ValueType &value)
    {
        m_scene->setLightColor(value);
    }

    Scene *m_scene;
};

struct AccessLightDirection
{
    typedef QVector3D ValueType;

    AccessLightDirection(Scene *scene) : m_scene(scene) {}

    const ValueType &get()
    {
        return m_scene->getDirectionalLightDirection();
    }

    void set(const ValueType &value)
    {
        m_scene->setDirectionalLightDirection(value);
    }

    Scene *m_scene;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ANIMATIONS_ACCESS_H
