#include "sceneeditor/SceneEditorGame.h"

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

void SceneEditorGame::onKeyEvent(int Key)
{
    // blupp
}
