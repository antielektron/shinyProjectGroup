#include "SceneEditor/SceneEditorGame.h"
#include "Scene/ObjectBase.h"

SceneEditorGame::SceneEditorGame() : QObject(nullptr), m_dummyCurrentObject(nullptr)
{}

void SceneEditorGame::initialize()
{
    m_scene.reset(new Scene());

    m_scene->addModel("octo", std::unique_ptr<Model>(new Model("models/octonorm.obj")));
    m_dummyCurrentObject = m_scene->createObject("octo");

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

void SceneEditorGame::onDoubleClick()
{

}

void SceneEditorGame::onMouseButtonDown(int button)
{

}

void SceneEditorGame::onMouseButtonUp(int button)
{

}

void SceneEditorGame::onMouseMove(int x, int y)
{

}

void SceneEditorGame::onKeyDown(int key)
{

}

void SceneEditorGame::onKeyUp(int key)
{

}

ObjectBase *SceneEditorGame::getRootObject()
{
    return m_objectRoot;
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

ObjectGroup *SceneEditorGame::getRootObject()
{
    // TODO
    return nullptr;
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
