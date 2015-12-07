#include "SceneEditor/SceneEditorGame.h"
#include "Scene/ObjectBase.h"

void SceneEditorGame::initialize()
{
    m_scene.reset(new Scene());
}

void SceneEditorGame::resize(int width, int height)
{

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
