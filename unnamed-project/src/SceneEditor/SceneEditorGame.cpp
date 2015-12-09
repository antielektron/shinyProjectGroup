#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"

SceneEditorGame::SceneEditorGame() : QObject(nullptr), m_dummyCurrentObject(nullptr)
{}

void SceneEditorGame::initialize()
{
    m_scene.reset(new Scene());

    //create a Root object
    //(so far only managed by us)
    m_objectRoot.reset(new ObjectGroup());

    //such casting, much wow
    ObjectGroup *root = static_cast<ObjectGroup*>(m_objectRoot.get());

    root->setName("Root Object");

    this->addModel("octo", std::unique_ptr<Model>(new Model("models/octonorm.obj")));
    m_dummyCurrentObject = m_scene->createObject("octo");

    //FIXME: this will result in a double free!
    root->addObject(m_dummyCurrentObject);

    m_scene->getCamera().translate(0., 0., -10);

    m_scene->setDirectionalLightDirection(QVector3D(0.0,0.0,-1.0));
    m_scene->setLightColor(QVector3D(1.0,1.0,1.0));

    emit currentObjectChanged();
}

void SceneEditorGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void SceneEditorGame::tick()
{

}

Scene *SceneEditorGame::getScene()
{
    return m_scene.get();
}


void SceneEditorGame::currentObjectModified()
{
    m_dummyCurrentObject->updateWorld();
    emit currentObjectChanged();
}

void SceneEditorGame::getModels(std::vector<Model *> &models)
{
    // TODO
}


ObjectGroup *SceneEditorGame::getCurrentObjectGroup()
{
    // TODO
    return nullptr;
}

Object *SceneEditorGame::getCurrentObject()
{
    // TODO
    return m_dummyCurrentObject;
}

void SceneEditorGame::addModel(const std::string &name, std::unique_ptr<Model> model)
{
    m_scene->addModel(name,std::move(model));
    emit modelsChanged();
}

