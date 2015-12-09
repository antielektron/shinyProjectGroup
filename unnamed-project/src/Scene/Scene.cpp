#include <cassert>

#include "Scene/Scene.h"

//------------------------------------------------------------------------------
Scene::Scene()
{
    ObjectGroup *rootGroup = new ObjectGroup();
    rootGroup->setName("SceneRoot");
    m_rootGroup.reset(rootGroup);
}

//------------------------------------------------------------------------------
void Scene::setCamera(const QMatrix4x4 &camera)
{
    m_camera = camera;
}

//------------------------------------------------------------------------------
void Scene::setProjection(const QMatrix4x4 &proj)
{
    m_proj = proj;
}

//------------------------------------------------------------------------------
QMatrix4x4 &Scene::getCamera()
{
    return m_camera;
}

//------------------------------------------------------------------------------
QMatrix4x4 &Scene::getProjection()
{
    return m_proj;
}

//------------------------------------------------------------------------------
void Scene::addModel(const std::string &name, std::unique_ptr<Model> model)
{
    assert(m_models.find(name) == m_models.end());
    m_models[name].swap(model);
}

//------------------------------------------------------------------------------
Object *Scene::createObject(const std::string &modelName, ObjectGroup *parent)
{
    if (!parent)
    {
        parent = m_rootGroup.get();
    }

    assert(m_models.find(modelName) != m_models.end());
    Model *model = m_models[modelName].get();
    Object *object = new Object(model);
    object->setName(QString::fromStdString(modelName)); //TODO: discuss how to choose identifiers!
    m_objects.push_back(object); // construct unique_ptr automatically
    parent->addObject(std::unique_ptr<Object>(object));
    return object;
}

//------------------------------------------------------------------------------
ObjectGroup *Scene::createObjectGroup(const std::string &name, ObjectGroup *parent)
{
    if (!parent)
    {
        parent = m_rootGroup.get();
    }

    ObjectGroup *newGroup = new ObjectGroup();
    newGroup->setName(QString::fromStdString(name));
    parent->addObjectGroup(std::unique_ptr<ObjectGroup>(newGroup));

    return newGroup;
}

//------------------------------------------------------------------------------
range<Scene::ObjectIterator> Scene::getObjects()
{
    return range<ObjectIterator>(m_objects.cbegin(),m_objects.cend());
}

//------------------------------------------------------------------------------
range<Scene::ModelIterator> Scene::getModels()
{
    return range<ModelIterator>(m_models.cbegin(), m_models.cend());
}

//------------------------------------------------------------------------------
QVector3D &Scene::getDirectionalLightDirection()
{
    return m_directionalLightDirection;
}

//------------------------------------------------------------------------------
QVector3D &Scene::getLightColor()
{
    return m_lightColor;
}

//------------------------------------------------------------------------------
void Scene::setLightColor(const QVector3D &color)
{
    this->m_lightColor = color;
}

//------------------------------------------------------------------------------
void Scene::setDirectionalLightDirection(const QVector3D &direction)
{
    this->m_directionalLightDirection = direction;
}

//------------------------------------------------------------------------------
void Scene::loadScene(std::string &filename)
{
    //TODO
}

//------------------------------------------------------------------------------
void Scene::saveScene(std::string &filename)
{
    //TODO
}

//------------------------------------------------------------------------------
ObjectGroup *Scene::getSceneRoot()
{
    return m_rootGroup.get();
}



