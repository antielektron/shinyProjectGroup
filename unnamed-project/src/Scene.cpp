#include <cassert>

#include "Scene.h"

void Scene::setCamera(const QMatrix4x4 &camera)
{
    m_camera = camera;
}

void Scene::setProjection(const QMatrix4x4 &proj)
{
    m_proj = proj;
}

QMatrix4x4 &Scene::getCamera()
{
    return m_camera;
}

QMatrix4x4 &Scene::getProjection()
{
    return m_proj;
}

void Scene::addModel(const std::string &name, std::unique_ptr<Model> model)
{
    assert(m_models.find(name) == m_models.end());
    m_models[name].swap(model);
}

Object *Scene::createObject(const std::string &modelName)
{
    assert(m_models.find(modelName) != m_models.end());
    Model *model = m_models[modelName].get();
    Object *object = new Object(model);
    m_objects.push_back(std::unique_ptr<Object>(object)); // construct unique_ptr automatically
    return object;
}

Scene::ObjectIterator Scene::objectsBegin()
{
    return m_objects.cbegin();
}

Scene::ObjectIterator Scene::objectsEnd()
{
    return m_objects.cend();
}

QVector3D &Scene::getDirectionalLightDirection()
{
    return m_directionalLightDirection;
}

QVector3D &Scene::getLightColor()
{
    return m_lightColor;
}

void Scene::setLightColor(const QVector3D &color)
{
    this->m_lightColor = color;
}

void Scene::setDirectionalLightDirection(const QVector3D &direction)
{
    this->m_directionalLightDirection = direction;
}

